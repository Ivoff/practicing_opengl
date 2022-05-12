#version 450 core

in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

out vec4 f_color;

uniform sampler2D texture0;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 camera_pos;

void main() 
{    
    float specular_intensity = 0.5f;
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 diffuse = max(dot(light_dir, normal), 0.0f) * light_color;

    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
    vec3 specular = spec * specular_intensity * light_color;
    
    vec3 f_intensity = light_color * (diffuse + specular);

    f_color = texture(texture0, tex_coord) * vec4(f_intensity, 1.0f);
}