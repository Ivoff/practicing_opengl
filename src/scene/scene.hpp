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
#include <model/model.hpp>

struct Scene
{
    ShaderProgram* current_program;    
    Camera* camera;
    glm::mat4 model_mat;
    DirectionalLight* directional_light;
    PointLight* light;
    Model model;
    float scale;
    
    void destroy();
};

#endif