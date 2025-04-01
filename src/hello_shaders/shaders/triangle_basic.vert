#version 330 core

layout(location = 0) in vec3 aPos;

void main() {
    // this sets the output of the vertex shader
    // we just set it to the exact same number as the input position
    // usually the input is not in normalized device coordinates, so we would
    // usually have to scale it ourselves
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.);
}