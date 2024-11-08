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
#include <array>
#include <memory>


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
        }
    }


    void ArrayMesh::add_surface(const std::vector<Vertex> &p_vertices, const std::vector<uint32_t> &p_indices, const EntityRef &p_material) {
        std::shared_ptr<opengl::VertexBuffer> vertex_buffer = std::make_shared<opengl::VertexBuffer>(p_vertices.data(), p_vertices.size() * sizeof(Vertex));
        std::shared_ptr<opengl::IndexBuffer> index_buffer = std::make_shared<opengl::IndexBuffer>(p_indices.data(), p_indices.size());
        opengl::VertexArrayBuffer vertex_array(vertex_buffer, index_buffer, Vertex::get_layout());

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
}