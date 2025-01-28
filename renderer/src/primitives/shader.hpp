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

#include "debug/debug.hpp"
#include "renderer/src/primitives/vbuffer.hpp"
#include "thirdparty/glm/glm.hpp"

#include "thirdparty/glad/include/glad/glad.h"

#include <cstdint>
#include <string>
#include <vector>


namespace lixy::opengl {

    enum class ShaderDataType {
        Unknown = 0, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Int, IVec2, IVec3, IVec4, Bool,
        Sampler2D
    };
    

    static ShaderDataType shader_data_type_from_gl_type(uint32_t gl_type) {
        switch (gl_type) {
        case GL_FLOAT:
            return ShaderDataType::Float;
        case GL_FLOAT_VEC2:
            return ShaderDataType::Vec2;
        case GL_FLOAT_VEC3:
            return ShaderDataType::Vec3;
        case GL_FLOAT_MAT2:
            return ShaderDataType::Mat2;
        case GL_FLOAT_MAT3:
            return ShaderDataType::Mat3;
        case GL_FLOAT_MAT4:
            return ShaderDataType::Mat4;
        case GL_INT:
            return ShaderDataType::Int;
        case GL_INT_VEC2:
            return ShaderDataType::IVec2;
        case GL_INT_VEC3:
            return ShaderDataType::IVec3;
        case GL_INT_VEC4:
            return ShaderDataType::IVec4;
        case GL_BOOL:
            return ShaderDataType::Bool;
        case GL_SAMPLER_2D:
            return ShaderDataType::Sampler2D;
        default:
            return ShaderDataType::Unknown;
        }
    }

    
    static uint32_t shader_data_type_get_size(ShaderDataType p_type) {
        switch (p_type) {
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Vec2: return 2 * 4;
        case ShaderDataType::Vec3: return 3 * 4;
        case ShaderDataType::Vec4: return 4 * 4;
        case ShaderDataType::Mat2: return 2 * 2 * 4;
        case ShaderDataType::Mat3: return 3 * 3 * 4;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::Sampler2D:
        case ShaderDataType::Int: return 4;
        case ShaderDataType::IVec2: return 2 * 4;
        case ShaderDataType::IVec3: return 3 * 4;
        case ShaderDataType::IVec4: return 4 * 4;
        case ShaderDataType::Bool: return 1;
        default:
            ASSERT_FATAL_ERROR(false, "Unknown ShaderDataType");
            return 0;
        }
    }
    
    
    static uint32_t shader_data_type_get_gl_type(ShaderDataType p_type) {
        switch (p_type) {
        case ShaderDataType::Float:
        case ShaderDataType::Vec2:
        case ShaderDataType::Vec3:
        case ShaderDataType::Vec4:
        case ShaderDataType::Mat2:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::IVec2:
        case ShaderDataType::IVec3:
        case ShaderDataType::IVec4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::Sampler2D:
            return GL_SAMPLER_2D;
        default:
            ASSERT_FATAL_ERROR(false, "Unknown ShaderDataType");
            return 0;
        }
    }
    
    
    static uint32_t shader_data_type_get_component_count(ShaderDataType p_type) {
        switch (p_type) {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
        case ShaderDataType::Sampler2D:
        case ShaderDataType::Bool:
            return 1;
        case ShaderDataType::Vec2:
        case ShaderDataType::IVec2:
            return 2;
        case ShaderDataType::Vec3:
        case ShaderDataType::IVec3:
            return 3;
        case ShaderDataType::Vec4:
        case ShaderDataType::Mat2:
        case ShaderDataType::IVec4:
            return 4;
        case ShaderDataType::Mat3:
            return 9;
        case ShaderDataType::Mat4:
            return 16;
        default:
            ASSERT_FATAL_ERROR(false, "Unknown ShaderDataType");
            return 0;
        }
    }
    
    
    class ShaderProgram {
    public:
        void bind() const;
        bool is_bound() const;
        void bind_uniform(const std::string &p_uniform_name, const float &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::vec2 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::vec3 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::vec4 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const int &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::ivec2 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::ivec3 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::ivec4 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::mat2 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::mat3 &p_value);
        void bind_uniform(const std::string &p_uniform_name, const glm::mat4 &p_value);
        void bind_storage_buffer(const std::string &p_storage_buffer_name, const ShaderStorageBuffer::Slice &p_slice);
        void unbind() const;

        int get_uniform_count();
        int get_uniform_index(const std::string &p_uniform_name);
        const std::string &get_uniform_name(int p_index);
        ShaderDataType get_uniform_type(int p_index);

        int get_storage_buffer_count();
        int get_storage_buffer_index(const std::string &p_ssb);
        const std::string &get_storage_buffer_name(int p_index);

        bool is_valid() const;
        const std::string &get_errors() const;
    
        // static ShaderProgram load_shader_program(const std::string &p_vertex_path, const std::string &p_fragment_path);
    
        ShaderProgram(const std::string &p_vertex_source, const std::string &p_fragment_source);
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram &&p_other);
        ShaderProgram &operator=(ShaderProgram &&p_other);
        virtual ~ShaderProgram();
    
    private:
        struct Uniform {
            ShaderDataType type;
            std::string name;
        };

        struct StorageBuffer {
            std::string name;
        };

    private:
        int32_t _get_uniform_location(const std::string &p_uniform_name);
        int32_t _get_storage_buffer_location(const std::string &p_storage_buffer_name);

    private:
        uint32_t program_id;

        std::vector<Uniform> uniforms;
        std::vector<StorageBuffer> storage_buffers;
    
        bool creation_error = false;
        std::string errors;
    };
}