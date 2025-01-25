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

#include "thirdparty/rgfw/include.hpp"


namespace lixy::opengl {
    int OpenGLContext::instance_count = 0;


    void OpenGLContext::initialize() {
        RGFW_setGLVersion(RGFW_glCore, 4, 4);
        
        window = RGFW_createWindow("", RGFW_RECT(0, 0, 800, 600), RGFW_windowCenter);
        ASSERT_FATAL_ERROR(window, "Could not create window");
        RGFW_window_makeCurrent(window);

        // Load OpenGL functions
        if (instance_count == 0) {
            ASSERT_FATAL_ERROR(gladLoadGLLoader((GLADloadproc)RGFW_getProcAddress), "Could not initialize glad");
        }

        // Print opengl context information
        const uint8_t *opengl_version_info = glGetString(GL_VERSION);
        const uint8_t *hardware_info = glGetString(GL_RENDERER);
        std::cout << "OpenGL API " << opengl_version_info << " - Using: " << hardware_info << std::endl;

        int context_flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

        // Add debug context
        if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(&OpenGLContext::_gl_debug_context, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        } else {
            LOG_WARNING("Could not set an OpenGL debug context");
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glEnable(GL_DEPTH_TEST);

        instance_count += 1;
    }


    void APIENTRY OpenGLContext::_gl_debug_context(GLenum p_source, GLenum p_type, GLuint p_id, GLenum p_severity, GLsizei p_length, const char *p_message, const void *p_user_param) {
        // ignore non-significant error/warning codes
        if(p_id == 131169 || p_id == 131185 || p_id == 131218 || p_id == 131204) return; // TODO: check

        std::string_view message_color;

        switch (p_type) {
            case GL_DEBUG_TYPE_ERROR:
                message_color = CSI_RED;
                std::cout << CSI_RED << "OpenGL Error: "; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Deprecated Behaviour: "; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Undefined Behaviour: "; break; 
            case GL_DEBUG_TYPE_PORTABILITY:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Portability: "; break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Performance: "; break;
            case GL_DEBUG_TYPE_MARKER:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Marker: "; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Push Group: "; break;
            case GL_DEBUG_TYPE_POP_GROUP:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Pop Group: "; break;
            case GL_DEBUG_TYPE_OTHER:
                message_color = CSI_YELLOW;
                std::cout << CSI_YELLOW << "OpenGL Warning - Other: "; break;
        }

        switch (p_severity) {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << CSI_BOLD << "Severity: high; " << CSI_CLEAR << message_color; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium; "; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low; "; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification; "; break;
        }

        switch (p_source) {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API; "; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System; "; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler; "; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party; "; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application; "; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other; "; break;
        }

        std::cout << "Debug message (" << p_id << "):\n" <<  p_message << CSI_CLEAR << std::endl;
    }



    void OpenGLContext::set_current() {
        RGFW_window_makeCurrent(window);
    }


    void OpenGLContext::window_set_title(const std::string_view &p_title) {
        RGFW_window_setName(window, p_title.data());
    }


    bool OpenGLContext::window_should_close() {
        return RGFW_window_shouldClose(window);
    }


    void OpenGLContext::window_poll_events() {
        while (RGFW_window_checkEvent(window));
    }


    void OpenGLContext::swap_buffers() {
        RGFW_window_swapBuffers(window);
    }

    
    int32_t OpenGLContext::window_get_width() const {
        return window->r.w;
    }
    
    
    int32_t OpenGLContext::window_get_height() const {
        return window->r.h;
    }


    OpenGLContext::OpenGLContext(OpenGLContext &&p_other)
        : window(p_other.window)
    {
        p_other.window = nullptr;
    }

    OpenGLContext &OpenGLContext::operator=(OpenGLContext &&p_other) {
        if (window != nullptr) {
            RGFW_window_close(window);
            instance_count -= 1;
        }
        
        window = p_other.window;
        p_other.window = nullptr;

        return *this;
    }


    OpenGLContext::~OpenGLContext() {
        if (!window) return;
        RGFW_window_close(window);
        instance_count -= 1;
    }
}