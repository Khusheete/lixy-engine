#pragma once


#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>


namespace lixy::opengl {

    class OpenGLContext {
    public:
        void initialize();

        void set_current();

        // ===== Window API =====
        inline GLFWwindow *get_window() { return window; }
        void window_set_title(const std::string &p_title);
        bool window_should_close();
        void swap_buffers();


        OpenGLContext() = default;
        OpenGLContext(const OpenGLContext&) = delete;
        OpenGLContext(OpenGLContext &&p_other);
        OpenGLContext &operator=(OpenGLContext &&p_other);
        virtual ~OpenGLContext();

    private:
        static int instance_count;

        GLFWwindow *window = nullptr;
    };
}