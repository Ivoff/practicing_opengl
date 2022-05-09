#version 450 core

in vec4 v_color;
in vec2 tex_coord;

out vec4 f_color;

uniform float intensity;
uniform sampler2D texture0;

void main() {
    f_color = texture(texture0, tex_coord);
}