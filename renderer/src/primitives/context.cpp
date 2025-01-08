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


#include "context.hpp"
#include "debug/debug.hpp"

#include <GLFW/glfw3.h>


namespace lixy::opengl {
    int OpenGLContext::instance_count = 0;


    void OpenGLContext::initialize() {
        if (instance_count == 0) {
            ASSERT_FATAL_ERROR(glfwInit(), "Could not initialize glfw");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(800, 600, "", nullptr, nullptr);
        ASSERT_FATAL_ERROR(window, "Could not create window");
        glfwMakeContextCurrent(window);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glEnable(GL_DEPTH_TEST);

        if (instance_count == 0) {
            ASSERT_FATAL_ERROR(glewInit() == GLEW_OK, "Could not initialize glew");
        }

        instance_count += 1;
    }


    void OpenGLContext::set_current() {
        glfwMakeContextCurrent(window);
    }


    void OpenGLContext::window_set_title(const std::string &p_title) {
        glfwSetWindowTitle(window, p_title.c_str());
    }


    bool OpenGLContext::window_should_close() {
        return glfwWindowShouldClose(window);
    }


    void OpenGLContext::swap_buffers() {
        glfwSwapBuffers(window);
    }


    OpenGLContext::OpenGLContext(OpenGLContext &&p_other)
        : window(p_other.window)
    {
        p_other.window = nullptr;
    }

    OpenGLContext &OpenGLContext::operator=(OpenGLContext &&p_other) {
        if (window != nullptr) {
            glfwDestroyWindow(window);
            instance_count -= 1;
        }
        
        window = p_other.window;
        p_other.window = nullptr;

        return *this;
    }


    OpenGLContext::~OpenGLContext() {
        if (!window) return;

        glfwDestroyWindow(window);

        instance_count -= 1;
        if (instance_count == 0) {
            glfwTerminate();
        }
    }
}