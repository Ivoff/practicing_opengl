#ifndef LIGHT_POINT_H
#define LIGHT_POINT_H

#include <light/light.hpp>

struct PointLight : Light
{
    float m_constant;
    float m_linear;
    float m_quadratic;

    PointLight(
        glm::vec3 position, 
        glm::vec3 ambient, 
        glm::vec3 diffuse, 
        glm::vec3 specular, 
        float constant, 
        float linear, 
        float quadratic
    );    
};

#endif