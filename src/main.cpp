#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_events(GLFWwindow* window)
{        
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {        
        glfwSetWindowShouldClose(window, true);
    }
}

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (scancode == 9 && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }    
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Erro ao criar janela\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Erro ao carregar funções opengl\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, keycallback); // usando isso aqui porque esc retorna GLFW_KEY_UNKNOWN

    while(!glfwWindowShouldClose(window))
    {
        process_events(window);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);        
        glfwPollEvents(); // chama as rotinas que lidam com eventos na janela, no caso aqui interações do usuário com a janela
    }

    glfwTerminate();    

    return 0;
}