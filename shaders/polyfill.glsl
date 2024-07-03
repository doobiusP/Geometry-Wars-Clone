#version 450 core

out vec4 fragColor;
uniform vec3 u_polyColor;

void main(){
    fragColor = vec4(u_polyColor, 1.0f);
}
