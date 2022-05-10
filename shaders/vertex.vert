#version 460 core

layout (location = 0) in vec4 vert_data;
layout (location = 1) in vec2 p_tex_coord;

out vec4 v_color;
out vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main() 
{
    const vec4 pos[3] = vec4[3](
        vec4( 0.0,  0.5, 0.5, 1.0),
        vec4( 0.5, -0.5, 0.5, 1.0),
        vec4(-0.5, -0.5, 0.5, 1.0)
    );

    if(gl_VertexID == 0)
        v_color = vec4(1.0, 0.0, 0.0, 1.0);
    if(gl_VertexID == 1)
        v_color = vec4(0.0, 1.0, 0.0, 1.0);
    if(gl_VertexID == 2)
        v_color = vec4(0.0, 0.0, 1.0, 1.0);
    
    tex_coord = p_tex_coord;

    gl_Position = proj_mat * view_mat * model_mat * vert_data;
}