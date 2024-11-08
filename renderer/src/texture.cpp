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