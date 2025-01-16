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


#include "texture.hpp"

#include <cstdint>
#include <vector>


namespace lixy::opengl {

    class Framebuffer;


    class RenderBufferObject {
        public:
            enum class RenderBufferFormat {
                RED, RG, RGB, RGBA, DEPTH_STENSIL,
            };

        public:
            void bind() const;
            void unbind() const;

            void set_size(int32_t p_width, int32_t p_height);

            RenderBufferObject() = default;
            RenderBufferObject(uint32_t p_width, uint32_t p_height, RenderBufferFormat p_format);
            RenderBufferObject(const RenderBufferObject &p_other) = delete;
            RenderBufferObject(RenderBufferObject &&p_other);
            RenderBufferObject &operator=(RenderBufferObject &&p_other);
            virtual ~RenderBufferObject();
        
        private:
            uint32_t get_internal_format() const;

        private:
            uint32_t buffer_id = 0;
            RenderBufferFormat format;

            friend Framebuffer;
    };


    class Framebuffer {
        public:
            void bind() const;
            void unbind() const;

            bool is_complete() const;

            inline uint32_t get_width() const { return width; };
            inline uint32_t get_height() const { return height; };

            inline int get_attachment_count() { return texture_attachments.size(); };
            uint32_t get_texture_attachment(int p_index);

            void set_size(uint32_t p_width, uint32_t p_height);

            Framebuffer() = default;
            Framebuffer(uint32_t p_width, uint32_t p_height, const std::vector<uint32_t> &p_texture_attachments);
            Framebuffer(const Framebuffer &p_other) = delete;
            Framebuffer(Framebuffer &&p_other);
            Framebuffer &operator=(Framebuffer &&p_other);
            virtual ~Framebuffer();

        private:
            uint32_t buffer_id = 0;
            uint32_t width, height;

            std::vector<uint32_t> texture_attachments;
            RenderBufferObject depth_stensil_attachment;
    };
}