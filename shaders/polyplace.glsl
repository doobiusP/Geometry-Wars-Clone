#version 450 core
layout(location=0) in vec2 aPos;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(aPos, 0.0f, 1.0f);
}