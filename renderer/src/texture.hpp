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


#pragma once

#include "core/src/ref.hpp"

#include "renderer/src/primitives/texture.hpp"
#include "thirdparty/flecs/flecs.h"

#include <filesystem>
#include <memory>
#include <string>


namespace lixy {

    class Texture {
    public:
        void bind(int p_location = 0) const;
        void unbind() const;

        bool is_valid() const;
        uint32_t get_texture_id() const;

        static EntityRef load_texture2d(flecs::world &p_world, const std::filesystem::path &p_path);
        static EntityRef create_texture2d(flecs::world &p_world, int p_width, int p_height, opengl::TextureFormat p_format);

        Texture() = default;
        Texture(Texture&&) = default;
        Texture &operator=(Texture&&) = default;
        virtual ~Texture() = default;

        opengl::BaseTexture *get_internal_texture();
        const opengl::BaseTexture *get_internal_texture() const;
    
    private:
        std::unique_ptr<opengl::BaseTexture> internal_texture;
    };
}