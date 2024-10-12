#include "renderer.hpp"

#include "renderer/src/mesh.hpp"

#include "thirdparty/flecs/flecs.h"
#include <GLFW/glfw3.h>


namespace lixy {

    void Renderer::window_set_title(const std::string &p_title) {
        context.window_set_title(p_title);
    }
        
        
    bool Renderer::window_should_close() {
        return context.window_should_close();
    }


    Renderer *Renderer::get_singleton(flecs::world &p_world) {
        return p_world.get_mut<Renderer>();
    }


    void Renderer::_initialize(flecs::world &p_world) {
        context.initialize();

        p_world.system("Start Frame")
            .kind(flecs::PreUpdate)
            .run([&](flecs::iter&) {
                context.set_current();
                glfwPollEvents();

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            });

        p_world.system<ArrayMeshInstance, Visible>("Draw ArrayMeshInstances")
            .kind(flecs::PreStore)
            .each([](flecs::iter&, size_t, const ArrayMeshInstance &instance, const Visible&) {
                const ArrayMesh *mesh = instance.array_mesh.get<ArrayMesh>();
                mesh->record_draw();
            });
        
        p_world.system("Swap Buffers")
            .kind(flecs::OnStore)
            .run([&](flecs::iter&) {
                context.swap_buffers();
            });
    }
}