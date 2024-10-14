#include "vbuffer.hpp"

#include "shader.hpp"

#include <cstdint>
#include <GL/glew.h>


namespace lixy::opengl {

    uint32_t BufferLayout::get_attribute_offset(uint32_t index) const {
        return attributes[index].offset;
    }


    uint32_t BufferLayout::get_attribute_size(uint32_t index) const {
        return shader_data_type_get_size(attributes[index].data_type);
    }
    
    
    GLint BufferLayout::get_attribute_gl_type(uint32_t index) const {
        return shader_data_type_get_gl_type(attributes[index].data_type);
    }
    
    
    uint32_t BufferLayout::get_attribute_component_count(uint32_t index) const {
        return shader_data_type_get_component_count(attributes[index].data_type);
    }


    BufferLayout::BufferLayout(const ShaderDataType *p_layout, uint32_t p_layout_length)
        : attributes(p_layout_length),
        layout_length(p_layout_length)
    {
        uint32_t current_offset = 0;
        for (uint32_t i = 0; i < p_layout_length; i++) {
            attributes[i].data_type = p_layout[i];
            attributes[i].offset = current_offset;
            current_offset += shader_data_type_get_size(p_layout[i]);
        }
        stride = current_offset;
    }


    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    }


    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    VertexBuffer::VertexBuffer(const void *p_vertices, uint32_t p_size) {
        glGenBuffers(1, &buffer_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, p_size, p_vertices, GL_STATIC_DRAW);
    }


    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &buffer_id);
    }


    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    }


    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    IndexBuffer::IndexBuffer(const uint32_t *p_indices, uint32_t p_size) {
        glGenBuffers(1, &buffer_id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_size * sizeof(uint32_t), p_indices, GL_STATIC_DRAW);
    }


    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &buffer_id);
    }


    void VertexArrayBuffer::bind() const {
        glBindVertexArray(array_index);
    }


    void VertexArrayBuffer::unbind() const {
        glBindVertexArray(0);
    }


    VertexArrayBuffer::VertexArrayBuffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, std::shared_ptr<IndexBuffer> p_index_buffer, const BufferLayout &p_buffer_layout)
        : vertex_buffer(p_vertex_buffer),
        index_buffer(p_index_buffer)
    {
        glGenVertexArrays(1, &array_index);
        bind();

        // Bind vertex buffer
        p_vertex_buffer->bind();

        for (uint32_t i = 0; i < p_buffer_layout.get_layout_length(); i++) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                i,
                p_buffer_layout.get_attribute_component_count(i),
                p_buffer_layout.get_attribute_gl_type(i),
                GL_FALSE,
                p_buffer_layout.get_stride(),
                reinterpret_cast<void*>(p_buffer_layout.get_attribute_offset(i))
            );
        }

        // Bind index buffer
        index_buffer->bind();

        unbind();
    }


    VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer &&p_other)
        : array_index(p_other.array_index),
        vertex_buffer(p_other.vertex_buffer),
        index_buffer(p_other.index_buffer)
    {
        p_other.array_index = 0;
    }


    VertexArrayBuffer &VertexArrayBuffer::operator=(VertexArrayBuffer &&p_other) {
        if (array_index) {
            glDeleteVertexArrays(1, &array_index);
        }

        array_index = p_other.array_index;
        vertex_buffer = p_other.vertex_buffer;
        index_buffer = p_other.index_buffer;

        p_other.array_index = 0;
        
        return *this;
    }


    VertexArrayBuffer::~VertexArrayBuffer() {
        if (!array_index) return;

        glDeleteVertexArrays(1, &array_index);
    }
}