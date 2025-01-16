#pragma once


#include "thirdparty/glm/glm.hpp"
#include <filesystem>
#include <istream>
#include <map>
#include <string>
#include <vector>


namespace lixy {

    struct WavefrontObject;


    struct WavefrontMaterial {
        enum class IlluminationModel {
            COLOR_ON_AMBIENT_OFF             = 0,
            COLOR_ON_AMBIENT_ON              = 1,
            HIGHLIGH_ON                      = 2,
            RT_REFLECTION                    = 3,
            RT_TRANSPARENCY_REFLECTION       = 4,
            RT_FRESNEL_REFLECTION            = 5,
            RT_REFRACTION_REFLECTION         = 6,
            RT_REFRACTION_FRESNEL_REFLECTION = 7,
            REFLECTION                       = 8,
            TRANSPARENCY_REFLECTION          = 9,
            CAST_INVIS_SHADOW                = 10,
        };

        struct TextureMap {
            std::filesystem::path path = "";
            glm::vec3 origin_offset = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
        };

        IlluminationModel illumination_model;

        glm::vec3 ambient_color;
        glm::vec3 diffuse_color;
        glm::vec3 specular_color;
        float specular_exponent;

        float dissolve;
        glm::vec3 transmission_filter_color;
        float index_of_refraction;

        TextureMap ambient_texture;
        TextureMap diffuse_texture;
        TextureMap specular_texture;
        TextureMap specular_exponent_texture;


        bool has_ambient_texture() const;
        bool has_diffuse_texture() const;
        bool has_specular_texture() const;
        bool has_specular_exponent_texture() const;
    };


    struct WavefrontObject {
        std::string material;
        std::vector<uint32_t> position_indices;
        std::vector<uint32_t> uv_indices;
        std::vector<uint32_t> normal_indices;
    };


    struct WavefrontMesh {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::map<std::string, WavefrontObject> objects;
        std::map<std::string, WavefrontMaterial> materials;

        static WavefrontMesh load(const std::filesystem::path &p_path);
    
    private:
        void _include_materials(const std::filesystem::path &p_path);
    };

}