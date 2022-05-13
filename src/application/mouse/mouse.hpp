#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>
#include <camera/camera.hpp>
#include <unordered_map>
#include <string>

struct Mouse
{    
    float m_last_x;
    float m_last_y;
    float m_cur_x;
    float m_cur_y;    
    float m_sensitivity;
    bool m_first {true};
    std::unordered_map<std::string, bool> m_toggle {
        {"left", false},
        {"right", false}
    };
    std::unordered_map<std::string, bool> m_pressed {
        {"left", false},
        {"right", false}
    };

    Mouse(int window_width, int window_height, float sensitivity);
    void static m_Input(Mouse& mouse, Camera& camera, double x, double y);
    void static m_Input(Mouse& mouse, GLFWwindow* window, int button, int action, int mods);
};

#endif