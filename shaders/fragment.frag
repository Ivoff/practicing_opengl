#version 460 core

struct Material
{    
    sampler2D diffuse;
    sampler2D specular;
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

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material            material;
uniform PointLight          point_light;
uniform DirectionalLight    dir_light;

void main() 
{
//    vec3 ambient = point_light.ambient * texture(material.diffuse, tex_coord).rgb;
//
//    vec3 light_dir = normalize(point_light.position - frag_pos);
//    vec3 diffuse = max(dot(light_dir, normal), 0.0f) * point_light.diffuse * texture(material.diffuse, tex_coord).rgb;
//    
//    vec3 view_dir = normalize(camera_pos - frag_pos);
//    vec3 reflect_dir = reflect(-light_dir, normal);    
//    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * texture(material.specular, tex_coord).rgb * light.specular;

    vec3 directional_result;
    vec3 point_result;
  
    DataLight data;
    data.directional_light = dir_light;
    data.point_light = point_light;
    data.tex_coord = tex_coord;
    data.camera_pos = camera_pos;
    data.normal = normal;
    data.frag_pos = frag_pos;

    directional_light_func(data, directional_result);
    point_light_func(data, point_result);

    frag_color = vec4(point_result, 1.0f);
}

void directional_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.directional_light.ambient * texture(material.diffuse, data.tex_coord).rgb;

    // negando porque o vetor direção da luz direcional é da luz em direção ao fragmento e não do fragmento a luz
    vec3 light_dir = normalize(-data.directional_light.direction); 
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * 
                   data.directional_light.diffuse * 
                   texture(material.diffuse, data.tex_coord).rgb;

    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal); // aqui negar denovo para não precisar normalizar o vetor original de novo
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * 
                    texture(material.specular, data.tex_coord).rgb * 
                    data.directional_light.specular;

    result = ambient + diffuse + specular;
}

void point_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.point_light.ambient * texture(material.diffuse, data.tex_coord).rgb;

    vec3 light_dir = normalize(data.point_light.position - data.frag_pos);
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.point_light.diffuse * texture(material.diffuse, data.tex_coord).rgb;
    
    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * texture(material.specular, data.tex_coord).rgb * data.point_light.specular;

    float distance = length(data.point_light.position - data.frag_pos);
    float attenuation = 1.0f / (data.point_light.constant + data.point_light.linear * distance + data.point_light.quadratic * (distance * distance));

    result = ambient * attenuation + diffuse * attenuation + specular * attenuation;
}