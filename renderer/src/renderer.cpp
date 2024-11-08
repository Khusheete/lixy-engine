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
#include "renderer/src/mesh.hpp"

#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/ext/matrix_clip_space.hpp"
#include "thirdparty/glm/matrix.hpp"

#include <GLFW/glfw3.h>


namespace lixy {

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


    Renderer *Renderer::get_singleton(flecs::world &p_world) {
        return p_world.get_mut<Renderer>();
    }


    void Renderer::_initialize(flecs::world &p_world) {
        context.initialize();

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
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
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

        p_world.system<const Renderer, const ArrayMeshInstance, Transform>("Draw ArrayMeshInstances")
            .term_at(0).singleton()
            .with<Visible>()
            .kind(flecs::PreStore)
            .each([](const Renderer &rd, const ArrayMeshInstance &p_instance, Transform &p_transform) {
                const ArrayMesh *mesh = p_instance.array_mesh.get<ArrayMesh>();
                mesh->record_draw(rd.projection_matrix, rd.view_matrix, p_transform.get_matrix());
            });
        
        p_world.system<Renderer>("Swap Buffers")
            .term_at(0).singleton()
            .kind(flecs::OnStore)
            .each([](Renderer &rd) {
                rd.context.swap_buffers();
            });
    }
}