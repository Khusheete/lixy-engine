#pragma once


#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>


class OpenGLContext {
public:
    void initialize();

    void set_current();

    // ===== Window API =====
    void window_set_title(const std::string &p_title);
    bool window_should_close();
    void swap_buffers();


    OpenGLContext() = default;
    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext(OpenGLContext&&) = delete;
    virtual ~OpenGLContext();

private:
    static int instance_count;

    GLFWwindow *window;
};