#pragma once

#include "debug/debug.hpp"
#include <cstdint>
#include <GL/glew.h>
#include <string>


enum class ShaderDataType {
    None = 0, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Int, IVec2, IVec3, IVec4, Bool
};


static uint32_t shader_data_type_get_size(ShaderDataType p_type) {
    switch (p_type) {
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Vec2: return 2 * 4;
    case ShaderDataType::Vec3: return 3 * 4;
    case ShaderDataType::Vec4: return 4 * 4;
    case ShaderDataType::Mat2: return 2 * 2 * 4;
    case ShaderDataType::Mat3: return 3 * 3 * 4;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
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
    default:
        ASSERT_FATAL_ERROR(false, "Unknown ShaderDataType");
        return 0;
    }
}


static uint32_t shader_data_type_get_component_count(ShaderDataType p_type) {
    switch (p_type) {
    case ShaderDataType::Float:
    case ShaderDataType::Int:
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


class Shader {
public:
    void bind();
    void unbind();

    bool is_valid();
    const std::string &get_errors();

    static Shader load_shader_program(const std::string &p_vertex_path, const std::string &p_index_path);

    Shader(const std::string &p_vertex_source, const std::string &p_fragment_source);
    Shader(const Shader&) = delete;
    Shader(Shader &&p_other);
    virtual ~Shader();

private:
    uint32_t program_id;

    bool creation_error = false;
    std::string errors;
};