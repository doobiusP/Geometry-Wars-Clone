#version 450 core

out vec4 fragColor;
uniform vec4 u_polyColor;

void main(){
    fragColor = u_polyColor;
}
