#version 460 core

in vec2 tex_coord;

out vec4 frag_out;

uniform sampler2D texture_diffuse_0;

void main()
{
    frag_out = vec4(texture(texture_diffuse_0, tex_coord).rgb, 1.0f);
}