#version 460 core

layout (location = 0) in vec3 vert_data;
layout (location = 1) in vec3 p_normal;
layout (location = 2) in vec2 p_tex_coord;

out vec3 vert_pos;
out vec3 vert_normal;
out vec2 tex_coord;
out vec4 light_vert_pos;

uniform mat4 light_view_mat;
uniform mat4 light_proj_mat;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

uniform mat3 normal_mat;

void main()
{
    vert_pos = (proj_mat * view_mat * model_mat * vec4(vert_data, 1.0f)).xyz;
    // vert_pos = vert_data;
    vert_normal = normalize(normal_mat * p_normal);
    tex_coord = p_tex_coord;
    light_vert_pos = light_proj_mat * light_view_mat * model_mat * vec4(vert_data, 1.0f);

    gl_Position = proj_mat * view_mat * model_mat * vec4(vert_pos, 1.0f);
}