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