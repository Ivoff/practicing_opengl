#include <application/application.hpp>

Application::Application(int width, int height, const char* title, int frame_target):
    m_info {
        false, // is_running
        title, // title
        frame_target, // frame_target
        0.0, // last_frame_time
        NULL, // min_fps_time
        0 // fps
    }
{
    if(!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }        

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = new Window(
        glfwCreateWindow(width, height, m_info.title, NULL, NULL),
        width, 
        height
    );
    
    if (!m_window->m_glfw_window) {
        std::cerr << "Error during window creation" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }        

    glfwMakeContextCurrent(m_window->m_glfw_window);                        

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Error while loading OpenGL functions";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }                

    glfwGetFramebufferSize(m_window->m_glfw_window, &m_window->m_width, &m_window->m_height);
    glViewport(0, 0, m_window->m_width, m_window->m_height);
    
    glfwSetFramebufferSizeCallback(m_window->m_glfw_window, [](GLFWwindow *window, int width, int height){
        glViewport(0, 0, width, height);
    });    

    m_keyboard = new Keyboard(m_window->m_glfw_window);
    glfwSetKeyCallback(m_window->m_glfw_window, m_KeyboardCallback);
    
    glfwSetWindowSizeCallback(m_window->m_glfw_window, m_WindowResizeCallback);

    m_mouse = new Mouse(m_window->m_width, m_window->m_height, 0.1f);
    glfwSetInputMode(m_window->m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window->m_glfw_window, m_MouseCallback);

    // glfwSetCursorPos(m_window->m_glfw_window, m_window->m_width/2.0, m_window->m_height/2.0);

    if (m_info.frame_target != 0) {
        m_info.min_fps_time = 1000 / m_info.frame_target;
    }

    m_scene = Scene();

    m_info.is_running = true;

    glfwSetWindowUserPointer(m_window->m_glfw_window, this);
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

    if (glfwWindowShouldClose(m_window->m_glfw_window))
        m_info.is_running = false;
    // else
        // TODO mouse input
}

Application::~Application() {
    m_destroy();
    glfwTerminate();        
}

//============================================================================================================================================================

void Application::m_setup()
{
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &m_scene.current_vao);    
    glGenBuffers(1, &m_scene.current_vbo);
    glGenBuffers(1, &m_scene.current_ebo);

    glBindVertexArray(m_scene.current_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_scene.current_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_scene.vertex_data), m_scene.vertex_data, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scene.current_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_scene.index_data), m_scene.index_data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(4*sizeof(float)));
    glEnableVertexAttribArray(1);

    m_scene.camera = new Camera(glm::radians(90.0f), 0.1f, 100.0f, m_window->m_width, m_window->m_height);
    m_scene.model_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 2));    

    m_scene.texture = new Texture("textures/wall.jpg", GL_TEXTURE_2D);
    m_scene.texture->m_gen_tex(GL_RGB, GL_RGB, true);
    m_scene.texture->m_set_filtering(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_scene.texture->m_set_filtering(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_scene.texture->m_set_wrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_scene.texture->m_set_wrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
    m_scene.texture->m_activate(GL_TEXTURE0);

    GLuint vertex_shader = Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = Shader::m_create("shaders/fragment.frag", GL_FRAGMENT_SHADER);     
    m_scene.current_program = new ShaderProgram({vertex_shader, fragment_shader});
    m_scene.current_program->m_use();
    
    m_scene.current_program->m_setUniform("texture0", m_scene.texture->m_tex_unit);
    m_scene.current_program->m_setUniform("model_mat", m_scene.model_mat);    
    m_scene.current_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.current_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);
}

void Application::m_update(float delta_time)
{
    if (m_keyboard->m_Pressed('w'))
    {
        m_scene.camera->m_position += delta_time * 0.5f * m_scene.camera->m_front;
    }
    if (m_keyboard->m_Pressed('s'))
    {
        m_scene.camera->m_position -= delta_time * 0.5f * m_scene.camera->m_front;
    }
    if (m_keyboard->m_Pressed('a'))
    {
        m_scene.camera->m_position -= delta_time * 0.5f * glm::normalize(glm::cross(m_scene.camera->m_front, m_scene.camera->m_up));
    }
    if (m_keyboard->m_Pressed('d'))
    {
        m_scene.camera->m_position += delta_time * 0.5f * glm::normalize(glm::cross(m_scene.camera->m_front, m_scene.camera->m_up));
    }

    m_scene.camera->m_LookAt(m_scene.camera->m_front);

    m_scene.model_mat = glm::rotate(m_scene.model_mat, glm::radians(1.0f), glm::vec3(0, 1, 0));
    m_scene.current_program->m_setUniform("model_mat", m_scene.model_mat);    
    m_scene.current_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.current_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);
    m_scene.current_program->m_setUniform("intensity", fabs(cos((float)glfwGetTime())));    
}

void Application::m_render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Application::m_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    app.m_keyboard->m_Input(*(app.m_keyboard), key, scancode, action, mods);
}

void Application::m_MouseCallback(GLFWwindow* window, double x, double y)
{
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    app.m_mouse->m_Input(*(app.m_mouse), *(app.m_scene.camera), x, y);
}

void Application::m_WindowResizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    
    app->m_scene.camera->m_UpdateProjMat(glm::radians(90.0f), 0.1f, 100, width, height);
}

void Application::m_destroy()
{
    m_scene.destroy();
}