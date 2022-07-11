#version 460 core

out vec4 frag_color;

in vec3 frag_pos;

uniform vec3 point_light_pos;
uniform samplerCube cubemap;
uniform int type;
uniform float far_plane;
uniform float bias;

void main()
{
    vec3 light_direction = frag_pos - point_light_pos;
    if (type == 0)
    {
        frag_color = vec4(vec3(texture(cubemap, light_direction).r), 1.0f);
    }
    else if (type == 1)
    {
        frag_color = vec4(vec3(length(light_direction) / far_plane), 1.0f);
    }
    else if (type == 2)
    {
        if((texture(cubemap, light_direction).r * far_plane + bias) < length(light_direction))
        {
            frag_color = vec4(vec3(0.0f), 1.0f);
        }
        else
        {
            frag_color = vec4(1.0f);
        }
    }
    else if (type == 3)
    {
        if((texture(cubemap, light_direction).r + bias) < length(light_direction) / far_plane)
        {
            frag_color = vec4(vec3(0.0f), 1.0f);
        }
        else
        {
            frag_color = vec4(1.0f);
        }
    }
}