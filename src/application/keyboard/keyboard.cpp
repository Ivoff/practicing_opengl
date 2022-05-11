#include <application/keyboard/keyboard.hpp>

std::unordered_map<char, char> Keyboard::m_key_map = {
    {9, 27} // ESC
};

Keyboard::Keyboard(GLFWwindow* window)
{
    m_window = window;
}

void Keyboard::m_Input(Keyboard& keyboard, int key, int scancode, int action, int mods)
{    
    GLFWwindow* window = keyboard.m_window;

    if(scancode ==  9 && action == GLFW_PRESS && !keyboard.m_Toggle(keyboard.m_ScanToAsciiCode(scancode)))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        keyboard.m_toggle[keyboard.m_ScanToAsciiCode(scancode)] = true;
    }
    else if (scancode ==  9 && action == GLFW_PRESS && keyboard.m_Toggle(keyboard.m_ScanToAsciiCode(scancode)))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        keyboard.m_toggle[keyboard.m_ScanToAsciiCode(scancode)] = false;
    }

    // =========================================================================================================================================================   
    if ((key == GLFW_KEY_Z && action == GLFW_PRESS) && !keyboard.m_Toggle('z'))
    {        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        keyboard.m_toggle['z'] = true;
    }
    else if ((key == GLFW_KEY_Z && action == GLFW_PRESS) && keyboard.m_Toggle('z'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        keyboard.m_toggle['z'] = false;
    }            

    // =========================================================================================================================================================    
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        keyboard.m_pressed['w'] = true;
    } 
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        keyboard.m_pressed['w'] = false;
    }

    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        keyboard.m_pressed['s'] = true;
    } 
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        keyboard.m_pressed['s'] = false;
    }

    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        keyboard.m_pressed['a'] = true;
    } 
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        keyboard.m_pressed['a'] = false;
    }

    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        keyboard.m_pressed['d'] = true;
    } 
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        keyboard.m_pressed['d'] = false;
    }   
}

bool Keyboard::m_Toggle(char input)
{
    return m_toggle[input];
}

bool Keyboard::m_Pressed(char input)
{
    return m_pressed[input];
}

char Keyboard::m_ScanToAsciiCode(char input)
{
    if (Keyboard::m_key_map.count(input))
        return Keyboard::m_key_map[input];
    else
    {
        printf("Error while converting scan to ascii code\n");
        std::exit(0);
    }
}