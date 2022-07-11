#version 460 core

out vec4 frag_color;

in vec3 tex_coord;

uniform samplerCube cubemap;

void main()
{
    frag_color = vec4(texture(cubemap, tex_coord).r, texture(cubemap, tex_coord).r, texture(cubemap, tex_coord).r, 1.0f);
}