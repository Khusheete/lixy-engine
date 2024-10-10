#include "material.hpp"
#include "core/src/ref.hpp"
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
    }
}