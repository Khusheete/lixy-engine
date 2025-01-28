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

#include "thirdparty/glm/fwd.hpp"
#include "thirdparty/glad/include/glad/glad.h"

#include <cstdint>
#include <vector>
#include <memory>


namespace lixy::opengl {

    enum class ShaderDataType;

    class BufferLayout {
    public:
        uint32_t get_attribute_offset(uint32_t index) const;
        uint32_t get_attribute_size(uint32_t index) const;
        GLint get_attribute_gl_type(uint32_t index) const;
        uint32_t get_attribute_component_count(uint32_t index) const;

        inline uint32_t get_stride() const { return stride; }
        inline uint32_t get_layout_length() const { return layout_length; }

        BufferLayout(const ShaderDataType *p_layout, uint32_t p_layout_length);

    private:
        struct Attribute {
            ShaderDataType data_type;
            uint32_t offset;
        };

    private:
        std::vector<Attribute> attributes;
        uint32_t stride;
        uint32_t layout_length;
    };


    class VertexBuffer {
    public:
        void bind() const;
        void unbind() const;

        VertexBuffer(const void *p_vertices, uint32_t p_size);
        VertexBuffer(const VertexBuffer &p_other) = delete;
        virtual ~VertexBuffer();

    private:
        uint32_t buffer_id;
    };


    class IndexBuffer {
    public:
        void bind() const;
        void unbind() const;

        IndexBuffer(const uint32_t *p_indices, uint32_t p_size);
        IndexBuffer(const IndexBuffer &p_other) = delete;
        virtual ~IndexBuffer();

    private:
        uint32_t buffer_id;
    };


    class VertexArrayBuffer {
    public:
        void bind() const;
        void unbind() const;

        void add_vertex_buffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, const BufferLayout &p_buffer_layout);
        void add_index_buffer(std::shared_ptr<IndexBuffer> p_index_buffer);

        VertexArrayBuffer();
        VertexArrayBuffer(const VertexArrayBuffer&) = delete;
        VertexArrayBuffer(VertexArrayBuffer &&p_other);
        VertexArrayBuffer &operator=(VertexArrayBuffer &&p_other);
        virtual ~VertexArrayBuffer();

    private:
        uint32_t array_index;
        uint32_t vertex_attrib_count;

        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;
        std::vector<std::shared_ptr<IndexBuffer>> index_buffers;
    };


    class ShaderStorageBuffer {
    public:
        class Slice {
        public:
            void bind_to_location(uint32_t p_bind_location) const;

            Slice() = default;
        private:
            inline Slice(uint32_t p_buffer_id, uint32_t p_offset, uint32_t p_size)
                : buffer_id(p_buffer_id),
                offset(p_offset),
                size(p_size) {}

        private:
            uint32_t buffer_id = 0;
            uint32_t offset;
            uint32_t size;
            
            friend ShaderStorageBuffer;
        };

    public:

        void bind() const;
        void unbind() const;

        Slice slice(uint32_t p_offset, uint32_t p_size) const;

        void allocate(uint32_t p_size);
        uint32_t get_size() const;

        void write_data(uint32_t p_offset, uint32_t p_size, void *p_data);

        ShaderStorageBuffer() = default;
        ShaderStorageBuffer(ShaderStorageBuffer &&p_other);
        ShaderStorageBuffer &operator=(ShaderStorageBuffer &&p_other);
        virtual ~ShaderStorageBuffer();
    
    private:
        uint32_t buffer_id = 0;
        uint32_t size = 0;
    };
}