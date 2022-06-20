#version 460 core

layout (location = 0) in vec3 vert_data;
layout (location = 1) in vec3 p_normal;
layout (location = 2) in vec2 p_tex_coord;
layout (location = 3) in vec3 p_tangent;
layout (location = 4) in vec3 p_bitangent;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;
out mat3 tbn_mat;
out vec4 light_frag_pos;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

uniform mat3 normal_mat;

uniform mat4 light_view_mat;
uniform mat4 light_proj_mat;

void main() 
{
    normal = normalize(normal_mat * p_normal);

    tbn_mat = mat3(
        normalize((model_mat * vec4(p_tangent, 0.0f)).xyz),
        normalize((model_mat * vec4(p_bitangent, 0.0f)).xyz),
        normalize((model_mat * vec4(normal, 0.0f)).xyz)
    );
        
    frag_pos = (model_mat * vec4(vert_data, 1.0f)).xyz;
    light_frag_pos = (light_proj_mat * light_view_mat * vec4(frag_pos, 1.0f));
    tex_coord = p_tex_coord;

    gl_Position = proj_mat * view_mat * model_mat * vec4(vert_data, 1.0f);
}