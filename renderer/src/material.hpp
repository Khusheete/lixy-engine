#pragma once


#include "core/src/ref.hpp"
#include "debug/debug.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "thirdparty/flecs/flecs.h"

#include <cstring>
#include <memory>
#include <stdexcept>
#include <unordered_map>


namespace lixy {

    class Material {
    public:
        bool is_valid() const;
        const std::string &get_errors() const;

        void bind_material() const;

        template<class T>
        void set_uniform(const std::string &p_uniform_name, const T &p_value) {
            static_assert(sizeof(T) <= 16 * sizeof(uint32_t), "Incorrect uniform size");
            try {
                memcpy(uniforms.at(p_uniform_name).data, &p_value, sizeof(p_value));
            } catch (std::out_of_range) {}
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
        struct Uniform {
            opengl::ShaderDataType type;
            uint32_t data[16];
        };

    private:
        std::unique_ptr<opengl::ShaderProgram> program;
        std::unordered_map<std::string, Uniform> uniforms;
    };
}