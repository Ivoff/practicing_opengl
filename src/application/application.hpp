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


struct WINDOW {
    int width;
    int height;
    int toggle[256];
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

    Application(int width, int height, const char* title, int frame_target);
    void m_pre_update();
    void m_pre_process_input();
    ~Application();    
    void m_setup();
    void m_render();
    void m_update(float delta_time);
    void m_process_input();
    void m_destroy();
};

#endif