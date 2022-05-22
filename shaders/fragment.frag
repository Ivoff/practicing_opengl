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

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;
in mat3 tbn_mat;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material            material;
uniform PointLight          point_light;
uniform DirectionalLight    dir_light;

uniform int                 directional_active;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_specular_0;
uniform sampler2D texture_specular_1;
uniform sampler2D texture_normal_0;
uniform sampler2D texture_normal_1;

void main() 
{
    vec3 directional_result;
    vec3 point_result;
  
    DataLight data;
    data.directional_light = dir_light;
    data.point_light = point_light;
    data.tex_coord = tex_coord;
    data.camera_pos = camera_pos;
    data.normal = normal;
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

    result = ambient + diffuse + specular;
}

void point_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.point_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 light_dir = normalize(data.point_light.position - data.frag_pos);
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.point_light.diffuse * material.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;
    
    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * texture(texture_specular_0, data.tex_coord).rgb * data.point_light.specular;

    float distance = length(data.point_light.position - data.frag_pos);
    float attenuation = 1.0f / (data.point_light.constant + data.point_light.linear * distance + data.point_light.quadratic * (distance * distance));

    result = (ambient * attenuation) + (diffuse * attenuation) + (specular * attenuation);
}