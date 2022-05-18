#ifndef SCENE_H
#define SCENE_H

#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <texture/texture.hpp>
#include <vector>
#include <unordered_map>
#include <material/material.hpp>
#include <light/light.hpp>
#include <light/directional/light_directional.hpp>
#include <light/point/light_point.hpp>

struct Scene
{
    ShaderProgram* current_program;
    ShaderProgram* lamp_program;
    GLuint current_vao;
    GLuint current_ebo;
    GLuint current_vbo;    
    Camera* camera;
    Texture* texture;
    glm::mat4 model_mat;
    glm::mat4 lamp_model_mat;    
    Material* material;
    GLuint lamp_vao;
    DirectionalLight* directional_light;
    PointLight* light;

    // CUBE
    float vertex_data[324] {
        // vertex_pos                 // uv coords   // normals
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,    1.0f, 0.0f,     0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,    1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,    1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
     
        -0.5f, -0.5f,  0.5f, 1.0f,    0.0f, 0.0f,     0.0f, 0.0f,  1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     0.0f, 0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 1.0f,     0.0f, 0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 1.0f,     0.0f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,    0.0f, 1.0f,     0.0f, 0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,    0.0f, 0.0f,     0.0f, 0.0f,  1.0f,
    
        -0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,    1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,    0.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
    
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,    1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,    0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
    
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,    1.0f, 1.0f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,    0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     0.0f, -1.0f,  0.0f,
    
        -0.5f,  0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,    1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,    1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,    0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,    0.0f, 1.0f,     0.0f,  1.0f,  0.0f
    };
    
    void destroy();
};

#endif