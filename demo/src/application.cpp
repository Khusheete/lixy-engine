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


#include "application.hpp"

#include "core/src/module.hpp"
#include "core/src/ref.hpp"
#include "core/src/transform.hpp"
#include "debug/debug.hpp"
#include "renderer/src/camera.hpp"
#include "renderer/src/light.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/module.hpp"
#include "renderer/src/renderer.hpp"
#include "renderer/src/texture.hpp"
#include "thirdparty/glm/ext/matrix_transform.hpp"
#include "thirdparty/glm/ext/scalar_constants.hpp"
#include "thirdparty/glm/fwd.hpp"
#include "thirdparty/glm/trigonometric.hpp"
#include "windowing/src/module.hpp"
#include "windowing/src/window.hpp"

#include <GL/gl.h>
// #include <array>
#include <cstdlib>
// #include <memory>


void DemoApplication::main_loop() {
    while (!lixy::Window::get_singleton(world)->should_close()) {
        // Update entities
        rotation_angle += world.delta_time() * 0.5 * glm::pi<float>();

        lixy::Transform *transform = light1.get_mut<lixy::Transform>();
        transform->set_position(glm::vec3(
            3.0 * glm::cos(rotation_angle),
            -1.0 + 3.0 * glm::sin(rotation_angle),
            0.0
        ));

        transform = light2.get_mut<lixy::Transform>();
        transform->set_position(glm::vec3(
            3.0 * glm::cos(-rotation_angle),
            -1.0 + 3.0 * glm::sin(-rotation_angle),
            0.0
        ));

        transform = camera.get_mut<lixy::Transform>();
        transform->set_position(glm::vec3(
            0.5 * glm::cos(0.33 * rotation_angle),
            0.5 * glm::sin(0.33 * rotation_angle),
            0.0
        ));

        // Progress world
        world.progress();
    }
}


DemoApplication::DemoApplication() {
    world.import<lixy::CoreModule>();
    world.import<lixy::WindowingModule>();
    world.import<lixy::RendererModule>();

    lixy::Window *window = lixy::Window::get_singleton(world);
    window->set_title("Demo OpenGL Application");

    // Create mesh
    lixy::EntityRef shaderball_mesh = lixy::ObjMesh::load(world, "assets/models/shaderball.obj");

    // Create rectangle entity
    shaderball = world.entity()
        .emplace<lixy::Transform>(glm::vec3(0.0, -1.0, -2.5))
        .set<lixy::MeshInstance>({shaderball_mesh})
        .add<lixy::Visible>();
    
    // Create camera entity
    camera = lixy::Camera::create(world, {.type = lixy::Camera::Type::PERSPECTIVE});

    lixy::Renderer *renderer = lixy::Renderer::get_singleton(world);
    renderer->set_current_camera(camera);

    // Add lights
    light1 = world.entity()
        .set<lixy::Transform>({})
        .set<lixy::PointLight>({ .color = glm::vec3(1.0, 0.5, 0.0), .radius = 10.0 })
        .add<lixy::Visible>();
    
    light2 = world.entity()
        .set<lixy::Transform>({})
        .set<lixy::PointLight>({ .color = glm::vec3(0.0, 0.5, 1.0), .radius = 10.0 })
        .add<lixy::Visible>();
}


DemoApplication::~DemoApplication() {
    shaderball.destruct();
    camera.destruct();
    world.release();
}
