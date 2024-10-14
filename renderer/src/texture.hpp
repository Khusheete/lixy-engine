#pragma once

#include "core/src/ref.hpp"

#include "renderer/src/primitives/texture.hpp"
#include "thirdparty/flecs/flecs.h"

#include <memory>
#include <string>


namespace lixy {

    class Texture {
    public:
        void bind(int p_location = 0) const;
        void unbind() const;

        bool is_valid() const;

        static EntityRef load_texture2d(flecs::world &p_world, const std::string &p_path);

        Texture() = default;
        Texture(Texture&&) = default;
        Texture &operator=(Texture&&) = default;
        virtual ~Texture() = default;
    
    private:
        std::unique_ptr<opengl::BaseTexture> internal_texture;
    };
}