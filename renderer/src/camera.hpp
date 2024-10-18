#pragma once

#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/glm/gtc/constants.hpp"


namespace lixy {


    struct Camera {
    public:
        enum class Type {
            ORTHOGRAPHIC, PERSPECTIVE
        };

    public:
        float near = 0.1;
        float far = 1000.0;
        float fov = glm::half_pi<float>();
        float focal_length = 1.0;
        Type type = Type::PERSPECTIVE;

        static flecs::entity create(flecs::world &p_world);
        static flecs::entity create(flecs::world &p_world, const Camera &p_camera);
    };
}