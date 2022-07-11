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
    glEnable(GL_CULL_FACE);

    m_scene.SetupCamera(m_window->m_width, m_window->m_height);
    m_scene.SetupPointLight();
    m_scene.SetupDirLight();

    m_scene.directional_active = true;
    m_scene.map_type = 1;
    m_scene.shadow_map_bias = 0.005f;
    m_scene.omnidirectional_shadow_bias = 0.005f;

    m_scene.LoadModels();

    m_scene.SetupIlluminationProgram();
    m_scene.IlluminationProgramUniforms();    
    m_scene.programs["current_program"] = m_scene.programs["illumination_program"];
        
    m_scene.SetupLightlessProgram();
    m_scene.LightlessProgramUniforms();
                
    m_scene.ShadowFramebufferSetup();
    m_scene.DirLightCameraSetup();
    m_scene.SetupShadowProgram();
    m_scene.ShadowProgramUniforms();
    m_scene.ShadowThumbnailSetup();

    m_scene.VoxelMapSetup();
}

void Application::m_update(float delta_time)
{
    m_scene.CurrentCameraUpdate(m_keyboard, delta_time);

    m_scene.CurrentProgramUpdate();

    m_scene.DirLightUpdate();

    m_scene.IlluminationProgramUniforms();

    m_scene.LightlessProgramUniforms();

    m_scene.ShadowProgramUniforms();

    m_scene.VoxelMapUpdate();

    m_scene.UpdateModels();
}

void Application::m_render()
{
    m_imgui->m_NewFrame();

    m_scene.SceneGui(m_mouse);

    m_scene.VoxelMapGui();

    m_scene.RenderShadowFramebuffer();

    // m_scene.RenderShadowThumbnailFramebuffer();

    m_scene.VoxelMapRender();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, m_window->m_width, m_window->m_height);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    m_scene.models["sponza"]->m_Draw(m_scene.programs["current_program"]);
    for (auto i = m_scene.models.begin(); i != m_scene.models.end(); i++)
    {
        if (i->second->m_dont_render == false)
        {
            if (i->second->m_shader == nullptr)
                i->second->m_Draw(m_scene.programs["current_program"]);
            else
                i->second->m_Draw(i->second->m_shader);
        }
    }

    m_imgui->m_Render();
}

void Application::m_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    
    if (!app.m_imgui->m_io->WantCaptureKeyboard)
    {
        app.m_keyboard->m_Input(*(app.m_keyboard), key, scancode, action, mods);
    }    
}

// Mouse movement
void Application::m_MouseCallback(GLFWwindow* window, double x, double y)
{    
    Application& app = *((Application*) glfwGetWindowUserPointer(window));
    if (!app.m_imgui->m_io->WantCaptureMouse && app.m_mouse->m_pressed["right"])
    {
        app.m_mouse->m_Input(*(app.m_mouse), *(app.m_scene.cameras["current_camera"]), x, y);
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

    app->m_scene.cameras["current_camera"]->m_UpdateProjMat(
        app->m_scene.cameras["current_camera"]->m_fov, 
        app->m_scene.cameras["current_camera"]->m_near, 
        app->m_scene.cameras["current_camera"]->m_far, 
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