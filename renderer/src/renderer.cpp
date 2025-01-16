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


#include "renderer.hpp"

#include "core/src/transform.hpp"
#include "debug/debug.hpp"
#include "renderer/src/camera.hpp"
#include "renderer/src/framebuffer.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"

#include "renderer/src/primitives/context.hpp"
#include "renderer/src/primitives/framebuffer.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/primitives/texture.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/ext/matrix_clip_space.hpp"
#include "thirdparty/glm/matrix.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <memory>


namespace lixy {

    static const std::string DEFAULT_VERTEX_SHADER =
        "#version 420 core\n"
        ""
        "layout(location = 0) in vec3 position;"
        "layout(location = 1) in vec3 normal;"
        "layout(location = 2) in vec2 tex_coord;"
        ""
        "uniform mat4 u_model;"
        "uniform mat4 u_view;"
        "uniform mat4 u_projection;"
        ""
        "layout(location = 0) out vec4 out_position;"
        "layout(location = 1) out vec3 out_normal;"
        "layout(location = 2) out vec2 out_tex_coord;"
        ""
        "void main() {"
        "    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);"
        "    out_position = gl_Position;"
        "    out_normal = (u_projection * u_view * u_model * vec4(normal, 0.0)).xyz;" // FIXME: project normal
        "    out_tex_coord = tex_coord;"
        "}";
    
    static const std::string DEFAULT_FRAGMENT_SHADER =
        "#version 420 core\n"
        ""
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec3 normal;"
        "layout(location = 2) in vec2 tex_coord;"
        ""
        "layout(location = 0) out vec4 out_position;"
        "layout(location = 1) out vec4 out_color;"
        "layout(location = 2) out vec4 out_normal;"
        ""
        "uniform sampler2D u_albedo_texture;"
        "uniform vec3 u_albedo_offset = vec3(0.0, 0.0, 0.0);"
        "uniform vec3 u_albedo_scale = vec3(1.0, 1.0, 1.0);"
        ""
        "void main() {"
        "    out_position = position;"
        "    out_color = texture(u_albedo_texture, u_albedo_offset.xy + tex_coord * u_albedo_scale.xy);"
        "    out_normal = vec4(normalize(normal), 1.0);" // FIXME: use a vec3 - currently using a vec3 gives nonsense images
        "}";


    static const std::string SCREEN_VERTEX_SHADER =
        "#version 420 core\n"
        ""
        "layout(location = 0) in vec2 position;"
        "layout(location = 1) in vec2 uv;"
        ""
        "layout(location = 1) out vec2 out_uv;"
        ""
        "void main() {"
        "    gl_Position = vec4(position, 0.0, 1.0);"
        "    out_uv = uv;"
        "}";

    static const std::string SCREEN_FRAGMENT_SHADER =
        "#version 420 core\n"
        ""
        "layout(location = 1) in vec2 uv;"
        ""
        "layout(location = 0) out vec4 out_color;"
        ""
        "uniform sampler2D u_position;"
        "uniform sampler2D u_color;"
        "uniform sampler2D u_normal;"
        ""
        "void main() {"
        "    out_color = texture(u_color, uv);"
        "}";


    void Renderer::window_set_title(const std::string &p_title) {
        context.window_set_title(p_title);
    }
        
        
    bool Renderer::window_should_close() {
        return context.window_should_close();
    }


    void Renderer::set_current_camera(flecs::entity p_camera) {
        ASSERT_FATAL_ERROR(p_camera.has<Camera>(), "The provided entity is not a camera");
        ASSERT_FATAL_ERROR(p_camera.has<Transform>(), "The provided camera has no transform component");

        current_camera = p_camera;
    }


    flecs::entity Renderer::get_current_camera() const {
        return current_camera;
    }


    EntityRef Renderer::create_default_material(flecs::world &p_world) {
        return EntityRef::create_reference(p_world).set<Material>(default_material);
    };


    Renderer *Renderer::get_singleton(flecs::world &p_world) {
        return p_world.get_mut<Renderer>();
    }


    void Renderer::_initialize(flecs::world &p_world) {
        // Create opengl context
        context.initialize();

        // Create gbuffer
        std::vector<opengl::TextureFormat> g_framebuffer_formats = {
            opengl::TextureFormat::RGBA8, // Position
            opengl::TextureFormat::RGBA8, // Albedo
            opengl::TextureFormat::RGBA8, // Normal
        };
        int width, height; // FIXME: width and height should be managed inside the windowing system
        glfwGetFramebufferSize(context.get_window(), &width, &height);
        gbuffer_ref = Framebuffer::create(p_world, width, height, g_framebuffer_formats);
        ASSERT_FATAL_ERROR(gbuffer_ref.get<Framebuffer>()->is_complete(), "Incomplete GBuffer");

        // Create materials
        default_material = Material(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
        ASSERT_FATAL_ERROR(default_material.is_valid(), "The default material is invalid: " << default_material.get_errors());

        // Create surface to render on the screen
        const std::array<glm::vec2, 8> quad_vertices_array = {
            glm::vec2(-1.0, -1.0), glm::vec2(0.0, 0.0),
            glm::vec2(-1.0, 1.0), glm::vec2(0.0, 1.0),
            glm::vec2(1.0, -1.0), glm::vec2(1.0, 0.0),
            glm::vec2(1.0, 1.0), glm::vec2(1.0, 1.0),
        };
        const std::array<uint32_t, 6> quad_indices_array = {
            0, 1, 2, 2, 1, 3
        };
        std::array<opengl::ShaderDataType, 2> quad_buffer_layout_array = {
            opengl::ShaderDataType::Vec2, opengl::ShaderDataType::Vec2,
        };
        opengl::BufferLayout quad_buffer_layout(quad_buffer_layout_array.data(), quad_buffer_layout_array.size());

        quad_vertices = std::make_shared<opengl::VertexBuffer>(
            quad_vertices_array.data(),
            quad_vertices_array.size() * sizeof(quad_vertices_array[0])
        );
        quad_indices = std::make_shared<opengl::IndexBuffer>(
            quad_indices_array.data(),
            quad_indices_array.size()
        );
        quad_vao = std::make_shared<opengl::VertexArrayBuffer>();
        quad_vao->add_index_buffer(quad_indices);
        quad_vao->add_vertex_buffer(quad_vertices, quad_buffer_layout);

        screen_material = Material(SCREEN_VERTEX_SHADER, SCREEN_FRAGMENT_SHADER);
        ASSERT_FATAL_ERROR(screen_material.is_valid(), "The screen material is invalid: " << screen_material.get_errors());
        { // Setup material uniforms
            Framebuffer *gbuffer = gbuffer_ref.get_mut<Framebuffer>();
            screen_material.set_uniform("u_position", gbuffer->get_attachment(0));
            screen_material.set_uniform("u_color", gbuffer->get_attachment(1));
            screen_material.set_uniform("u_normal", gbuffer->get_attachment(2));
        }


        // Create renderer systems
        p_world.system<Renderer>("Start Frame")
            .term_at(0).singleton()
            .kind(flecs::PreUpdate)
            .each([](Renderer &rd) {
                rd.context.set_current();
                glfwPollEvents();

                // Set viewport size
                int width, height; // FIXME: width and height should be managed inside the windowing system
                glfwGetFramebufferSize(rd.context.get_window(), &width, &height);
                glViewport(0, 0, width, height); // FIXME: resize when there is a resize event

                // Clear screen
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                // Set the current framebuffer to the gbuffer, resize it and clear it
                Framebuffer *gbuffer = rd.gbuffer_ref.get_mut<Framebuffer>();
                gbuffer->bind();
                // rd.gbuffer.set_size(width, height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            });

        p_world.system<Renderer>("Calculate Camera Transform")
            .term_at(0).singleton()
            .kind(flecs::PostUpdate)
            .each([](Renderer &rd) {
                if (!rd.current_camera.is_alive()) return;

                const Camera *camera = rd.current_camera.get<Camera>();
                Transform *camera_transform = rd.current_camera.get_mut<Transform>();

                int width, height; // FIXME: width and height should be managed inside the windowing system
                glfwGetFramebufferSize(rd.context.get_window(), &width, &height);

                // Calculate view matrix transform
                rd.view_matrix = glm::inverse(camera_transform->get_matrix());

                // Calculate projection matrix
                switch (camera->type) {
                case Camera::Type::PERSPECTIVE:
                {
                    float aspect_ratio = (float) width / height;
                    rd.projection_matrix = glm::perspective(camera->fov, aspect_ratio, camera->near, camera->far);
                    break;
                }
                case Camera::Type::ORTHOGRAPHIC:
                {
                    rd.projection_matrix = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, camera->near, camera->far);
                    break;
                }
                }
            });

        p_world.system<const Renderer, const MeshInstance, Transform>("Draw MeshInstances")
            .term_at(0).singleton()
            .with<Visible>()
            .kind(flecs::PreStore)
            .each([](const Renderer &rd, const MeshInstance &p_instance, Transform &p_transform) {
                if (p_instance.mesh.has<ArrayMesh>()) {
                    const ArrayMesh *mesh = p_instance.mesh.get<ArrayMesh>();
                    mesh->record_draw(rd.projection_matrix, rd.view_matrix, p_transform.get_matrix());
                } else if (p_instance.mesh.has<ObjMesh>()) {
                    const ObjMesh *mesh = p_instance.mesh.get<ObjMesh>();
                    mesh->record_draw(rd.projection_matrix, rd.view_matrix, p_transform.get_matrix());
                }
            });
        
        p_world.system<Renderer>("End Frame")
            .term_at(0).singleton()
            .kind(flecs::OnStore)
            .each([](Renderer &rd) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                rd.screen_material.bind_material();
                rd.quad_vao->bind();
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                rd.quad_vao->unbind();

                rd.context.swap_buffers();
            });
    }
}