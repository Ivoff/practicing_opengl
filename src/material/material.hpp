#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

struct Material
{
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};

#endif