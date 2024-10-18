#include "transform.hpp"

#include "thirdparty/flecs/flecs.h"
#include "thirdparty/glm/glm.hpp"


namespace lixy {

    void Transform::set_position(const glm::vec3 &p_position) {
        position = p_position;
        dirty_matrix = true;
    }


    void Transform::translate(const glm::vec3 &p_vector) {
        position += p_vector;
        dirty_matrix = true;
    }
    

    glm::mat4 Transform::get_matrix() {
        if (dirty_matrix) {
            _calculate_matrix();
        }
        return matrix;
    }


    void Transform::_calculate_matrix() {
        matrix = glm::translate(
            glm::scale(
                glm::mat4(rotation),
                scale
            ),
            position
        );
        dirty_matrix = false;
    }


    Transform::Transform(const glm::vec3 &p_position, const glm::quat &p_rotation, const glm::vec3 &p_scale)
        : position(p_position),
        rotation(p_rotation),
        scale(p_scale)
    {
        _calculate_matrix();
    }
}