#include <light/point/light_point.hpp>

PointLight::PointLight(
    glm::vec3 position, 
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    float constant, 
    float linear, 
    float quadratic
) : Light(position, ambient, diffuse, specular)
{
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
}