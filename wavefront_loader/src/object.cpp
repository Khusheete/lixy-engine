#include "object.hpp"
#include "debug/debug.hpp"

#include "thirdparty/glm/glm.hpp"

#include <fstream>
#include <string>


namespace lixy {
    template<class e, class t>
    std::basic_istream<e,t>& operator>>(std::basic_istream<e,t>& in, const e& cliteral) { // FIXME: don't use that
            e buffer(0);
            in >> buffer;
            if (buffer != cliteral)
                in.setstate(std::ios::failbit); // Fail if the literal is not correct
            return in;
    }


    WavefrontMesh WavefrontMesh::load(const std::filesystem::path &p_path) {
        WavefrontMesh mesh{};
        
        std::ifstream file(p_path);
        WavefrontObject *current_object = nullptr;
        std::string token;

        while (file.good()) {
            file >> token;
            
            if (token == "v") { // Vertex position
                if (!current_object) {
                    LOG_WARNING("Could not load mesh at `" << p_path << "` missing object");
                    return WavefrontMesh{};
                }
                float x, y, z;
                file >> x >> y >> z;
                mesh.positions.push_back(glm::vec3(x, y, z));
            } else if (token == "vn") { // Vertex normal
                if (!current_object) {
                    LOG_WARNING("Could not load mesh at `" << p_path << "` missing object");
                    return WavefrontMesh{};
                }
                float x, y, z;
                file >> x >> y >> z;
                mesh.normals.push_back(glm::vec3(x, y, z));
            } else if (token == "vt") { // Vertex texture
                if (!current_object) {
                    LOG_WARNING("Could not load mesh at `" << p_path << "` missing object");
                    return WavefrontMesh{};
                }
                float u, v;
                file >> u >> v;
                mesh.uvs.push_back(glm::vec2(u, v));
            } else if (token == "f") {
                if (!current_object) {
                    LOG_WARNING("Could not load mesh at `" << p_path << "` missing object");
                    return WavefrontMesh{};
                }
                uint32_t pos0, uv0, norm0, pos1, uv1, norm1, pos2, uv2, norm2;
                file >> pos0 >> '/' >> uv0 >> '/' >> norm0;
                file >> pos1 >> '/' >> uv1 >> '/' >> norm1;
                file >> pos2 >> '/' >> uv2 >> '/' >> norm2;

                current_object->position_indices.push_back(pos0 - 1);
                current_object->position_indices.push_back(pos1 - 1);
                current_object->position_indices.push_back(pos2 - 1);

                current_object->uv_indices.push_back(uv0 - 1);
                current_object->uv_indices.push_back(uv1 - 1);
                current_object->uv_indices.push_back(uv2 - 1);

                current_object->normal_indices.push_back(norm0 - 1);
                current_object->normal_indices.push_back(norm1 - 1);
                current_object->normal_indices.push_back(norm2 - 1);
            } else if (token == "o") { // Change the current object
                // Setup the new object
                file >> token;
                mesh.objects[token] = WavefrontObject{};
                current_object = &mesh.objects[token];
            } else if  (token == "mtllib") {
                file >> token;
                mesh._include_materials(token);
            } else if (token == "usemtl") {
                if (!current_object) {
                    LOG_WARNING("Could not load mesh at `" << p_path << "` missing object");
                    return WavefrontMesh{};
                }
                file >> current_object->material;
            } else if (token == "s" || token == "#") {
                std::getline(file, token); // Ignore tokens
            }
        }

        return mesh;
    }


    void WavefrontMesh::_include_materials(const std::filesystem::path &p_path) {
        // TODO: implement
        LOG_WARNING("Material template library at `" << p_path << "` was not loaded (MISSING IMPLEMENTATION)");
    }
}