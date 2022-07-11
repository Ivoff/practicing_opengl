#version 460 core

layout (location = 0) in vec3 vert_data;

out vec3 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main() 
{
    vec3 frag_pos = (model_mat * vec4(vert_data, 1.0f)).xyz;
    tex_coord = frag_pos;

    gl_Position = proj_mat * view_mat * model_mat * vec4(frag_pos, 1.0f);
}