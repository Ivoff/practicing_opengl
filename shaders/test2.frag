#version 460 core 

out vec4 frag_out;
in vec4 light_frag_pos;
uniform sampler2D shadow_map;
uniform float shadow_map_bias;

float shadow_func(in vec4 light_frag_pos)
{
    light_frag_pos = light_frag_pos / light_frag_pos.w;
    light_frag_pos = light_frag_pos * 0.5f + 0.5f;
    
    if (light_frag_pos.z > 1.0f)
    {
        light_frag_pos.z = 1.0f;
    }

    return light_frag_pos.z;
}

void main()
{
    float depth = shadow_func(light_frag_pos);
    frag_out = vec4(depth, depth, depth, 1.0f);
}