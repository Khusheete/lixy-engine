#include "mesh.hpp"

#include "core/src/ref.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include <array>
#include <memory>


namespace lixy {

    const BufferLayout &Vertex::get_layout() {
        static const std::array<ShaderDataType, 1> layer_list = {
            ShaderDataType::Vec3
        };
        static const BufferLayout buffer_layout(layer_list.data(), layer_list.size());
        return buffer_layout;
    }


    void ArrayMesh::record_draw() const {
        for (const Surface &surface : surfaces) {
            surface.material.get<Material>()->bind_material();
            surface.vertex_array.bind();
            glDrawElements(GL_TRIANGLES, surface.index_count, GL_UNSIGNED_INT, nullptr);
        }
    }


    void ArrayMesh::add_surface(const std::vector<Vertex> &p_vertices, const std::vector<uint32_t> &p_indices, const EntityRef &p_material) {
        std::shared_ptr<VertexBuffer> vertex_buffer = std::make_shared<VertexBuffer>(p_vertices.data(), p_vertices.size() * sizeof(Vertex));
        std::shared_ptr<IndexBuffer> index_buffer = std::make_shared<IndexBuffer>(p_indices.data(), p_indices.size());
        VertexArrayBuffer vertex_array(vertex_buffer, index_buffer, Vertex::get_layout());

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