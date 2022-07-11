#version 460 core

layout (location = 0) in vec3 p_vertex_data;

uniform mat4 model_mat;

void main()
{
    gl_Position = model_mat * vec4(p_vertex_data, 1.0f);
}