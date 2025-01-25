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


#include "window.hpp"

#include "debug/debug.hpp"
#include "thirdparty/flecs/flecs.h"



namespace lixy {

    ProcAddress Window::get_proc_address(const char *p_proc) {
        return RGFW_getProcAddress(p_proc);
    }


    void Window::set_as_current_context() {
        RGFW_window_makeCurrent(window_ptr);
        RGFW_window_makeCurrent_OpenGL(window_ptr);
    }

    
    
    bool Window::should_close() {
        return RGFW_window_shouldClose(window_ptr);
    }
    
    
    void Window::set_title(const std::string_view &p_title) {
        RGFW_window_setName(window_ptr, p_title.data());
    }
    
    
    int32_t Window::get_width() const {
        return window_ptr->r.w;
    }
    
    
    int32_t Window::get_height() const {
        return window_ptr->r.h;
    }
    
    
    void Window::poll_events() {
        RGFW_event *event = nullptr;

        while ((event = RGFW_window_checkEvent(window_ptr)));
    }
    
    
    void Window::swap_buffers() {
        RGFW_window_swapBuffers(window_ptr);
    }


    Window *Window::get_singleton(flecs::world &p_world) {
        return p_world.get_mut<Window>();
    }


    Window::Window(uint32_t p_width, uint32_t p_height, std::string_view p_name) {
        RGFW_setGLVersion(RGFW_glCore, 4, 4);
        window_ptr = RGFW_createWindow("", RGFW_RECT(0, 0, 800, 600), RGFW_windowCenter);
        ASSERT_FATAL_ERROR(window_ptr, "Could not create window");
        set_as_current_context();
    }
    
    
    Window::Window(Window &&p_other) {
        if (window_ptr) {
            RGFW_window_close(window_ptr);
        }

        window_ptr = p_other.window_ptr;

        p_other.window_ptr = nullptr;
    }
    
    
    Window &Window::operator=(Window &&p_other) {
        if (window_ptr) {
            RGFW_window_close(window_ptr);
        }

        window_ptr = p_other.window_ptr;

        p_other.window_ptr = nullptr;

        return *this;
    }
    
    
    Window::~Window() {
        if (window_ptr) {
            RGFW_window_close(window_ptr);
        }
    }
}