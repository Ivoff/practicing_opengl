#ifndef IMGUI_WRAPPER_HPP
#define IMGUI_WRAPPER_HPP

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <string>

struct ImGuiWrapper
{
    ImGuiIO* m_io;
    ImGuiContext* m_context;

    ImGuiWrapper(GLFWwindow* glfw_window, std::string opengl_version);

    void m_NewFrame();
    void m_Render();   
    void m_Destroy();
};

#endif