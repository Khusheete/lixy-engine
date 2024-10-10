#include "application.hpp"

#include "core/src/module.hpp"
#include "core/src/ref.hpp"
#include "debug/debug.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/module.hpp"
#include "renderer/src/renderer.hpp"
#include "thirdparty/glm/fwd.hpp"

#include <GL/gl.h>
#include <array>
#include <cstdlib>
#include <memory>


void DemoApplication::main_loop() {
    while (!lixy::Renderer::get_singleton(world)->window_should_close()) {
        world.progress();
    }
}


DemoApplication::DemoApplication() {
    world.import<lixy::CoreModule>();
    world.import<lixy::RendererModule>();

    lixy::Renderer::get_singleton(world)->window_set_title("Demo OpenGL Application");

    // Create material
    lixy::EntityRef material = lixy::Material::load(world, "assets/shaders/shader.vert", "assets/shaders/shader.frag");
    lixy::Material *material_component = material.get_mut<lixy::Material>();
    ASSERT_FATAL_ERROR(
        material_component->is_valid(),
        "Failed to load shaders" << material_component->get_errors()
    );
    material_component->set_uniform("u_color", glm::vec3(1.0, 0.0, 1.0));


    // Create mesh
    std::vector<lixy::Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
    };

    std::vector<uint32_t> indices {
        0, 1, 2, 2, 3, 1
    };

    lixy::EntityRef mesh = lixy::ArrayMesh::create(world);
    mesh.get_mut<lixy::ArrayMesh>()->add_surface(vertices, indices, material);

    // Rectangle
    rectangle = world.entity()
        .set<lixy::ArrayMeshInstance>({mesh})
        .add<lixy::Visible>();
}


DemoApplication::~DemoApplication() {
    rectangle.destruct();
}