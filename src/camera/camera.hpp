#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct Camera
{
    float m_near;
    float m_far;
    float m_fov;
    float m_width;
    float m_height;
    glm::vec3 m_position;
    glm::mat4x4 m_view_mat;
    glm::mat4x4 m_proj_mat;

    Camera(glm::vec3 position, float fov, float near, float far, float width, float height);
    Camera(float fov, float near, float far, float width, float height);
};

#endif