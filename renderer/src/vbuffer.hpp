#pragma once

#include "debug/debug.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <cstdint>
#include <vector>
#include <memory>


class VertexBuffer {
public:
    void bind();
    void unbind();

    VertexBuffer(const void *p_vertices, uint32_t p_size);
    VertexBuffer(const VertexBuffer &p_other) = delete;
    VertexBuffer(VertexBuffer &&p_other);
    virtual ~VertexBuffer();

private:
    uint32_t buffer_id;
};


class IndexBuffer {
public:
    void bind();
    void unbind();

    IndexBuffer(const uint32_t *p_indices, uint32_t p_size);
    IndexBuffer(const IndexBuffer &p_other) = delete;
    virtual ~IndexBuffer();

private:
    uint32_t buffer_id;
};


class VertexArrayBuffer {
public:
    void bind();
    void unbind();

    VertexArrayBuffer(std::shared_ptr<VertexBuffer> p_vertex_buffer, std::shared_ptr<IndexBuffer> p_index_buffer, const ShaderDataType *p_layout, uint32_t p_layout_size);
    VertexArrayBuffer(const VertexArrayBuffer&) = delete;
    virtual ~VertexArrayBuffer();

private:
    uint32_t array_index;

    std::shared_ptr<VertexBuffer> vertex_buffer;
    std::shared_ptr<IndexBuffer> index_buffer;
};