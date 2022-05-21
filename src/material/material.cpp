#include <material/material.hpp>

Material::Material(glm::vec3 diffuse, glm::vec3 specular, float shininess)
{    
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}

Material::Material()
{    
    m_diffuse = glm::vec3(0.0f);
    m_specular = glm::vec3(0.0f);
    m_shininess = 0.0f;
}