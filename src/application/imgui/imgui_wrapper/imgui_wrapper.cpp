#include <application/imgui/imgui_wrapper/imgui_wrapper.hpp>

ImGuiWrapper::ImGuiWrapper(GLFWwindow* glfw_window, std::string opengl_version)
{
    IMGUI_CHECKVERSION();
    
    m_context = ImGui::CreateContext();
    
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init(opengl_version.c_str());
}

void ImGuiWrapper::m_NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::m_Render()
{    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::m_Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();    
    ImGui::DestroyContext();
}