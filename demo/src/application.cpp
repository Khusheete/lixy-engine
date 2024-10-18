#include "application.hpp"

#include "core/src/module.hpp"
#include "core/src/ref.hpp"
#include "core/src/transform.hpp"
#include "debug/debug.hpp"
#include "renderer/src/camera.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/module.hpp"
#include "renderer/src/renderer.hpp"
#include "renderer/src/texture.hpp"
#include "thirdparty/glm/ext/matrix_transform.hpp"
#include "thirdparty/glm/ext/scalar_constants.hpp"
#include "thirdparty/glm/fwd.hpp"
#include "thirdparty/glm/trigonometric.hpp"

#include <GL/gl.h>
#include <array>
#include <cstdlib>
#include <memory>


void DemoApplication::main_loop() {
    while (!lixy::Renderer::get_singleton(world)->window_should_close()) {
        // Update entities
        rotation_angle += world.delta_time() * 0.2 * glm::pi<float>();

        lixy::Transform *camera_trasnform = camera.get_mut<lixy::Transform>();
        camera_trasnform->set_position(glm::vec3(
            1.0 * glm::cos(rotation_angle),
            1.0 * glm::sin(rotation_angle),
            1.0
        ));

        // Progress world
        world.progress();
    }
}


DemoApplication::DemoApplication() {
    world.import<lixy::CoreModule>();
    world.import<lixy::RendererModule>();

    lixy::Renderer *renderer = lixy::Renderer::get_singleton(world);
    renderer->window_set_title("Demo OpenGL Application");

    // Load image
    lixy::EntityRef texture = lixy::Texture::load_texture2d(world, "assets/textures/test.png");
    ASSERT_FATAL_ERROR(texture.get<lixy::Texture>()->is_valid(), "Image not valid");

    // Create material
    lixy::EntityRef material = lixy::Material::load(world, "assets/shaders/shader.vert", "assets/shaders/shader.frag");
    lixy::Material *material_component = material.get_mut<lixy::Material>();
    ASSERT_FATAL_ERROR(
        material_component->is_valid(),
        "Failed to load shaders" << material_component->get_errors()
    );
    material_component->set_uniform("u_color", glm::vec3(1.0, 0.0, 1.0));
    material_component->set_uniform("u_texture", texture);

    // Create mesh
    std::vector<lixy::Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0, 0.0}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0, 0.0}},
        {{-0.5f,  0.5f, 0.0f}, {0.0, 1.0}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0, 1.0}},
    };

    std::vector<uint32_t> indices {
        0, 1, 2, 2, 3, 1
    };

    lixy::EntityRef mesh = lixy::ArrayMesh::create(world);
    mesh.get_mut<lixy::ArrayMesh>()->add_surface(vertices, indices, material);

    // Create rectangle entity
    rectangle = world.entity()
        .set<lixy::Transform>({})
        .set<lixy::ArrayMeshInstance>({mesh})
        .add<lixy::Visible>();
    
    rectangle.get_mut<lixy::Transform>()->set_position(glm::vec3(0.0, 0.0, -1.0));
    
    // Create camera entity
    camera = lixy::Camera::create(world, {.type = lixy::Camera::Type::PERSPECTIVE});
    renderer->set_current_camera(camera);
}


DemoApplication::~DemoApplication() {
    rectangle.destruct();
    camera.destruct();
    world.release();
}