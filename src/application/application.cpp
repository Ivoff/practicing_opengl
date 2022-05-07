#include <application/application.hpp>

Application::Application(int width, int height, const char* title, int frame_target):
    m_info {false, title, frame_target, 0.0, NULL, 0},
    m_window {width, height, {0}, nullptr}
{
    if(!glfwInit()) {
            std::cerr << "Error initializing GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }        

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window.window = glfwCreateWindow(m_window.width, m_window.height, m_info.title, NULL, NULL);        
        if (!m_window.window) {
            std::cerr << "Error during window creation" << std::endl;
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }        

        glfwMakeContextCurrent(m_window.window);                        

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
            std::cerr << "Error while loading OpenGL functions";
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }                

        glfwGetFramebufferSize(m_window.window, &m_window.width, &m_window.height);
        glViewport(0, 0, m_window.width, m_window.height);
        
        glfwSetFramebufferSizeCallback(m_window.window, [](GLFWwindow *window, int width, int height){
            glViewport(0, 0, width, height);
        });

        if (m_info.frame_target != 0) {
            m_info.min_fps_time = 1000 / m_info.frame_target;
        }

        m_scene = Scene();

        m_info.is_running = true;
}

void Application::m_pre_update()
{
    unsigned int available_time = m_info.min_fps_time - (Utils::ticks() - m_info.last_frame_time);

    // precisa ser <= aqui porque se a funcao de update for simples e nao demorar passa direto
    if (available_time > 0 && available_time <= m_info.min_fps_time)
    {        
        std::this_thread::sleep_for(std::chrono::milliseconds(available_time));
    }    

    float current_delta = Utils::ticks() - m_info.last_frame_time;
    current_delta = (current_delta > 0.05f) ? 0.05f : current_delta;
            
    m_update(current_delta);

    m_info.last_frame_time = Utils::ticks();
}

void Application::m_pre_process_input()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(m_window.window))
        m_info.is_running = false;
    else
        m_process_input();
}

Application::~Application() {
    m_destroy();
    glfwTerminate();        
}

void Application::m_setup()
{
    glGenVertexArrays(1, &m_scene.current_vao);    
    glGenBuffers(1, &m_scene.current_vbo);

    glBindVertexArray(m_scene.current_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_scene.current_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_scene.vertex_data), m_scene.vertex_data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_scene.camera = new Camera(glm::radians(90.0f), 0.1f, 100.0f, m_window.width, m_window.height);
    m_scene.model_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 2));

    GLuint vertex_shader = Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = Shader::m_create("shaders/fragment.frag", GL_FRAGMENT_SHADER);     
    m_scene.current_program = new ShaderProgram({vertex_shader, fragment_shader});
    m_scene.current_program->m_use();
    m_scene.current_program->m_setUniform("model_mat", m_scene.model_mat);    
    m_scene.current_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.current_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);    
}

void Application::m_update(float delta_time)
{
    m_scene.model_mat = glm::rotate(m_scene.model_mat, glm::radians(1.0f), glm::vec3(0, 1, 0));
    m_scene.current_program->m_setUniform("model_mat", m_scene.model_mat);
    m_scene.current_program->m_setUniform("intensity", fabs(cos((float)glfwGetTime())));
}

void Application::m_render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::m_process_input()
{
    if(glfwGetKey(m_window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {        
        m_info.is_running = false;
        glfwSetWindowShouldClose(m_window.window, true);
    }
    if (glfwGetKey(m_window.window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        if ((bool)m_window.toggle['z'] == false)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            m_window.toggle['z'] = true;
        }            
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            m_window.toggle['z'] = false;
        }
    }
}

void Application::m_destroy()
{
    m_scene.destroy();
}