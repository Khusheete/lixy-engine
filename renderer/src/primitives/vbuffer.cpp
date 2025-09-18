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


#include "vbuffer.hpp"
#include "shader.hpp"

#include "thirdparty/glad/include/glad/glad.h"

#include <cstdint>


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


    void VertexArrayBuffer::add_vertex_buffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, const BufferLayout &p_buffer_layout) {
        bind();
        p_vertex_buffer->bind();
        
        for (uint32_t i = 0; i < p_buffer_layout.get_layout_length(); i++) {
            uint32_t index = vertex_attrib_count + i;
            glVertexAttribPointer(
                index,
                p_buffer_layout.get_attribute_component_count(i),
                p_buffer_layout.get_attribute_gl_type(i),
                GL_FALSE,
                p_buffer_layout.get_stride(),
                reinterpret_cast<void*>(p_buffer_layout.get_attribute_offset(i))
            );
            glEnableVertexAttribArray(index);
        }
        vertex_attrib_count += p_buffer_layout.get_layout_length();
        unbind();
        vertex_buffers.push_back(p_vertex_buffer);
    }


    void VertexArrayBuffer::add_index_buffer(std::shared_ptr<IndexBuffer> p_index_buffer) {
        bind();
        p_index_buffer->bind();
        unbind();
        index_buffers.push_back(p_index_buffer);
    }


    VertexArrayBuffer::VertexArrayBuffer()
        : vertex_attrib_count(0)
    {
        glGenVertexArrays(1, &array_index);
    }


    VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer &&p_other)
        : array_index(p_other.array_index),
        vertex_buffers(p_other.vertex_buffers),
        index_buffers(p_other.index_buffers),
        vertex_attrib_count(p_other.vertex_attrib_count)
    {
        p_other.array_index = 0;
    }


    VertexArrayBuffer &VertexArrayBuffer::operator=(VertexArrayBuffer &&p_other) {
        if (array_index) {
            glDeleteVertexArrays(1, &array_index);
        }

        array_index = p_other.array_index;
        vertex_attrib_count = p_other.vertex_attrib_count;
        vertex_buffers = std::move(p_other.vertex_buffers);
        index_buffers = std::move(p_other.index_buffers);

        p_other.array_index = 0;
        
        return *this;
    }


    VertexArrayBuffer::~VertexArrayBuffer() {
        if (!array_index) return;
        glDeleteVertexArrays(1, &array_index);
    }


    void ShaderStorageBuffer::Slice::bind_to_location(uint32_t p_bind_location) const {
        if (buffer_id) {
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, p_bind_location, buffer_id, offset, size);
        }
    }


    void ShaderStorageBuffer::bind() const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);
    }


    void ShaderStorageBuffer::unbind() const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }


    ShaderStorageBuffer::Slice ShaderStorageBuffer::slice(uint32_t p_offset, uint32_t p_size) const {
        return Slice(buffer_id, p_offset, p_size);
    }


    void ShaderStorageBuffer::allocate(uint32_t p_size) {
        size = p_size;
        if (!buffer_id) glCreateBuffers(1, &buffer_id);
        bind();
        glBufferData(GL_SHADER_STORAGE_BUFFER, p_size, nullptr, GL_STREAM_READ);
    }


    uint32_t ShaderStorageBuffer::get_size() const {
        return size;
    }


    void ShaderStorageBuffer::write_data(uint32_t p_offset, uint32_t p_size, void *p_data) {
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, p_offset, p_size, p_data);
    }


    ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer &&p_other) {
        if (buffer_id) {
            glDeleteBuffers(1, &buffer_id);
        }

        buffer_id = p_other.buffer_id;
        size = p_other.size;

        p_other.buffer_id = 0;
    }


    ShaderStorageBuffer &ShaderStorageBuffer::operator=(ShaderStorageBuffer &&p_other) {
        if (buffer_id) {
            glDeleteBuffers(1, &buffer_id);
        }

        buffer_id = p_other.buffer_id;
        size = p_other.size;

        p_other.buffer_id = 0;
        return *this;
    }


    ShaderStorageBuffer::~ShaderStorageBuffer() {
        if (buffer_id) glDeleteBuffers(1, &buffer_id);
    }
}
