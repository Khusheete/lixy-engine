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


#include "material.hpp"

#include "core/src/ref.hpp"
#include "debug/debug.hpp"

#include "renderer/src/primitives/shader.hpp"
#include "renderer/src/texture.hpp"

#include <filesystem>
#include <fstream>
#include <memory>


namespace lixy {

    const std::string Material::MODEL_UNIFORM = "u_model";
    const std::string Material::VIEW_UNIFORM = "u_view";
    const std::string Material::PROJECTION_UNIFORM = "u_projection";


    std::string _read_file(const std::filesystem::path &file_path) {
        std::fstream fi(file_path);
        std::stringstream string_buffer;
        string_buffer << fi.rdbuf();
        fi.close();

        return string_buffer.str();
    }


    void Material::bind_material() const {
        if (program->is_bound()) return; // This function can be ignored because shader programs are currently unique
        program->bind();

        for (const auto &[name, uniform] : uniforms) {
            switch (uniform.type) {
            case opengl::ShaderDataType::Float: program->bind_uniform(name, *reinterpret_cast<const float*>(uniform.data)); break;
            case opengl::ShaderDataType::Vec2 :  program->bind_uniform(name, *reinterpret_cast<const glm::vec2*>(uniform.data)); break;
            case opengl::ShaderDataType::Vec3 :  program->bind_uniform(name, *reinterpret_cast<const glm::vec3*>(uniform.data)); break;
            case opengl::ShaderDataType::Vec4 :  program->bind_uniform(name, *reinterpret_cast<const glm::vec4*>(uniform.data)); break;
            case opengl::ShaderDataType::Mat2 :  program->bind_uniform(name, *reinterpret_cast<const glm::mat2*>(uniform.data)); break;
            case opengl::ShaderDataType::Mat3 :  program->bind_uniform(name, *reinterpret_cast<const glm::mat3*>(uniform.data)); break;
            case opengl::ShaderDataType::Mat4 :  program->bind_uniform(name, *reinterpret_cast<const glm::mat4*>(uniform.data)); break;
            case opengl::ShaderDataType::Int  :   program->bind_uniform(name, *reinterpret_cast<const int*>(uniform.data)); break;
            case opengl::ShaderDataType::IVec2: program->bind_uniform(name, *reinterpret_cast<const glm::ivec2*>(uniform.data)); break;
            case opengl::ShaderDataType::IVec3: program->bind_uniform(name, *reinterpret_cast<const glm::ivec3*>(uniform.data)); break;
            case opengl::ShaderDataType::IVec4: program->bind_uniform(name, *reinterpret_cast<const glm::ivec4*>(uniform.data)); break;
            default:
                LOG_WARNING("Unimplemented uniform type");
            }
        }

        int texture_location = 0;
        for (const auto &[name, uniform] : resource_uniform) {
            switch (uniform.type) {
            case opengl::ShaderDataType::Sampler2D:
            {
                if (!(uniform.resource.is_alive() && uniform.resource.has<Texture>())) continue;
                uniform.resource.get<Texture>()->bind(texture_location);
                program->bind_uniform(name, texture_location);
                texture_location += 1;
                break;
            }
            default:
                LOG_WARNING("Unimplemented uniform type");
            }
        }
    }


    void Material::bind_pvm(const glm::mat4 &p_projection, const glm::mat4 &p_view, const glm::mat4 &p_model) const {
        if (!program->is_bound()) program->bind();

        program->bind_uniform(Material::PROJECTION_UNIFORM, p_projection);
        program->bind_uniform(Material::VIEW_UNIFORM, p_view);
        program->bind_uniform(Material::MODEL_UNIFORM, p_model);
    }


    template<>
    void Material::set_uniform<EntityRef>(const std::string &p_uniform_name, const EntityRef &p_value) {
        try {
            resource_uniform.at(p_uniform_name).resource = p_value;
        } catch (std::out_of_range) {}
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


    EntityRef Material::load(flecs::world &p_world, const std::filesystem::path &p_vertex_path, const std::filesystem::path &p_fragment_path) {
        return Material::create_from_source(p_world, _read_file(p_vertex_path), _read_file(p_fragment_path));
    }


    Material::Material(const std::string &p_vertex_source, const std::string &p_fragment_source) {
        program = std::make_shared<opengl::ShaderProgram>(p_vertex_source, p_fragment_source);

        // Get uniforms
        uniforms.reserve(program->get_uniform_count());
        for (int i = 0; i < program->get_uniform_count(); i++) {
            opengl::ShaderDataType type = program->get_uniform_type(i);
            std::string name = program->get_uniform_name(i);

            if (name == Material::MODEL_UNIFORM || name == Material::VIEW_UNIFORM || name == Material::PROJECTION_UNIFORM) continue;

            switch (type) {
            case opengl::ShaderDataType::Float:
            case opengl::ShaderDataType::Vec2:
            case opengl::ShaderDataType::Vec3:
            case opengl::ShaderDataType::Vec4:
            case opengl::ShaderDataType::Mat2:
            case opengl::ShaderDataType::Mat3:
            case opengl::ShaderDataType::Mat4:
            case opengl::ShaderDataType::Int:
            case opengl::ShaderDataType::IVec2:
            case opengl::ShaderDataType::IVec3:
            case opengl::ShaderDataType::IVec4:
            case opengl::ShaderDataType::Bool:
                uniforms[name] = Uniform{
                    .type = type
                };
                break;
            case opengl::ShaderDataType::Sampler2D:
                resource_uniform[name] = ResourceUniform{
                    .type = type
                };
                break;
            case opengl::ShaderDataType::Unknown:
                LOG_WARNING("Unknown uniform type named: `" << name << "` in shader");
            }
            
        }
    }
}