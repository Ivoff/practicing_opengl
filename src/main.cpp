#include <application/application.hpp>

int main()
{
    Application app = Application(1920, 1080, "OpenGL", 60);
    app.m_setup();
    while(app.m_info.is_running)
    {
        app.m_pre_process_input();
        app.m_pre_update();
        app.m_render();

        glfwSwapBuffers(app.m_window->m_glfw_window);
        app.m_info.curr_frame += 1;
    }    
    std::exit(0);
    return 0;
}