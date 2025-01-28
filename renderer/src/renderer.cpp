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

#include "core/src/ref.hpp"
#include "core/src/transform.hpp"
#include "debug/debug.hpp"
#include "renderer/src/camera.hpp"
#include "renderer/src/framebuffer.hpp"
#include "renderer/src/light.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"

#include "renderer/src/primitives/context.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/primitives/texture.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/ext/matrix_clip_space.hpp"
#include "thirdparty/glm/matrix.hpp"
#include "windowing/src/window.hpp"

#include <array>
#include <memory>


namespace lixy {

    static const std::string DEFAULT_VERTEX_SHADER =
        "#version 430 core\n"
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
        "    out_position = u_model * vec4(position, 1.0);"
        "    gl_Position = u_projection * u_view * out_position;"
        "    out_normal = mat3(transpose(inverse(u_model))) * normal;"
        "    out_tex_coord = tex_coord;"
        "}";
    
    static const std::string DEFAULT_FRAGMENT_SHADER =
        "#version 430 core\n"
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
        "    out_normal = vec4(normalize(normal), 1.0);"
        "}";


    static const std::string SCREEN_VERTEX_SHADER =
        "#version 430 core\n"
        ""
        "layout(location = 0) in vec2 position;"
        "layout(location = 1) in vec2 uv;"
        ""
        "layout(location = 0) out vec2 out_uv;"
        ""
        "void main() {"
        "    gl_Position = vec4(position, 0.0, 1.0);"
        "    out_uv = uv;"
        "}";

    static const std::string SCREEN_FRAGMENT_SHADER =
        "#version 430 core\n"
        ""
        "layout(location = 0) in vec2 uv;"
        ""
        "struct PointLight {"
        "    float r;" // Separate the color elements for better struct alignment
        "    float g;"
        "    float b;"
        "    float energy;"
        "    float radius;"
        "};"
        ""
        "layout(std430, binding = 0) readonly buffer PointLightsData {"
        "    PointLight point_lights_data[];"
        "};"
        ""
        "layout(std430, binding = 1) readonly buffer PointLightsPosition {"
        "    vec4 point_lights_position[];"
        "};"
        ""
        "layout(location = 0) out vec4 out_color;"
        ""
        "uniform sampler2D u_position;"
        "uniform sampler2D u_color;"
        "uniform sampler2D u_normal;"
        ""
        "void main() {"
        "    vec3 position = texture(u_position, uv).rgb;"
        "    vec3 color = texture(u_color, uv).rgb;"
        "    vec3 normal = texture(u_normal, uv).rgb;"
        ""
        "    vec3 lighting = 0.1 * color;"
        ""
        "    for (int i = 0; i < point_lights_data.length(); i++) {"
        "        vec3 light_screen_position = point_lights_position[i].xyz;"
        "        vec3 light_direction = normalize(light_screen_position - position);"
        ""
        "        vec3 light_color = vec3(point_lights_data[i].r, point_lights_data[i].g, point_lights_data[i].b);"
        "        vec3 diffuse = max(dot(normal, light_direction), 0.0) * light_color * color * point_lights_data[i].energy;"
        "        lighting += diffuse;"
        "    }"
        ""
        "    out_color = vec4(lighting, 1.0);"
        "}";


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


    void Renderer::_initialize(flecs::world &p_world, flecs::entity &p_self) {
        // Create opengl context
        context.initialize(&Window::get_proc_address);

        // Create gbuffer
        {
            std::vector<opengl::TextureFormat> g_framebuffer_formats = {
                opengl::TextureFormat::RGBA16F, // Position
                opengl::TextureFormat::RGBA16F, // Albedo
                opengl::TextureFormat::RGBA16F, // Normal
            };
            Window *window = p_world.get_mut<Window>();
            int width = window->get_width(), height = window->get_height();
            gbuffer_ref = Framebuffer::create(p_world, width, height, g_framebuffer_formats)
                .add(flecs::ChildOf, p_self);
            ASSERT_FATAL_ERROR(gbuffer_ref.get<Framebuffer>()->is_complete(), "Incomplete GBuffer");
        }

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
        p_world.system<Renderer, Window>("Start Frame")
            .term_at(0).singleton()
            .term_at(1).singleton()
            .kind(flecs::PreUpdate)
            .each([](Renderer &rd, Window &window) {
                window.set_as_current_context();
                window.poll_events();

                // Set viewport size
                int width = window.get_width(), height = window.get_height();
                glViewport(0, 0, width, height); // FIXME: resize when there is a resize event

                // Clear screen
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                // Set the current framebuffer to the gbuffer, resize it and clear it
                Framebuffer *gbuffer = rd.gbuffer_ref.get_mut<Framebuffer>();
                gbuffer->bind();
                gbuffer->set_size(width, height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            });

        p_world.system<Renderer, Window>("Calculate Camera Transform")
            .term_at(0).singleton()
            .term_at(1).singleton()
            .kind(flecs::PostUpdate)
            .each([](Renderer &rd, Window &window) {
                if (!rd.current_camera.is_alive()) return;

                const Camera *camera = rd.current_camera.get<Camera>();
                Transform *camera_transform = rd.current_camera.get_mut<Transform>();

                int width = window.get_width(), height = window.get_height();

                // Calculate view matrix transform
                rd.view_matrix = glm::inverse(camera_transform->get_matrix());

                // Calculate projection matrix
                switch (camera->type) {
                case Camera::Type::PERSPECTIVE:
                {
                    float aspect_ratio = (float) width / height;
                    rd.projection_matrix = glm::perspective(camera->fov, aspect_ratio, camera->clip_near, camera->clip_far);
                    break;
                }
                case Camera::Type::ORTHOGRAPHIC:
                {
                    rd.projection_matrix = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, camera->clip_near, camera->clip_far);
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


        p_world.system("Draw Lights And Present")
            .kind(flecs::OnStore)
            .immediate()
            .run([](flecs::iter &it) {
                Renderer *rd = it.world().get_mut<Renderer>();
                Window *window = it.world().get_mut<Window>();

                // Render to the main window
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // Fetch point lights data
                flecs::query<PointLight, Transform> point_light_query = it.world().query_builder<PointLight, Transform>()
                    .with<Visible>()
                    .cached()
                    .build();

                int32_t point_light_count = point_light_query.count();
                int32_t light_buffer_size = point_light_count * sizeof(PointLight);
                int32_t light_transform_buffer_size = point_light_count * sizeof(glm::vec4);

                if (rd->point_lights_storage.get_size() < light_buffer_size) {
                    rd->point_lights_storage.allocate(light_buffer_size);
                }
                if (rd->point_lights_transform_storage.get_size() < light_transform_buffer_size) {
                    rd->point_lights_transform_storage.allocate(light_transform_buffer_size);
                }

                point_light_query.run([&rd, point_light_count, light_transform_buffer_size](flecs::iter &it) {
                    int32_t point_light_total = 0;

                    std::vector<glm::vec4> transform_buffer;
                    transform_buffer.reserve(point_light_count);

                    while (it.next()) {
                        auto point_lights = it.field<PointLight>(0);
                        rd->point_lights_storage.write_data(point_light_total * sizeof(PointLight), it.count() * sizeof(PointLight), &*point_lights);

                        auto transforms = it.field<Transform>(1);
                        for (int32_t i = 0; i < it.count(); i++) {
                            transform_buffer[point_light_total + i] = glm::vec4(transforms[i].get_local_position(), 1.0);
                        }

                        point_light_total += it.count();
                    }

                    rd->point_lights_transform_storage.write_data(0, light_transform_buffer_size, transform_buffer.data());
                });

                // Render the lit scene to the screen fbo
                rd->screen_material.set_uniform("PointLightsData", rd->point_lights_storage.slice(0, light_buffer_size));
                rd->screen_material.set_uniform("PointLightsPosition", rd->point_lights_transform_storage.slice(0, light_transform_buffer_size));
                rd->screen_material.bind_material();
                rd->quad_vao->bind();
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                rd->quad_vao->unbind();

                window->swap_buffers();
            });
    }
}