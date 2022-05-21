#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <texture/texture.hpp>
#include <string>

struct Material
{    
    std::string m_name;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;

    Material(glm::vec3 diffuse, glm::vec3 specular, float shininess);
    Material();
};

#endif