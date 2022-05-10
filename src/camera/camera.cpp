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

    m_yaw = -90.0f; // para garantir que a camera comece apontada na direção do z negativo
    m_pitch = 0.0f;
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

    m_yaw = 45.0f; // para garantir que a camera comece apontada na direção do z negativo
    m_pitch = 0.0f;
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

void Camera::m_front_dir(float yaw, float pitch)
{
    m_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front.y = sin(glm::radians(pitch));
    m_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    m_front = glm::normalize(m_front);
}

void Camera::m_look_at(glm::vec3 direction)
{
    glm::vec3 right_vec = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
    m_up = glm::normalize(glm::cross(right_vec, direction));
    
    m_view_mat = glm::lookAt(m_position, m_position+direction, m_up);
}