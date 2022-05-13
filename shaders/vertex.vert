#version 460 core

layout (location = 0) in vec4 vert_data;
layout (location = 1) in vec2 p_tex_coord;
layout (location = 2) in vec3 p_normal;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

uniform mat3 normal_mat;

void main() 
{
    const vec4 pos[3] = vec4[3](
        vec4( 0.0,  0.5, 0.5, 1.0),
        vec4( 0.5, -0.5, 0.5, 1.0),
        vec4(-0.5, -0.5, 0.5, 1.0)
    );
        
    normal = normalize(normal_mat * p_normal);
    frag_pos = vec3(model_mat * vert_data);
    tex_coord = p_tex_coord;

    gl_Position = proj_mat * view_mat * model_mat * vert_data;
}