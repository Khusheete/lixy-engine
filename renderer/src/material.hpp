#pragma once


#include "core/src/ref.hpp"
#include "debug/debug.hpp"
#include "renderer/src/mesh.hpp"
#include "renderer/src/primitives/shader.hpp"
#include "thirdparty/flecs/flecs.h"

#include <cstring>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <filesystem>


namespace lixy {

    class Material {
    public:
        bool is_valid() const;
        const std::string &get_errors() const;

        void bind_material() const;
        void bind_pvm(const glm::mat4 &p_projection, const glm::mat4 &p_view, const glm::mat4 &p_model) const;

        template<class T>
        void set_uniform(const std::string &p_uniform_name, const T &p_value) {
            static_assert(sizeof(T) <= 16 * sizeof(uint32_t), "Incorrect uniform size");
            try {
                memcpy(uniforms.at(p_uniform_name).data, &p_value, sizeof(p_value));
            } catch (std::out_of_range) {}
        }

        static EntityRef load(flecs::world &p_world, const std::filesystem::path &p_vertex_path, const std::filesystem::path &p_fragment_path);
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

        struct ResourceUniform {
            opengl::ShaderDataType type;
            EntityRef resource;
        };

    private:
        std::unique_ptr<opengl::ShaderProgram> program;
        std::unordered_map<std::string, Uniform> uniforms;
        std::unordered_map<std::string, ResourceUniform> resource_uniform; // Resources need to be implemented apart from other uniforms as they require cleanup
    
    private:
        static const std::string MODEL_UNIFORM;
        static const std::string VIEW_UNIFORM;
        static const std::string PROJECTION_UNIFORM;
    };


    template<>
    void Material::set_uniform(const std::string &p_uniform_name, const EntityRef &p_value);
}