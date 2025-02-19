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


#pragma once


#include "core/src/ref.hpp"
#include "primitives/context.hpp"

#include "renderer/src/framebuffer.hpp"
#include "renderer/src/material.hpp"
#include "renderer/src/primitives/framebuffer.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/glm.hpp"

#include <memory>


namespace lixy {

    struct RendererModule;

    
    struct MeshInstance {
        EntityRef mesh;
    };


    struct Visible {};


    class Renderer {
    public:
        void set_current_camera(flecs::entity p_camera);
        flecs::entity get_current_camera() const;

        EntityRef create_default_material(flecs::world &p_world);

        static Renderer *get_singleton(flecs::world &p_world);

        Renderer() = default;
        
    private:
        void _initialize(flecs::world &p_world, flecs::entity &p_self);
        friend RendererModule;
    
    private:
        opengl::OpenGLContext context;
        EntityRef gbuffer_ref;
        
        flecs::entity current_camera = flecs::entity::null();
        glm::mat4 projection_matrix = glm::mat4(1.0);
        glm::mat4 view_matrix = glm::mat4(1.0);

        Material default_material;
        
        Material screen_material;
        std::shared_ptr<opengl::VertexBuffer> quad_vertices;
        std::shared_ptr<opengl::IndexBuffer> quad_indices;
        std::shared_ptr<opengl::VertexArrayBuffer> quad_vao;

        opengl::ShaderStorageBuffer point_lights_storage;
        opengl::ShaderStorageBuffer point_lights_transform_storage;
    };
}