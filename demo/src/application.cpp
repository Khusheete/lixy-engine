#include "application.hpp"

#include "debug/debug.hpp"
#include "renderer/src/shader.hpp"
#include "renderer/src/vbuffer.hpp"
#include <GL/gl.h>
#include <array>
#include <cstdlib>
#include <memory>


void DemoApplication::main_loop() {
    while (!context.window_should_close()) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        vertex_array_buffer->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        vertex_array_buffer->unbind();
        shader->unbind();

        context.swap_buffers();
    }
}


DemoApplication::DemoApplication() {
    context.initialize();
    context.window_set_title("Demo OpenGL Application");

    // Create mesh
    std::array<float, 8> vertices = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f,
    };

    std::array<ShaderDataType, 1> layout = {
        ShaderDataType::Vec2
    };

    std::array<uint32_t, 6> indices {
        0, 1, 2, 2, 3, 1
    };

    vertex_buffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    index_buffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    vertex_array_buffer = std::make_shared<VertexArrayBuffer>(
        vertex_buffer,
        index_buffer,
        layout.data(),
        layout.size()
    );

    // Create shader
    shader = std::make_shared<Shader>(
        Shader::load_shader_program("assets/shaders/shader.vert", "assets/shaders/shader.frag")
    );
    ASSERT_FATAL_ERROR(shader->is_valid(), shader->get_errors());
}