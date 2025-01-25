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


#include "thirdparty/flecs/flecs.h"
#include "thirdparty/rgfw/include.hpp"

#include <functional>
#include <string_view>


namespace lixy {
    typedef void(*ProcAddress);


    class Window {
        public:
            static Window *get_singleton(flecs::world &p_world);
            static ProcAddress get_proc_address(const char *p_proc);

            void set_as_current_context();

            bool should_close();
            void set_title(const std::string_view &p_title);
            int32_t get_width() const;
            int32_t get_height() const;
            void poll_events();
            void swap_buffers();

            Window() = default;
            Window(uint32_t p_width, uint32_t p_height, std::string_view p_name);
            Window(Window &&p_other);
            Window &operator=(Window &&p_other);
            virtual ~Window();

        private:
            RGFW_window *window_ptr = nullptr;
    };
}