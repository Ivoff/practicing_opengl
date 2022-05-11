#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include <stdio.h>

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

struct KEY {
    int toggle[256];
    int pressed[256];
};

struct WINDOW {
    int width;
    int height;
    KEY key;
    GLFWwindow* window;    
};

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
    WINDOW m_window;
    Scene m_scene;
    Mouse* m_mouse;
    Keyboard* m_keyboard;

    Application(int width, int height, const char* title, int frame_target);
    void m_pre_update();
    void m_pre_process_input();
    ~Application();    
    void m_setup();
    void m_render();
    void m_update(float delta_time);    
    static void m_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void m_MouseCallback(GLFWwindow* window, double x, double y);
    static void m_window_resize(GLFWwindow* window, int width, int height);    
    void m_destroy();
};

#endif