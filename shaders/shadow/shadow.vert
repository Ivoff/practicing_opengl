#version 460 core

layout (location = 0) in vec3 p_vertex_data;
layout (location = 2) in vec2 p_tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

out vec2 tex_coord;

void main()
{
    tex_coord = p_tex_coord;
    gl_Position = proj_mat * view_mat * model_mat * vec4(p_vertex_data, 1.0f);
}