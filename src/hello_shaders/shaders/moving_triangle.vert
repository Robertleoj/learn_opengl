#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;

uniform vec2 shift;

out vec4 vertex_color;

void main() {
    gl_Position = vec4(a_pos.xy + shift, a_pos.z, 1.);

    vertex_color = vec4(((a_color.xy + shift) / 2.) + 0.5, a_color.z, 1.);
}