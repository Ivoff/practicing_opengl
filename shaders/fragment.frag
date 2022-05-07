#version 450 core

in vec4 v_color;
out vec4 f_color;

uniform float intensity;

void main() {
    f_color = vec4(v_color.x*intensity, v_color.y*intensity, v_color.z*intensity, v_color.w);
}