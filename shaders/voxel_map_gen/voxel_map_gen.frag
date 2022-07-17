#version 460 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coord;
in vec4 frag_light_vert_pos;

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
};

struct DataLight
{
    DirectionalLight directional_light;
    vec2 tex_coord;
    vec3 normal;
    vec3 frag_pos;
    vec4 light_frag_pos;
};

uniform sampler2D texture_diffuse_0;
uniform sampler2D directional_shadow_map;
layout(RGBA8) uniform image3D voxel_map;

uniform DirectionalLight dir_light;
uniform float shadow_map_bias;
uniform int shadow_active;

vec3 directional_light_func(in DataLight data);
float directional_shadow_func(in vec4 light_frag_pos);

void main()
{
    DataLight data;
    data.directional_light = dir_light;
    data.tex_coord = frag_tex_coord;
    data.normal = frag_normal;
    data.frag_pos = frag_pos;
    data.light_frag_pos = frag_light_vert_pos;

    vec4 frag_color = vec4(directional_light_func(data), 1.0f);

    ivec3 voxel_map_size = imageSize(voxel_map);
    vec3 frag_pos_normalized = frag_pos * 0.5f + 0.5;
    ivec3 location = ivec3(frag_pos_normalized * voxel_map_size);
    imageStore(voxel_map, location, frag_color);
}

vec3 directional_light_func(in DataLight data)
{
    vec3 ambient = data.directional_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 light_dir = normalize(-data.directional_light.direction); 
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.directional_light.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;
    
    float shadow = 1.0f;
    if (shadow_active == 1)
    {
        shadow = directional_shadow_func(data.light_frag_pos);
    }    
    
    return shadow * diffuse;
}

float directional_shadow_func(in vec4 light_frag_pos)
{
    light_frag_pos = light_frag_pos / light_frag_pos.w;
    light_frag_pos = light_frag_pos * 0.5f + 0.5f;
    
    if (light_frag_pos.z > 1.0f)
    {
        light_frag_pos.z = 1.0f;
    }

    float depth = texture(directional_shadow_map, light_frag_pos.xy).r;

    if ((depth + shadow_map_bias) < light_frag_pos.z)
    {
        return 0.0f;
    } 
    else
    {
        return 1.0f;
    }    
}