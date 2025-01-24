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
#include "thirdparty/flecs/flecs.h"
#include "primitives/framebuffer.hpp"
#include "primitives/texture.hpp"


namespace lixy {


    class Framebuffer {
    public:
        void bind() const;
        void unbind() const;

        bool is_complete() const;

        inline uint32_t get_width() const { return framebuffer.get_width(); };
        inline uint32_t get_height() const { return framebuffer.get_height(); };
        void set_size(uint32_t p_width, uint32_t p_height);

        inline int get_attachment_count() { return texture_attachments.size(); };
        EntityRef get_attachment(int p_index);

        static EntityRef create(flecs::world &p_world, uint32_t p_width, uint32_t p_height, const std::vector<opengl::TextureFormat> &p_texture_attachment_formats);

        Framebuffer() = default;
        Framebuffer(Framebuffer&&) = default;
        Framebuffer &operator=(Framebuffer&&) = default;
        virtual ~Framebuffer() = default;
    private:
        opengl::Framebuffer framebuffer;
        std::vector<EntityRef> texture_attachments;

        uint32_t width, height;
    };
}