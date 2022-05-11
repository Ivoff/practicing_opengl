#include <application/window/window.hpp>

Window::Window(GLFWwindow* window, int width, int height)
{
    m_width = width;
    m_height = height;
    m_glfw_window = window;
}

void Window::m_Resize(Window& window, int width, int height)
{
    
}