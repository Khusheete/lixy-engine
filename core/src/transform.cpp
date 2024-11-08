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