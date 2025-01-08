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


#include <cstdint>
#include <string>


namespace lixy::opengl {

    enum class TextureFormat {
        R8, RG8, RGB8, RGBA8,
        R16, RG16, RGB16, RGBA16
    };

    
    class BaseTexture {
    public:
        virtual void bind(uint32_t p_location = 0) const = 0;
        virtual void unbind() const = 0;
        virtual bool is_valid() const = 0;
    };


    class Texture2D : public BaseTexture {
    public:
        virtual void bind(uint32_t p_location = 0) const override;
        virtual void unbind() const override;

        bool is_valid() const override;

        static Texture2D load(const std::string &p_path);

        Texture2D() = default;
        Texture2D(int p_width, int p_height, TextureFormat p_format);
        Texture2D(const void *p_data, int p_width, int p_height, TextureFormat p_format);
        Texture2D(Texture2D &&p_other);
        Texture2D &operator=(Texture2D &&p_other);
        virtual ~Texture2D();

    private:
        void get_opengl_format(TextureFormat p_tex_format, uint32_t *p_internal_format, uint32_t *p_format);

    private:
        uint32_t texture_id;
        bool valid = false;
    };
}