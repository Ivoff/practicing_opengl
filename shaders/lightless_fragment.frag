#version 460 core

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_diffuse_2;
uniform sampler2D texture_diffuse_3;

uniform sampler2D texture_specular_0;
uniform sampler2D texture_specular_1;
uniform sampler2D texture_specular_2;
uniform sampler2D texture_specular_3;

void main() 
{
    frag_color = texture(texture_diffuse_0, tex_coord);
}