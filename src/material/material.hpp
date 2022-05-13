#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <texture/texture.hpp>

struct Material
{    
    Texture* m_diffuse;
    Texture* m_specular;
    float m_shininess;

    Material(Texture* diffuse, Texture* specular, float shininess);    
};

#endif