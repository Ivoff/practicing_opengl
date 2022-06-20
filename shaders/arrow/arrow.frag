#version 460 core

struct Material
{    
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

out vec4 frag_out;

void main()
{
    frag_out = vec4(material.diffuse, 1.0f);
}