#include "module.hpp"

#include "core/src/module.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/renderer.hpp"

#include "renderer/src/texture.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    RendererModule::RendererModule(flecs::world &p_world) {
        p_world.module<RendererModule>();
        p_world.import<CoreModule>();

        // Add components
        p_world.add<ArrayMesh>();
        p_world.add<Material>();
        p_world.add<Texture>();
        p_world.add<ArrayMeshInstance>();
        p_world.add<Visible>();

        // Initialize renderer singleton
        p_world.add<Renderer>();
        p_world.set<Renderer>(Renderer());
        p_world.get_mut<Renderer>()->_initialize(p_world);
    }
}