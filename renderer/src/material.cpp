#include "material.hpp"
#include "core/src/ref.hpp"
#include "debug/debug.hpp"
#include "renderer/src/primitives/shader.hpp"
#include <fstream>
#include <memory>


namespace lixy {

    std::string _read_file(const std::string &file_path) {
        std::fstream fi(file_path);
        std::stringstream string_buffer;
        string_buffer << fi.rdbuf();
        fi.close();

        return string_buffer.str();
    }


    void Material::bind_material() const {
        program->bind();

        for (const auto &[name, uniform] : uniforms) {
            switch (uniform.type) {
            case opengl::ShaderDataType::Float: program->bind_uniform(name, *reinterpret_cast<const float*>(uniform.data));
            case opengl::ShaderDataType::Vec2:  program->bind_uniform(name, *reinterpret_cast<const glm::vec2*>(uniform.data));
            case opengl::ShaderDataType::Vec3:  program->bind_uniform(name, *reinterpret_cast<const glm::vec3*>(uniform.data));
            case opengl::ShaderDataType::Vec4:  program->bind_uniform(name, *reinterpret_cast<const glm::vec4*>(uniform.data));
            case opengl::ShaderDataType::Mat2:  program->bind_uniform(name, *reinterpret_cast<const glm::mat2*>(uniform.data));
            case opengl::ShaderDataType::Mat3:  program->bind_uniform(name, *reinterpret_cast<const glm::mat3*>(uniform.data));
            case opengl::ShaderDataType::Mat4:  program->bind_uniform(name, *reinterpret_cast<const glm::mat4*>(uniform.data));
            case opengl::ShaderDataType::Int:   program->bind_uniform(name, *reinterpret_cast<const int*>(uniform.data));
            case opengl::ShaderDataType::IVec2: program->bind_uniform(name, *reinterpret_cast<const glm::ivec2*>(uniform.data));
            case opengl::ShaderDataType::IVec3: program->bind_uniform(name, *reinterpret_cast<const glm::ivec3*>(uniform.data));
            case opengl::ShaderDataType::IVec4: program->bind_uniform(name, *reinterpret_cast<const glm::ivec4*>(uniform.data));
            default:
                LOG_WARNING("Unimplemented uniform type");
            }
        }
    }


    bool Material::is_valid() const {
        return program->is_valid();
    }
    
    
    const std::string &Material::get_errors() const {
        return program->get_errors();
    }


    EntityRef Material::create_from_source(flecs::world &p_world, const std::string &p_vertex_source, const std::string &p_fragment_source) {
        return EntityRef::create_reference(p_world).emplace<Material>(p_vertex_source, p_fragment_source);
    }


    EntityRef Material::load(flecs::world &p_world, const std::string &p_vertex_path, const std::string &p_fragment_path) {
        return Material::create_from_source(p_world, _read_file(p_vertex_path), _read_file(p_fragment_path));
    }


    Material::Material(const std::string &p_vertex_source, const std::string &p_fragment_source) {
        program = std::make_unique<ShaderProgram>(p_vertex_source, p_fragment_source);

        // Get uniforms
        uniforms.reserve(program->get_uniform_count());
        for (int i = 0; i < program->get_uniform_count(); i++) {
            uniforms[program->get_uniform_name(i)] = Uniform{
                .type = program->get_uniform_type(i)
            };
        }
    }
}