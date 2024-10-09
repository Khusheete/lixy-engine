#pragma once

#include <memory>

#include "renderer/src/shader.hpp"
#include "renderer/src/vbuffer.hpp"
#include <renderer/src/context.hpp>


class DemoApplication {
public:
    void main_loop();

    DemoApplication();

private:
    lixy::OpenGLContext context;

    std::shared_ptr<lixy::Shader> shader;

    std::shared_ptr<lixy::VertexBuffer> vertex_buffer;
    std::shared_ptr<lixy::IndexBuffer> index_buffer;
    std::shared_ptr<lixy::VertexArrayBuffer> vertex_array_buffer;
};