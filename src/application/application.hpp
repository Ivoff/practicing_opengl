#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include <stdio.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <scene/scene.hpp>
#include <utils/utils.cpp>
#include <texture/texture.hpp>
#include <application/mouse/mouse.hpp>
#include <application/keyboard/keyboard.hpp>
#include <application/window/window.hpp>

struct APP {
    bool is_running;
    const char* title;
    int frame_target;
    unsigned int last_frame_time;
    unsigned int min_fps_time;
    unsigned int fps;
};

class Application 
{
private:
    
public:    
    APP m_info;
    Window* m_window;
    Scene m_scene;
    Mouse* m_mouse;
    Keyboard* m_keyboard;
    ImGuiIO* m_imgui_io;

    Application(int width, int height, const char* title, int frame_target);
    void m_pre_update();
    void m_pre_process_input();
    ~Application();    
    void m_setup();
    void m_render();
    void m_update(float delta_time);    
    static void m_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void m_MouseCallback(GLFWwindow* window, double x, double y);
    static void m_MouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void m_WindowResizeCallback(GLFWwindow* window, int width, int height);    
    void m_destroy();
};

#endif