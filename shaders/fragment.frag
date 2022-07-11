#version 460 core

struct Material
{    
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
       
    float constant;
    float linear;
    float quadratic;
};

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
    PointLight point_light;    
    vec2 tex_coord;
    vec3 camera_pos;
    vec3 normal;
    vec3 frag_pos;
};

void directional_light_func(in DataLight data, out vec3 result);
void point_light_func(in DataLight data, out vec3 result);
float directional_shadow_func(in vec4 light_frag_pos);
float omnidirectional_shadow_func(in vec3 frag_pos, in vec3 point_light_pos);
float VectorToDepthValue(vec3 Vec);

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;
in mat3 tbn_mat;
in vec4 light_frag_pos;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material material;
uniform PointLight point_light;
uniform DirectionalLight dir_light;

uniform int directional_active;

uniform float far_plane;
uniform float near_plane;
uniform int omnidirectional_shadow_enable;
uniform float omnidirectional_shadow_bias;

uniform float shadow_map_bias;
uniform int shadow_enable;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_specular_0;
uniform sampler2D texture_specular_1;
uniform sampler2D texture_normal_0;
uniform sampler2D texture_normal_1;
uniform sampler2D directional_shadow_map;
uniform samplerCube omnidirectional_shadow_map;

void main() 
{
    vec3 directional_result;
    vec3 point_result;
  
    DataLight data;
    data.directional_light = dir_light;
    data.point_light = point_light;
    data.tex_coord = tex_coord;
    data.camera_pos = camera_pos;    
    data.frag_pos = frag_pos;

    data.normal = texture(texture_normal_0, tex_coord).rgb;
    data.normal = data.normal * 2.0f - 1.0f;
    data.normal = normalize(tbn_mat * data.normal);

    directional_light_func(data, directional_result);
    point_light_func(data, point_result);

    if (directional_active == 0)
    {
        directional_result = vec3(0.0f, 0.0f, 0.0f);
    }

    frag_color = vec4(point_result + directional_result, 1.0f);
}

void directional_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.directional_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    // negando porque o vetor direção da luz direcional é da luz em direção ao fragmento e não do fragmento a luz
    vec3 light_dir = normalize(-data.directional_light.direction); 
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.directional_light.diffuse * material.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal); // aqui negar denovo para não precisar normalizar o vetor original de novo
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * texture(texture_specular_0, data.tex_coord).rgb * data.directional_light.specular;

    float shadow = 1.0f;
    if (shadow_enable == 1)
    {
        shadow = directional_shadow_func(light_frag_pos);
    }        

    result = ambient + shadow * (diffuse + specular);
}

void point_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.point_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 light_dir = normalize(data.point_light.position - data.frag_pos);
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.point_light.diffuse * material.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;
    
    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * vec3(texture(texture_specular_0, data.tex_coord).r) * data.point_light.specular;

    float distance = length(data.point_light.position - data.frag_pos);
    float attenuation = 1.0f / (data.point_light.constant + data.point_light.linear * distance + data.point_light.quadratic * (distance * distance));

    result = (ambient * attenuation) + (diffuse * attenuation) + (specular * attenuation);
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

float omnidirectional_shadow_func(in vec3 frag_pos, in vec3 point_light_pos)
{
    vec3 light_direction = frag_pos.xyz - point_light_pos;
    float cur_distance = VectorToDepthValue(light_direction);

    float closest_distance = texture(omnidirectional_shadow_map, light_direction).r;

    if ((closest_distance + omnidirectional_shadow_bias) < cur_distance)
    {
        return 0.0f;
    }
    else 
    {
        return 1.0f;
    }
}

float VectorToDepthValue(vec3 Vec)
{
    vec3 AbsVec = abs(Vec);
    float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));

    const float f = far_plane;
    const float n = near_plane;
    float NormZComp = (f+n) / (f-n) - (2*f*n)/(f-n)/LocalZcomp;
    return (NormZComp + 1.0) * 0.5;
}