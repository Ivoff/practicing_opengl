#ifndef SCENE_H
#define SCENE_H

#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <vector>
#include <unordered_map>

struct Scene
{
    ShaderProgram *current_program;
    GLuint current_vao;
    GLuint current_ebo;
    GLuint current_vbo;
    Camera* camera;
    glm::mat4 model_mat;    

    float vertex_data[12]{
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f
    };
    
    void destroy();
};

#endif