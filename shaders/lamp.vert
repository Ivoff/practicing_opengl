#version 460 core

layout (location = 0) in vec4 vert_data;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main()
{
    gl_Position = proj_mat * view_mat * model_mat * vert_data;
}