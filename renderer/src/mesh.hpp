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

        static const BufferLayout &get_layout();
    };


    class ArrayMesh {
    public:
        void record_draw() const;
        void add_surface(const std::vector<Vertex> &p_vertices, const std::vector<uint32_t> &p_indices, const EntityRef &p_material);

        static EntityRef create(flecs::world &p_world);

        ArrayMesh() = default;
        ArrayMesh(ArrayMesh&&) = default;
        ArrayMesh &operator=(ArrayMesh&&) = default;
        virtual ~ArrayMesh() = default;

    private:
        struct Surface {
            std::shared_ptr<VertexBuffer> vertex_buffer;
            std::shared_ptr<IndexBuffer> index_buffer;
            VertexArrayBuffer vertex_array;
            uint32_t index_count;

            EntityRef material;
        };

    private:
        std::vector<Surface> surfaces;
    };
}