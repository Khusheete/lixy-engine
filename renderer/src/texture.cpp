#include "texture.hpp"
#include "core/src/ref.hpp"
#include "renderer/src/primitives/texture.hpp"
#include "thirdparty/flecs/flecs.h"
#include <string>


namespace lixy {

    void Texture::bind(int p_location) const {
        internal_texture->bind(p_location);
    }
    
    
    void Texture::unbind() const {
        internal_texture->unbind();
    }


    bool Texture::is_valid() const {
        return internal_texture->is_valid();
    }
    
    
    EntityRef Texture::load_texture2d(flecs::world &p_world, const std::string &p_path) {
        EntityRef texture = EntityRef::create_reference(p_world).add<Texture>();
        texture.get_mut<Texture>()->internal_texture = std::make_unique<opengl::Texture2D>(opengl::Texture2D::load(p_path));
        return texture;
    }
}