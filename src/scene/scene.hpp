#ifndef SCENE_H
#define SCENE_H

#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <texture/texture.hpp>
#include <vector>
#include <unordered_map>

struct Scene
{
    ShaderProgram *current_program;
    GLuint current_vao;
    GLuint current_ebo;
    GLuint current_vbo;
    Camera* camera;
    Texture* texture;
    glm::mat4 model_mat;
    glm::mat4 world_mat;

    // CUBE
    float vertex_data[216] {
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f, 1.0f
    };

    int index_data[36] {
        // FRONT-FACE
        0, 1, 2,
        1, 2, 3,
        // RIGHT-FACE
        3, 2, 6,
        6, 7, 3,
        // BACK-FACE
        4, 5, 6,
        6, 7, 4,
        // LEFT-FACE
        0, 1, 5,
        5, 4, 0,
        // TOP-FACE
        4, 0, 3,
        3, 7, 4,
        // BOTTOM-FACE
        5, 1, 2,
        2, 6, 5
    };
    
    void destroy();
};

#endif