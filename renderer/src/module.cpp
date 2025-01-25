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


#include "module.hpp"

#include "core/src/module.hpp"
#include "renderer/src/framebuffer.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/renderer.hpp"

#include "renderer/src/texture.hpp"
#include "thirdparty/flecs/flecs.h"
#include "windowing/src/module.hpp"


namespace lixy {

    RendererModule::RendererModule(flecs::world &p_world) {
        p_world.module<RendererModule>();
        p_world.import<CoreModule>();
        p_world.import<WindowingModule>();

        // Add components
        p_world.add<ArrayMesh>();
        p_world.add<ObjMesh>();
        p_world.add<Material>();
        p_world.add<Texture>();
        p_world.add<MeshInstance>();
        p_world.add<Visible>();
        p_world.add<Framebuffer>();

        // Initialize renderer singleton
        flecs::entity rd = p_world.entity<Renderer>();
        p_world.set<Renderer>(Renderer());
        p_world.get_mut<Renderer>()->_initialize(p_world, rd);
    }
}