#include <light/directional/light_directional.hpp>

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(glm::vec3(0.0f, 0.0f, 0.0f), ambient, diffuse, specular)
{
    m_direction = direction;
}