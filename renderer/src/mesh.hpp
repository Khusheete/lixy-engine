#pragma once

#include "core/src/ref.hpp"

#include "primitives/shader.hpp"
#include "primitives/vbuffer.hpp"

#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/glm.hpp"

#include <memory>


namespace lixy {

    struct Vertex {
        glm::vec3 vertex_pos;
        glm::vec2 uv;

        static const opengl::BufferLayout &get_layout();
    };


    class ArrayMesh {
    public:
        void record_draw(const glm::mat4 &p_projection, const glm::mat4 &p_view, const glm::mat4 &p_model) const;
        void add_surface(const std::vector<Vertex> &p_vertices, const std::vector<uint32_t> &p_indices, const EntityRef &p_material);

        static EntityRef create(flecs::world &p_world);

        ArrayMesh() = default;
        ArrayMesh(ArrayMesh&&) = default;
        ArrayMesh &operator=(ArrayMesh&&) = default;
        virtual ~ArrayMesh() = default;

    private:
        struct Surface {
            std::shared_ptr<opengl::VertexBuffer> vertex_buffer;
            std::shared_ptr<opengl::IndexBuffer> index_buffer;
            opengl::VertexArrayBuffer vertex_array;
            uint32_t index_count;

            EntityRef material;
        };

    private:
        std::vector<Surface> surfaces;
    };
}