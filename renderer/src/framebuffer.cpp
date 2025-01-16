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


#include "framebuffer.hpp"

#include "core/src/ref.hpp"
#include "renderer/src/primitives/texture.hpp"
#include "renderer/src/texture.hpp"
#include "thirdparty/flecs/flecs.h"


namespace lixy {

    void Framebuffer::bind() const {
        framebuffer.bind();
    }


    void Framebuffer::unbind() const {
        framebuffer.unbind();
    }


    bool Framebuffer::is_complete() const {
        return framebuffer.is_complete();
    }


    void Framebuffer::set_size(uint32_t p_width, uint32_t p_height) {
        return framebuffer.set_size(p_width, p_height);
    }


    EntityRef Framebuffer::get_attachment(int p_index) {
        return texture_attachments[p_index];
    }


    EntityRef Framebuffer::create(flecs::world &p_world, uint32_t p_width, uint32_t p_height, const std::vector<opengl::TextureFormat> &p_texture_attachment_formats) {
        EntityRef fb_ref = EntityRef::create_reference(p_world).add<Framebuffer>();
        Framebuffer *fb = fb_ref.get_mut<Framebuffer>();

        // Create texture attachments
        std::vector<uint32_t> texture_ids(p_texture_attachment_formats.size());

        fb->texture_attachments.reserve(p_texture_attachment_formats.size());
        for (int i = 0; i < p_texture_attachment_formats.size(); i++) {
            opengl::TextureFormat texture_format = p_texture_attachment_formats[i];
            EntityRef tex_ref = Texture::create_texture2d(p_world, p_width, p_height, p_texture_attachment_formats[i])
                .add(flecs::ChildOf, fb_ref.get_entity());
            
            const Texture *tex = tex_ref.get<Texture>();
            texture_ids[i] = tex->get_texture_id();
            
            fb->texture_attachments.push_back(tex_ref);
        }

        // Create framebuffer object
        fb->framebuffer = opengl::Framebuffer(p_width, p_height, texture_ids);

        return fb_ref;
    }
}