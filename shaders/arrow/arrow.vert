#version 460 core

layout (location = 0) in vec3 vert_data;
layout (location = 1) in vec3 p_normal;
layout (location = 2) in vec2 p_tex_coord;
layout (location = 3) in vec3 p_tangent;
layout (location = 4) in vec3 p_bitangent;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main()
{    
    gl_Position = proj_mat * view_mat * model_mat * vec4(vert_data, 1.0f);
}