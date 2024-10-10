#include "context.hpp"
#include "debug/debug.hpp"

#include <GLFW/glfw3.h>


namespace lixy {
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