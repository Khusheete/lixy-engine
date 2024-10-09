#include "vbuffer.hpp"

#include <cstdint>
#include <GL/glew.h>


void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}


void VertexBuffer::unbind() {
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


void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
}


void IndexBuffer::unbind() {
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


void VertexArrayBuffer::bind() {
    glBindVertexArray(array_index);
}


void VertexArrayBuffer::unbind() {
    glBindVertexArray(0);
}


VertexArrayBuffer::VertexArrayBuffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, std::shared_ptr<IndexBuffer> p_index_buffer, const ShaderDataType *p_layout, uint32_t p_layout_size)
    : vertex_buffer(p_vertex_buffer),
    index_buffer(p_index_buffer)
{
    glGenVertexArrays(1, &array_index);
    bind();
    
    // Bind vertex buffer
    p_vertex_buffer->bind();

    // Set vertex buffer layout
    uint32_t vertex_size = 0;
    for (int i = 0; i < p_layout_size; i++)
        vertex_size += shader_data_type_get_size(p_layout[i]);

    uint32_t offset = 0;
    for (int i = 0; i < p_layout_size; i++) {
        ShaderDataType data_type = p_layout[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            shader_data_type_get_component_count(data_type),
            shader_data_type_get_gl_type(data_type),
            GL_FALSE,
            vertex_size,
            reinterpret_cast<void*>(&offset)
        );
        offset += shader_data_type_get_size(data_type);
    }

    // Bind index buffer
    index_buffer->bind();

    unbind();
}


VertexArrayBuffer::~VertexArrayBuffer() {
    glDeleteVertexArrays(1, &array_index);
}