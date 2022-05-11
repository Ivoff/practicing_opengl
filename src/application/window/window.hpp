#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

struct Window
{
    int m_width;
    int m_height;
    GLFWwindow* m_glfw_window;

    Window(GLFWwindow* window, int width, int height);
    void m_Resize(Window& window, int width, int height);
};

#endif