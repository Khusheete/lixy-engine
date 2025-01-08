/*
* Copyright 2024 Souchet Ferdinand
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
* Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "mesh.hpp"

#include "core/src/ref.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "renderer/src/renderer.hpp"
#include "wavefront_loader/src/object.hpp"

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>


namespace lixy {

    const opengl::BufferLayout &Vertex::get_layout() {
        static const std::array<opengl::ShaderDataType, 2> layer_list = {
            opengl::ShaderDataType::Vec3, opengl::ShaderDataType::Vec2
        };
        static const opengl::BufferLayout buffer_layout(layer_list.data(), layer_list.size());
        return buffer_layout;
    }


    void ArrayMesh::record_draw(const glm::mat4 &p_projection, const glm::mat4 &p_view, const glm::mat4 &p_model) const {
        for (const Surface &surface : surfaces) {
            const Material *material = surface.material.get<Material>();
            material->bind_material();
            material->bind_pvm(p_projection, p_view, p_model);

            surface.vertex_array.bind();
            glDrawElements(GL_TRIANGLES, surface.index_count, GL_UNSIGNED_INT, nullptr);
            surface.vertex_array.unbind();
        }
    }


    void ArrayMesh::add_surface(const std::vector<Vertex> &p_vertices, const std::vector<uint32_t> &p_indices, const EntityRef &p_material) {
        std::shared_ptr<opengl::VertexBuffer> vertex_buffer = std::make_shared<opengl::VertexBuffer>(p_vertices.data(), p_vertices.size() * sizeof(Vertex));
        std::shared_ptr<opengl::IndexBuffer> index_buffer = std::make_shared<opengl::IndexBuffer>(p_indices.data(), p_indices.size());
        opengl::VertexArrayBuffer vertex_array;
        vertex_array.bind();
        vertex_array.add_vertex_buffer(vertex_buffer, Vertex::get_layout());
        vertex_array.add_index_buffer(index_buffer);
        vertex_array.unbind();

        surfaces.push_back(Surface{
            vertex_buffer,
            index_buffer,
            std::move(vertex_array),
            (uint32_t)p_indices.size(),
            p_material
        });
    }


    EntityRef ArrayMesh::create(flecs::world &p_world) {
        return EntityRef::create_reference(p_world).add<ArrayMesh>();
    }


    void ObjMesh::record_draw(const glm::mat4 &p_projection, const glm::mat4 &p_view, const glm::mat4 &p_model) const {
        vertex_array->bind();
        for (const Surface &surface : surfaces) {
            const Material *material = surface.material.get<Material>();
            material->bind_material();
            material->bind_pvm(p_projection, p_view, p_model);
            surface.indices->bind();

            glDrawElements(GL_TRIANGLES, surface.index_count, GL_UNSIGNED_INT, nullptr);
        }
        vertex_array->unbind();
    }


    EntityRef ObjMesh::load(flecs::world &p_world, const std::filesystem::path &p_path) {
        EntityRef entity = EntityRef::create_reference(p_world).add<ObjMesh>();
        ObjMesh *mesh = entity.get_mut<ObjMesh>();

        mesh->vertex_array = std::make_unique<opengl::VertexArrayBuffer>();

        EntityRef default_material = Renderer::get_singleton(p_world)->create_default_material(p_world);

        // Load mesh from the file
        WavefrontMesh wavefront = WavefrontMesh::load(p_path);

        // Create vertex data suited for single index buffer for positions, normals and uvs
        std::vector<float> vertex_buffer;
        vertex_buffer.reserve((sizeof(wavefront.positions[0]) + sizeof(wavefront.normals[0]) + sizeof(wavefront.uvs[0])) * wavefront.positions.size() / sizeof(float));
        uint32_t vertex_count = 0;

        std::unordered_map<uint64_t, uint32_t> index_map;

        int object_index_buffer_size = 0;
        std::vector<uint32_t> object_indices;

        for (auto element : wavefront.objects) {
            WavefrontObject object = element.second;

            object_indices.reserve(object.position_indices.size());
            object_index_buffer_size = 0;

            // Build index buffer
            for (int i = 0; i < object.position_indices.size(); i++) {
                uint64_t position_index = object.position_indices[i];
                uint64_t normal_index = object.normal_indices[i];
                uint64_t uv_index = object.uv_indices[i];

                // Assume that indices hold in a 16 bit number
                uint64_t index_id = position_index + (normal_index << 0x10) + (uv_index << 0x20);

                auto mapped_index = index_map.find(index_id);
                if (mapped_index == index_map.end()) {
                    // Add the new vertex
                    glm::vec3 position = wavefront.positions[position_index];
                    glm::vec3 normal = wavefront.normals[normal_index];
                    glm::vec2 uv = wavefront.uvs[uv_index];
                    vertex_buffer.insert(vertex_buffer.end(), {
                        position.x,
                        position.y,
                        position.z,
                        normal.x,
                        normal.y,
                        normal.z,
                        uv.x,
                        uv.y,
                    });

                    if (object_indices.size() > object_index_buffer_size) {
                        object_indices[object_index_buffer_size] = vertex_count;
                    } else {
                        object_indices.push_back(vertex_count);
                    }
                    object_index_buffer_size += 1;

                    index_map[index_id] = vertex_count;
                    vertex_count += 1;
                } else {
                    if (object_indices.size() > object_index_buffer_size) {
                        object_indices[object_index_buffer_size] = mapped_index->second;
                    } else {
                        object_indices.push_back(mapped_index->second);
                    }
                    object_index_buffer_size += 1;
                }
            }

            // Create surface
            Surface surface{};
            surface.material = default_material;
            surface.indices = std::make_shared<opengl::IndexBuffer>(
                object_indices.data(),
                object_index_buffer_size
            );
            // mesh->vertex_array->add_index_buffer(surface.indices);
            surface.index_count = object_index_buffer_size;
            mesh->surfaces.push_back(std::move(surface));
        }

        mesh->vertex_buffer = std::make_shared<opengl::VertexBuffer>(
            vertex_buffer.data(),
            vertex_buffer.size() * sizeof(float)
        );
        mesh->vertex_array->add_vertex_buffer(mesh->vertex_buffer, ObjMesh::get_vertex_buffer_layout());

        return entity;
    }


    const opengl::BufferLayout &ObjMesh::get_vertex_buffer_layout() {
        static const std::array<opengl::ShaderDataType, 3> vertex_buffer_layout_array = {
            opengl::ShaderDataType::Vec3,
            opengl::ShaderDataType::Vec3,
            opengl::ShaderDataType::Vec2,
        };
        static const opengl::BufferLayout vertex_buffer_layout(vertex_buffer_layout_array.data(), vertex_buffer_layout_array.size());
        return vertex_buffer_layout;
    }
}