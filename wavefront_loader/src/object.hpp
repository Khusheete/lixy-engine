#pragma once


#include "thirdparty/glm/fwd.hpp"
#include <filesystem>
#include <map>
#include <string>
#include <vector>


namespace lixy {

    struct WavefrontMaterial {
        // TODO: implement
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