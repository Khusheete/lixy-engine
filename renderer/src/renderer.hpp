#pragma once


#include "core/src/ref.hpp"
#include "primitives/context.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    struct RendererModule;

    
    struct ArrayMeshInstance {
        EntityRef array_mesh;
    };


    struct Visible {};


    class Renderer {
    public:
        void window_set_title(const std::string &p_title);
        bool window_should_close();

        static Renderer *get_singleton(flecs::world &p_world);

        Renderer() = default;
        
    private:
        void _initialize(flecs::world &p_world);
        friend RendererModule;
    
    private:
        OpenGLContext context;
    };
}