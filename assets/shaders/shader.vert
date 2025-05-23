#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 out_uv;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;


void main() {
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    out_uv = uv;
}