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
    glfwSetCursorPosCallback(m_window->m_glfw_window, m_MouseCallback);
    glfwSetMouseButtonCallback(m_window->m_glfw_window, m_MouseCallback);

    m_imgui = new ImGuiWrapper(m_window->m_glfw_window, "#version 460");

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
}

//============================================================================================================================================================

void Application::m_setup()
{
    glEnable(GL_DEPTH_TEST);    

    m_scene.camera = new Camera(90.0f, 0.1f, 100.0f, m_window->m_width, m_window->m_height);    
    m_scene.light = new PointLight(
        glm::vec3(-2.0f, 2.0f, -2.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f
    );    
    m_scene.directional_light = new DirectionalLight(
        glm::vec3(-1.0f, -1.0f, -0.5f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.60f, 0.60f, 0.60f),
        glm::vec3(0.85f, 0.85f, 0.85f) 
    );
    m_scene.directional_active = true;

    GLuint vertex_shader = Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = Shader::m_create("shaders/fragment.frag", GL_FRAGMENT_SHADER);     
    m_scene.current_program = new ShaderProgram({vertex_shader, fragment_shader});
    m_scene.current_program->m_selected = true;
    m_scene.current_program->m_use();
    
    m_scene.model.m_model_mat = glm::scale(m_scene.model.m_model_mat, glm::vec3(0.02));
    m_scene.current_program->m_setUniform("model_mat", m_scene.model.m_model_mat);
    m_scene.current_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.current_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);
    m_scene.current_program->m_setUniform("normal_mat", m_scene.model.m_GetNormalMat());
    m_scene.current_program->m_setUniform("camera_pos", m_scene.camera->m_position);
    
    m_scene.current_program->m_setUniform("dir_light.direction", m_scene.directional_light->m_direction);
    m_scene.current_program->m_setUniform("dir_light.ambient", m_scene.directional_light->m_ambient);
    m_scene.current_program->m_setUniform("dir_light.diffuse", m_scene.directional_light->m_diffuse);
    m_scene.current_program->m_setUniform("dir_light.specular", m_scene.directional_light->m_specular);
    m_scene.current_program->m_setUniform("directional_active", (int)m_scene.directional_active);

    m_scene.current_program->m_setUniform("point_light.position", m_scene.light->m_position);
    m_scene.current_program->m_setUniform("point_light.ambient", m_scene.light->m_ambient);
    m_scene.current_program->m_setUniform("point_light.diffuse", m_scene.light->m_diffuse);
    m_scene.current_program->m_setUniform("point_light.specular", m_scene.light->m_specular);
    m_scene.current_program->m_setUniform("point_light.constant", m_scene.light->m_constant);
    m_scene.current_program->m_setUniform("point_light.linear", m_scene.light->m_linear);
    m_scene.current_program->m_setUniform("point_light.quadratic", m_scene.light->m_quadratic);
    m_scene.illumination_program = m_scene.current_program;

    m_scene.lightless_program = new ShaderProgram({
        vertex_shader,
        Shader::m_create("shaders/lightless_fragment.frag", GL_FRAGMENT_SHADER)
    });
    m_scene.lightless_program->m_use();
    m_scene.lightless_program->m_setUniform("model_mat", m_scene.lamp.m_model_mat);
    m_scene.lightless_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.lightless_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);

    m_scene.lamp_program = new ShaderProgram({
        Shader::m_create("shaders/lamp.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/lamp.frag", GL_FRAGMENT_SHADER)
    });
    m_scene.lamp_program->m_use();
    m_scene.lamp.m_model_mat = glm::translate(m_scene.lamp.m_model_mat, m_scene.light->m_position);
    m_scene.lamp_program->m_setUniform("model_mat", m_scene.lamp.m_model_mat);
    m_scene.lamp_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.lamp_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);    

    m_scene.model.m_LoadModel(PROJECT_ROOT + std::string("models/sponza/sponza.obj"));
    m_scene.lamp.m_LoadModel(PROJECT_ROOT + std::string("models/cube/cube.obj"));
}

void Application::m_update(float delta_time)
{
    if (m_keyboard->m_Pressed('w'))
    {
        m_scene.camera->m_position += delta_time * m_scene.camera->m_speed * m_scene.camera->m_front;
    }
    if (m_keyboard->m_Pressed('s'))
    {
        m_scene.camera->m_position -= delta_time * m_scene.camera->m_speed * m_scene.camera->m_front;
    }
    if (m_keyboard->m_Pressed('a'))
    {
        m_scene.camera->m_position -= delta_time * m_scene.camera->m_speed * glm::normalize(glm::cross(m_scene.camera->m_front, m_scene.camera->m_up));
    }
    if (m_keyboard->m_Pressed('d'))
    {
        m_scene.camera->m_position += delta_time * m_scene.camera->m_speed * glm::normalize(glm::cross(m_scene.camera->m_front, m_scene.camera->m_up));
    }
    if (m_keyboard->m_Pressed(15)) // SHIFT
    {
        m_scene.camera->m_position += delta_time * m_scene.camera->m_speed * glm::vec3(0, 1, 0);
    }
    if (m_keyboard->m_Pressed(7)) // CTRL
    {
        m_scene.camera->m_position += delta_time * (-m_scene.camera->m_speed) * glm::vec3(0, 1, 0);
    }

    m_scene.camera->m_LookAt(m_scene.camera->m_front);

    m_scene.current_program->m_use();        

    glm::mat4 model_mat = glm::scale(
        m_scene.model.m_model_mat, 
        glm::vec3(
            m_scene.model.m_scale, 
            m_scene.model.m_scale, 
            m_scene.model.m_scale
        )
    );
    m_scene.current_program->m_setUniform("model_mat", model_mat);    

    m_scene.camera->m_UpdateProjMat(
        m_scene.camera->m_fov,
        m_scene.camera->m_near,
        m_scene.camera->m_far,
        m_scene.camera->m_width, 
        m_scene.camera->m_height
    );
    m_scene.current_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.current_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);
    
    m_scene.current_program->m_setUniform("camera_pos", m_scene.camera->m_position);
    
    m_scene.current_program->m_setUniform("directional_active", (int)m_scene.directional_active);

    m_scene.current_program->m_setUniform("point_light.position", m_scene.light->m_position);
    m_scene.current_program->m_setUniform("point_light.ambient", m_scene.light->m_ambient);
    m_scene.current_program->m_setUniform("point_light.diffuse", m_scene.light->m_diffuse);
    m_scene.current_program->m_setUniform("point_light.specular", m_scene.light->m_specular);

    m_scene.lamp_program->m_use();
    m_scene.lamp.m_model_mat[3][0] = m_scene.light->m_position.x;
    m_scene.lamp.m_model_mat[3][1] = m_scene.light->m_position.y;
    m_scene.lamp.m_model_mat[3][2] = m_scene.light->m_position.z;
    glm::mat4 lamp_model_mat = glm::scale(m_scene.lamp.m_model_mat, glm::vec3(m_scene.lamp.m_scale));    
    m_scene.lamp_program->m_setUniform("model_mat", lamp_model_mat);
    m_scene.lamp_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.lamp_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);

    m_scene.lightless_program->m_use();
    m_scene.lightless_program->m_setUniform("model_mat", model_mat);
    m_scene.lightless_program->m_setUniform("view_mat", m_scene.camera->m_view_mat);
    m_scene.lightless_program->m_setUniform("proj_mat", m_scene.camera->m_proj_mat);
}

void Application::m_render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    
    m_imgui->m_NewFrame();

    ImGui::Begin("Light Properties");
    ImGui::Checkbox("Directional Light", &m_scene.directional_active);
    ImGui::DragFloat3("Light Position", &m_scene.light->m_position[0], 0.25f, -999.0f, 999.0f, "%.3f");
    ImGui::DragFloat3("Light Ambient", &m_scene.light->m_ambient[0], 0.05f, 0.0f, 1.0f, "%.2f");
    ImGui::DragFloat3("Light Diffuse", &m_scene.light->m_diffuse[0], 0.05f, 0.0f, 1.0f, "%.2f");
    ImGui::DragFloat3("Light Specular", &m_scene.light->m_specular[0], 0.05f, 0.0f, 1.0f, "%.2f");
    ImGui::DragFloat("Light Scale", &m_scene.lamp.m_scale, 0.01f, 0.0f, 10.0f, "%.2f");
    ImGui::End();

    ImGui::Begin("Model Properties");
    ImGui::DragFloat("Model Scale", &m_scene.model.m_scale, 0.001f, 0.0f, 5.0f, "%.3f");
    ImGui::End();

    ImGui::Begin("Camera Properties");
    ImGui::DragFloat("Near Plane", &m_scene.camera->m_near, 0.01f, 0.0f, 100000.0f, "%.2f");
    ImGui::DragFloat("Far Plane", &m_scene.camera->m_far, 10.0f, 0.0f, 100000.0f, "%.2f");
    ImGui::DragFloat("FOV", &m_scene.camera->m_fov, 1.0f, 0.0f, 180.0f, "%.2f");
    ImGui::End();

    ImGui::Begin("Control Properties");    
    ImGui::DragFloat("Move Speed", &m_scene.camera->m_speed, 0.05f, 0.0f, 100.0f, "%.2f");    
    ImGui::DragFloat("Mouse Sensibility", &m_mouse->m_sensitivity, 0.001f, 0.0f, 5.0f, "%.3f");
    ImGui::End();

    ImGui::Begin("Scene Properties");    
    if (ImGui::BeginMenu("Current Shader"))
    {        
        if (ImGui::MenuItem("illumination_program", "", &m_scene.illumination_program->m_selected))
        {
            m_scene.current_program = m_scene.illumination_program;
            m_scene.illumination_program->m_selected = true;
            m_scene.lightless_program->m_selected = false;
        } 
        if (ImGui::MenuItem("lightless_program", "", &m_scene.lightless_program->m_selected))
        {
            m_scene.current_program = m_scene.lightless_program;
            m_scene.lightless_program->m_selected = true;
            m_scene.illumination_program->m_selected = false;
        }        
        ImGui::EndMenu();
    }        
    ImGui::End();

    ImGui::ShowDemoWindow();

    m_scene.model.m_Draw(m_scene.current_program);
    m_scene.lamp.m_Draw(m_scene.lamp_program);

    m_imgui->m_Render();
}

void Application::m_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    app.m_keyboard->m_Input(*(app.m_keyboard), key, scancode, action, mods);
}

// Mouse movement
void Application::m_MouseCallback(GLFWwindow* window, double x, double y)
{    
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    if (!app.m_imgui->m_io->WantCaptureMouse && app.m_mouse->m_pressed["right"])
    {
        app.m_mouse->m_Input(*(app.m_mouse), *(app.m_scene.camera), x, y);
    }        
}

// Mouse button
void Application::m_MouseCallback(GLFWwindow* window, int button, int action, int mods)
{    
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    if (!app.m_imgui->m_io->WantCaptureMouse)
        app.m_mouse->m_Input(*(app.m_mouse), window, button, action, mods);
}

void Application::m_WindowResizeCallback(GLFWwindow* window, int width, int height)
{    
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    
    app->m_window->m_width = width;
    app->m_window->m_height = height;    

    app->m_scene.camera->m_UpdateProjMat(
        app->m_scene.camera->m_fov, 
        app->m_scene.camera->m_near, 
        app->m_scene.camera->m_far, 
        width, 
        height
    );
}

void Application::m_destroy()
{
    m_imgui->m_Destroy();
    m_scene.destroy();
    glfwTerminate();    
}