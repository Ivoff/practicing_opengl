#include <camera/camera.hpp>

Camera::Camera(glm::vec3 position, float fov, float near, float far, int width, int height):
    m_position {position},
    m_fov {fov},
    m_near {near},
    m_far {far},
    m_width {width},
    m_height {height}
{
    m_view_mat = glm::mat4x4(1.0f);
    m_view_mat[2][2] = -1.0f;
    m_view_mat = glm::translate(m_view_mat, m_position);

    m_proj_mat = glm::perspective(m_fov, (m_width*1.0f) / (m_height*1.0f), m_near, m_far);
};

Camera::Camera(float fov, float near, float far, int width, int height):
    m_fov {fov},
    m_near {near},
    m_far {far},
    m_width {width},
    m_height {height}
{
    m_view_mat = glm::mat4x4(1.0f);
    m_view_mat[2][2] = -1.0f;

    m_proj_mat = glm::perspective(m_fov, (m_width*1.0f) / (m_height*1.0f), m_near, m_far);
}

void Camera::m_update(float fov, float near, float far, int width, int height)
{
    m_fov = fov;
    m_near = near;
    m_far = far;
    m_width = width;
    m_height = height;

    m_proj_mat = glm::perspective(m_fov, (m_width*1.0f) / (m_height*1.0f), m_near, m_far);
}