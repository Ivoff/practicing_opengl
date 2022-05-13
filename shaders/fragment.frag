#version 460 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
};

in vec3 frag_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material material;
uniform Light light;

void main() 
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffuse = max(dot(light_dir, normal), 0.0f) * light.diffuse * material.diffuse;
    
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * light.specular;        

    frag_color = vec4(ambient + diffuse + specular, 1.0f);
}