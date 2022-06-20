#include <application/mouse/mouse.hpp>

Mouse::Mouse(int window_width, int window_height, float sensitivity)
{
    m_last_x = window_width/2.0f;
    m_last_y = window_height/2.0f;
    m_sensitivity = sensitivity;
}

void Mouse::m_Input(Mouse& mouse, Camera& camera, double x, double y)
{    
    mouse.m_cur_x = static_cast<float>(x);
    mouse.m_cur_y = static_cast<float>(y);

    if (mouse.m_first)
    {
        mouse.m_last_x = mouse.m_cur_x;
        mouse.m_last_y = mouse.m_cur_y;
        mouse.m_first = false;
    }

    camera.m_yaw += (mouse.m_cur_x - mouse.m_last_x) * mouse.m_sensitivity;
    camera.m_pitch += (mouse.m_last_y - mouse.m_cur_y) * mouse.m_sensitivity;

    mouse.m_last_x = mouse.m_cur_x;
    mouse.m_last_y = mouse.m_cur_y;
    
    if (camera.m_pitch > 89.0f)
        camera.m_pitch =  89.0f;
    
    if (camera.m_pitch < -89.0f)
        camera.m_pitch = -89.0f;

    camera.m_SetFrontDir(camera.m_yaw, camera.m_pitch);
    camera.m_LookAt(camera.m_front);
}

void Mouse::m_Input(Mouse& mouse, GLFWwindow* window, int button, int action, int mods)
{    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        mouse.m_pressed["right"] = true;        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        mouse.m_pressed["right"] = false;
        mouse.m_first = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);        
    }
}