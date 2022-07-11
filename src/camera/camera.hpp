#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct Camera
{
    float m_near;
    float m_far;
    float m_fov;
    int m_width;
    int m_height;
    float m_yaw;
    float m_pitch;
    float m_speed;
    float m_ortho_dimensions[4]{0};
    int m_curr_proj_mat{0};
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::mat4x4 m_view_mat;
    glm::mat4x4 m_proj_mat;
    glm::mat4x4 m_ortho_mat;
    
    Camera(float fov, float near, float far, int width, int height);
    void m_SetOrtho(float size);
    void m_UpdateProjMat(float fov, float near, float far, int width, int height);
    void m_SetFrontDir(float yaw, float pitch);
    void m_LookAt(glm::vec3 direction);
    glm::mat4& m_GetProjMat();
    void m_UpdateEulerAngles();
};

#endif