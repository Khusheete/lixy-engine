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

#include "debug/debug.hpp"

#include "thirdparty/stb/stb_image.h"

#include <GL/glew.h>


namespace lixy::opengl {


    void Texture2D::bind(uint32_t p_location) const {
        ASSERT_FATAL_ERROR(p_location < 32, "There is a maximum of 32 opengl texture slots");
        glActiveTexture(GL_TEXTURE0 + p_location);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }


    void Texture2D::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    bool Texture2D::is_valid() const {
        return valid;
    }


    Texture2D Texture2D::load(const std::string &p_path) {
        int width, height, channel_count;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *image_data = stbi_load(p_path.c_str(), &width, &height, &channel_count, 4);
        
        if (!image_data) {
            LOG_ERROR("Could not laod image at " << p_path);
            return Texture2D(); // Return invalid image
        }

        TextureFormat format;
        switch (channel_count) {
        case 1:
            format = TextureFormat::R8;
            break;
        case 2:
            format = TextureFormat::RG8;
            break;
        case 3:
            format = TextureFormat::RGB8;
            break;
        case 4:
            format = TextureFormat::RGBA8;
            break;
        default:
            ASSERT_FATAL_ERROR(false, "Invalid channel count (this should never happen)")
        }

        Texture2D texture(image_data, width, height, format);
        stbi_image_free(image_data);
        return texture;
    }


    Texture2D::Texture2D(const void *p_data, int p_width, int p_height, TextureFormat p_format) {
        glGenTextures(1, &texture_id);

        uint32_t internal_format, format;
        switch (p_format) {
        case TextureFormat::R8:
            internal_format = GL_R8;
            format = GL_RED;
            break;
        case TextureFormat::RG8:
            internal_format = GL_RG8;
            format = GL_RG;
            break;
        case TextureFormat::RGB8: // TODO: check conversion
            internal_format = GL_RGB8;
            format = GL_RGB;
            break;
        case TextureFormat::RGBA8:
            internal_format = GL_RGBA8;
            format = GL_RGBA;
            break;
        case TextureFormat::R16:
            internal_format = GL_R16;
            format = GL_RED;
            break;
        case TextureFormat::RG16:
            internal_format = GL_RG16;
            format = GL_RG;
            break;
        case TextureFormat::RGB16: // TODO: check conversion
            internal_format = GL_RGB16;
            format = GL_RGB;
            break;
        case TextureFormat::RGBA16:
            internal_format = GL_RGBA16;
            format = GL_RGBA;
            break;
        }

        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, p_width, p_height, 0, format, GL_UNSIGNED_BYTE, p_data);

        unbind();
        valid = true;
    }


    Texture2D::Texture2D(Texture2D &&p_other)
        : texture_id(p_other.texture_id),
        valid(p_other.valid)
    {
        p_other.texture_id = 0;
        p_other.valid = false;
    }
    
    
    Texture2D &Texture2D::operator=(Texture2D &&p_other) {
        if (valid) {
            glDeleteTextures(1, &texture_id);
        }

        texture_id = p_other.texture_id;
        valid = p_other.valid;

        p_other.texture_id = 0;
        p_other.valid = 0;

        return *this;
    }
    
    
    Texture2D::~Texture2D() {
        if (valid) glDeleteTextures(1, &texture_id);
    }

}