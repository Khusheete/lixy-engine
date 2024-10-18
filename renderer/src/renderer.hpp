#pragma once


#include "core/src/ref.hpp"
#include "primitives/context.hpp"

#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/glm.hpp"


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

        void set_current_camera(flecs::entity p_camera);
        flecs::entity get_current_camera() const;

        static Renderer *get_singleton(flecs::world &p_world);

        Renderer() = default;
        
    private:
        void _initialize(flecs::world &p_world);
        friend RendererModule;
    
    private:
        opengl::OpenGLContext context;
        
        flecs::entity current_camera = flecs::entity::null();
        glm::mat4 projection_matrix = glm::mat4(1.0);
        glm::mat4 view_matrix = glm::mat4(1.0);
    };
}