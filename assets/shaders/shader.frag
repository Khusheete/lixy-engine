#version 420 core

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 out_color;


uniform sampler2D u_texture;


void main() {
    out_color = texture(u_texture, uv);
}