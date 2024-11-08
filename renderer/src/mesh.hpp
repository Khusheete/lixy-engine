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