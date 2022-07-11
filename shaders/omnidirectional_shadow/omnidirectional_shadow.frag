#version 460 core

in vec4 frag_pos;

uniform vec3 light_pos;
uniform float far_plane;
uniform float near_plane;

void main()
{
    //float distance = length(frag_pos.xyz - light_pos);
    //distance = distance / far_plane;
    //gl_FragDepth = distance;
}