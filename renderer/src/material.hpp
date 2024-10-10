#pragma once


#include "core/src/ref.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "thirdparty/flecs/flecs.h"
#include <memory>


namespace lixy {

    class Material {
    public:
        bool is_valid() const;
        const std::string &get_errors() const;


        inline const ShaderProgram &get_shader_program() const {
            return *program;
        }

        static EntityRef load(flecs::world &p_world, const std::string &p_vertex_path, const std::string &p_fragment_path);
        static EntityRef create_from_source(flecs::world &p_world, const std::string &p_vertex_source, const std::string &p_fragment_source);

        Material() = default;
        Material(const std::string &p_vertex_source, const std::string &p_fragment_source);
        Material(const Material&) = delete;
        Material(Material&&) = default;
        Material &operator=(Material&&) = default;
        virtual ~Material() = default;
    
    private:
        std::unique_ptr<ShaderProgram> program;
    };
}