#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <camera/camera.hpp>
#include <utils/utils.cpp>
#include <GLFW/glfw3.h>

struct Keyboard
{
    GLFWwindow* m_window;
    bool m_toggle[256] {0};
    bool m_pressed[256] {0};
    static std::unordered_map<char, char> m_key_map;

    Keyboard(GLFWwindow* window);
    static void m_Input(Keyboard& keyboard, int key, int scancode, int action, int mods);
    bool m_Toggle(char input);
    bool m_Pressed(char input);
    char m_ScanToAsciiCode(char input);
};

#endif