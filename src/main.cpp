#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <camera/camera.hpp>
#include <glm/gtx/string_cast.hpp>
#include <application/application.hpp>
#include <chrono>
#include <thread>

int main()
{
    Application app = Application(800, 600, "OpenGL", 60);
    app.m_setup();
    while(app.m_info.is_running)
    {
        app.m_pre_process_input();
        app.m_pre_update();
        app.m_render();

        glfwSwapBuffers(app.m_window->m_glfw_window);        
    }    

    return 0;
}