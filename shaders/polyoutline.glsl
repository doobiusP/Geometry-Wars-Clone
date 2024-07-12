#version 450 core
out vec4 outlineColor;

uniform vec4 inOutlineColor;
void main(){
    outlineColor = inOutlineColor;
}