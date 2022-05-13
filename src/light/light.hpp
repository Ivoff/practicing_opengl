#ifndef LIGHT_CPP
#define LIGHT_CPP

#include <glm/vec3.hpp>

struct Light
{
    glm::vec3 m_position;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

#endif