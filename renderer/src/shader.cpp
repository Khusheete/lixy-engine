#include "shader.hpp"
#include "debug/debug.hpp"

#include <asm-generic/errno-base.h>
#include <sstream>
#include <fstream>
#include <vector>


std::string _read_file(const std::string &file_path) {
    std::fstream fi(file_path);
    std::stringstream string_buffer;
    string_buffer << fi.rdbuf();
    fi.close();

    return string_buffer.str();
}


void Shader::bind() {
    glUseProgram(program_id);
}


void Shader::unbind() {
    glUseProgram(0);
}


bool Shader::is_valid() {
    return !creation_error;
}


const std::string &Shader::get_errors() {
    return errors;
}


Shader::Shader(const std::string &p_vertex_source, const std::string &p_fragment_source) {
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

    // Record possible errors
    if (creation_error) errors = error_stream.str();
}


Shader::Shader(Shader &&p_other)
    : program_id(p_other.program_id),
    creation_error(p_other.creation_error),
    errors(std::move(p_other.errors))
{
    p_other.program_id = 0;
}


Shader::~Shader() {
    if (program_id == 0) return;
    glDeleteProgram(program_id);
}


Shader Shader::load_shader_program(const std::string &p_vertex_path, const std::string &p_index_path) {
    return Shader(_read_file(p_vertex_path), _read_file(p_index_path));
}