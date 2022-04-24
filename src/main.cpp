#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void process_events(GLFWwindow* window)
{        
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {        
        glfwSetWindowShouldClose(window, true);
    }
}

static void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

    static GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
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

    // =======================================================================================================================

    static char* vert_shader_source = "#version 460 core \n"
    "layout (location = 0) in vec3 aPos; \n"
    "void main() { \n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f); \n"
    "}\0";

    static char* frag_shader_source = "#version 460 core \n"
    "out vec4 outColor; \n"
    "void main() { \n"
    "   outColor = vec4(0.1f, 0.3f, 0.2f, 1.0f); \n"
    "}\0";
    

    // static float vertex[] {
    //     -0.5f, -0.5f, 1.0f,
    //     -0.5f, 0.5f, 1.0f,
    //     0.5f, -0.5f, 1.0f,
    //     0.5f, 0.5f, 1.0f
    // };
    
    static float vertex[] {
        -0.75f, -0.5f, 1.0f,
        -0.5f, 0.5f, 1.0f,
        -0.25f, -0.5f, 1.0f,
        0.25f, -0.5f, 1.0f,
        0.5f, 0.5f, 1.0f,
        0.75f, -0.5f, 1.0f
    };

    static unsigned int elements[] {0, 1, 2, 1, 2, 3};

    static unsigned int vao;
    static unsigned int vbo;
    static unsigned int ebo;
    static unsigned int vert_shader_id;
    static unsigned int frag_shader_id;
    static unsigned int program_id;

    static bool is_successful;

    static unsigned int err_log_len;
    static char* err_log_msg = NULL;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);    
    glEnableVertexArrayAttrib(vao, 0);

    vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader_id, 1, &vert_shader_source, NULL);
    glCompileShader(vert_shader_id);

    glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, (int*)&is_successful);    
    if (!is_successful)
    {
        glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, (int*)&err_log_len);
        err_log_msg = (char*)malloc(sizeof(char)*err_log_len);
        glGetShaderInfoLog(vert_shader_id, err_log_len, NULL, err_log_msg);
        printf("Shader Compile Error: Vertex Shader\n%s\n", err_log_msg);
        std::exit(EXIT_FAILURE);
    }

    frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader_id, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader_id);

    glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, (int*)&is_successful);
    if (!is_successful)
    {
        if (err_log_msg != NULL)
        {
            free(err_log_msg);
        }

        glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, (int*)&err_log_len);                
        err_log_msg = (char*)malloc(sizeof(char)*err_log_len);
        glGetShaderInfoLog(frag_shader_id, err_log_len, NULL, err_log_msg);
        printf("Shader Compile Error: Fragment Shader\n%s\n", err_log_msg);
        std::exit(EXIT_FAILURE);
    }

    program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

    glGetShaderiv(program_id, GL_LINK_STATUS, (int*)&is_successful);
    if (!is_successful)
    {
        if (err_log_msg != NULL)
        {
            free(err_log_msg);
        }

        glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, (int*)&err_log_len);
        err_log_msg = (char*)malloc(sizeof(char)*err_log_len);
        glGetShaderInfoLog(program_id, err_log_len, NULL, err_log_msg);
        printf("Shader Link Error:\n%s\n", err_log_msg);
        std::exit(EXIT_FAILURE);
    }

    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);

    glUseProgram(program_id);
    

    while(!glfwWindowShouldClose(window))
    {
        process_events(window);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);        
        glfwPollEvents(); // chama as rotinas que lidam com eventos na janela, no caso aqui interações do usuário com a janela
    }

    glfwTerminate();    

    return 0;
}