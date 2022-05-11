#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>
#include <camera/camera.hpp>

struct Mouse
{    
    float m_last_x;
    float m_last_y;
    float m_cur_x;
    float m_cur_y;    
    float m_sensitivity;

    Mouse(int window_width, int window_height, float sensitivity);
    void static m_Input(Mouse& mouse, Camera& camera, double x, double y);
};

#endif