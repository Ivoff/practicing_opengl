#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vert_pos[];
in vec3 vert_normal[];
in vec2 tex_coord[];
in vec4 light_vert_pos[];

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_tex_coord;
out vec4 frag_light_vert_pos;

void main()
{
    vec3 triangle_normal = abs(
        cross(
            (vert_pos[1] - vert_pos[0]), 
            (vert_pos[2] - vert_pos[0])
        )
    );

    float dominant_axis = max(triangle_normal.x, max(triangle_normal.z, triangle_normal.y));

    for(int i = 0; i < 3; ++i)
    {
        frag_pos = vert_pos[i];
        frag_normal = vert_normal[i];
        frag_tex_coord = tex_coord[i];
        frag_light_vert_pos = light_vert_pos[i];

        if (dominant_axis == triangle_normal.x)
        {
            gl_Position = 1.5f * vec4(frag_pos.yz, 0.0f, 1.0f);
        }
        else if (dominant_axis == triangle_normal.y)
        {
            gl_Position = 1.5f * vec4(frag_pos.xz, 0.0f, 1.0f);
        }
        else if (dominant_axis == triangle_normal.z)
        {
            gl_Position = 1.5f * vec4(frag_pos.xy, 0.0f, 1.0f);
        }

        EmitVertex();
    }

    EndPrimitive();
}