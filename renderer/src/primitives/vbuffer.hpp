#pragma once

#include "debug/debug.hpp"
#include "shader.hpp"
#include "thirdparty/glm/fwd.hpp"
#include <GL/glew.h>

#include <cstdint>
#include <vector>
#include <memory>


namespace lixy::opengl {

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

        VertexArrayBuffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, std::shared_ptr<IndexBuffer> p_index_buffer, const BufferLayout &p_buffer_layout);
        VertexArrayBuffer(const VertexArrayBuffer&) = delete;
        VertexArrayBuffer(VertexArrayBuffer &&p_other);
        VertexArrayBuffer &operator=(VertexArrayBuffer &&p_other);
        virtual ~VertexArrayBuffer();

    private:
        uint32_t array_index;

        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<IndexBuffer> index_buffer;
    };
}