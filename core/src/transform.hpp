#pragma once


#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/ext/quaternion_float.hpp"
#include "thirdparty/glm/ext/vector_float3.hpp"
#include "thirdparty/glm/gtc/quaternion.hpp"
#include "thirdparty/glm/glm.hpp"


namespace lixy {

    class Transform {
    public:
        void set_position(const glm::vec3 &p_position);
        void translate(const glm::vec3 &p_vector);

        glm::mat4 get_matrix();

        Transform() = default;
        Transform(const glm::vec3 &p_position, const glm::quat &p_rotation, const glm::vec3 &p_scale);
        Transform(const Transform&) = default;
        Transform &operator=(const Transform&) = default;

    private:
        void _calculate_matrix();
    
    private:
        bool dirty_matrix = false;

        glm::vec3 position = glm::vec3(0.0);
        glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
        glm::vec3 scale = glm::vec3(1.0);

        glm::mat4 matrix = glm::mat4(1.0);
    };
}