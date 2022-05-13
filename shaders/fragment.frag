#version 460 core

struct Material
{    
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 tex_coord;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material material;
uniform Light light;

void main() 
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffuse = max(dot(light_dir, normal), 0.0f) * light.diffuse * vec3(texture(material.diffuse, tex_coord));
    
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * vec3(texture(material.specular, tex_coord)) * light.specular;

    frag_color = vec4(ambient + diffuse + specular, 1.0f);
}