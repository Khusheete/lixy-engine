#include "shader.hpp"

#include "debug/debug.hpp"

#include "thirdparty/glm/glm.hpp"

#include <algorithm>
#include <asm-generic/errno-base.h>
#include <sstream>
#include <vector>



namespace lixy::opengl {
    void ShaderProgram::bind() const {
        glUseProgram(program_id);
    }


    bool ShaderProgram::is_bound() const {
        int pid;
        glGetIntegerv(GL_CURRENT_PROGRAM, &pid);
        return ((uint32_t)pid) == program_id;
    }


    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const float &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform1f(location, p_value);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::vec2 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform2f(location, p_value.x, p_value.y);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::vec3 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform3f(location, p_value.x, p_value.y, p_value.z);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::vec4 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform4f(location, p_value.x, p_value.y, p_value.z, p_value.w);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const int &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform1i(location, p_value);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::ivec2 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform2i(location, p_value.x, p_value.y);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::ivec3 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform3i(location, p_value.x, p_value.y, p_value.z);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::ivec4 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniform4i(location, p_value.x, p_value.y, p_value.z, p_value.w);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::mat2 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniformMatrix2fv(location, 1, GL_FALSE, &p_value[0][0]);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::mat3 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniformMatrix3fv(location, 1, GL_FALSE, &p_value[0][0]);
    }
    
    
    void ShaderProgram::bind_uniform(const std::string &p_uniform_name, const glm::mat4 &p_value) {
        int32_t location = _get_uniform_location(p_uniform_name);
        if (location == -1) return;
        glUniformMatrix4fv(location, 1, GL_FALSE, &p_value[0][0]);
    }


    int32_t ShaderProgram::_get_uniform_location(const std::string &p_name) {
        return glGetUniformLocation(program_id, p_name.c_str());
    }


    void ShaderProgram::unbind() const {
        glUseProgram(0);
    }


    int ShaderProgram::get_uniform_count() {
        return uniforms.size();
    }
    
    
    int ShaderProgram::get_uniform_index(const std::string &p_uniform_name) {
        for (int i = 0; i < uniforms.size(); i++) {
            if (uniforms[i].name == p_uniform_name) return i;
        }
        return -1;
    }
    
    
    const std::string &ShaderProgram::get_uniform_name(int index) {
        ASSERT_FATAL_ERROR(index >= 0 && index < get_uniform_count(), "Error index out of bounds");
        return uniforms[index].name;
    }
    
    
    ShaderDataType ShaderProgram::get_uniform_type(int index) {
        ASSERT_FATAL_ERROR(index >= 0 && index < get_uniform_count(), "Error index out of bounds");
        return uniforms[index].type;
    }


    bool ShaderProgram::is_valid() const {
        return !creation_error;
    }


    const std::string &ShaderProgram::get_errors() const {
        return errors;
    }


    ShaderProgram::ShaderProgram(const std::string &p_vertex_source, const std::string &p_fragment_source) {
        std::stringstream error_stream;

        // Compile vertex shader
        const char *cvertex_source = p_vertex_source.c_str();
        uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &cvertex_source, nullptr);
        glCompileShader(vertex_shader);

        int32_t ok;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            creation_error = true;
            int32_t max_length;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> error_logs(max_length);
            glGetShaderInfoLog(vertex_shader, max_length, &max_length, error_logs.data());
            error_stream << "Error while compiling vertex shader:" << std::endl;
            error_stream << error_logs.data() << std::endl;
        }

        // Compile fragment shader
        const char *cfragment_source = p_fragment_source.c_str();
        uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &cfragment_source, nullptr);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            creation_error = true;
            int32_t max_length;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> error_logs(max_length);
            glGetShaderInfoLog(fragment_shader, max_length, &max_length, error_logs.data());
            error_stream << "Error while compiling fragment shader:" << std::endl;
            error_stream << error_logs.data() << std::endl;
        }

        // Create shader program
        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader);
        glAttachShader(program_id, fragment_shader);
        glLinkProgram(program_id);

        glGetProgramiv(program_id, GL_LINK_STATUS, &ok);
        if (!ok) {
            creation_error = true;
            int32_t max_length;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> error_logs(max_length);
            glGetProgramInfoLog(program_id, max_length, &max_length, error_logs.data());
            error_stream << "Error while linking shader program:" << std::endl;
            error_stream << error_logs.data() << std::endl;        
        }

        // Free used shaders
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);

        // Get uniforms
        int32_t uniform_count;
        glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &uniform_count);
        uniforms.reserve(uniform_count);
    
        int32_t max_name_length;
        glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);
        std::vector<GLchar> name(max_name_length);

        for (int i = 0; i < uniform_count; i++) {
            int32_t size;
            uint32_t type;
            glGetActiveUniform(program_id, i, max_name_length, nullptr, &size, &type, name.data());
            
            uniforms.push_back(Uniform{
                .type = shader_data_type_from_gl_type(type),
                .name = std::string(name.data())
            });
        }

        // Record possible errors
        if (creation_error) errors = error_stream.str();
    }


    ShaderProgram::ShaderProgram(ShaderProgram &&p_other)
        : program_id(p_other.program_id),
        creation_error(p_other.creation_error),
        errors(std::move(p_other.errors))
    {
        p_other.program_id = 0;
    }


    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&p_other) {
        if (program_id != 0) {
            glDeleteProgram(program_id);
        }

        program_id = p_other.program_id;
        p_other.program_id = 0;
        
        return *this;
    }


    ShaderProgram::~ShaderProgram() {
        if (program_id == 0) return;
        glDeleteProgram(program_id);
    }


    // ShaderProgram ShaderProgram::load_shader_program(const std::string &p_vertex_path, const std::string &p_fragment_path) {
    //     return ShaderProgram(_read_file(p_vertex_path), _read_file(p_fragment_path));
    // }
}