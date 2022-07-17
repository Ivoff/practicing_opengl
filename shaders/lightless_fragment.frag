#version 460 core

in vec2 tex_coord;
in mat3 tbn_mat;
in vec3 normal;

out vec4 frag_color;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_diffuse_1;

uniform sampler2D texture_specular_0;
uniform sampler2D texture_specular_1;

uniform sampler2D texture_normal_0;
uniform sampler2D texture_normal_1;

uniform int map_type;

void main() 
{    
    vec4 color;

    if (map_type == 1)
    {
        color = texture(texture_diffuse_0, tex_coord);
    }
    else if (map_type == 2)
    {
        color = vec4(vec3(texture(texture_specular_0, tex_coord).r), 1.0f);
    }
    else if (map_type == 3)
    {
        vec3 t_normal = texture(texture_normal_0, tex_coord).rgb;
        // normal = normal * 2.0f - 1.0f;
        // normal = normalize(tbn_mat * normal);
        color = vec4(t_normal, 1.0f);
    }
    else if (map_type == 4)
    {
        vec3 aux = normal;
        if (normal.x < 0.0001f)
        {
            aux.x = 0.0f;
        }
        if (normal.y < 0.0001f)
        {
            aux.y = 0.0f;
        }
        if (normal.z < 0.0001f)
        {
            aux.z = 0.0f;
        }

        color = vec4(aux, 1.0f);
    }

    frag_color = color;
}