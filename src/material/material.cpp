#include <material/material.hpp>

Material::Material(Texture* diffuse, Texture* specular, float shininess)
{    
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}