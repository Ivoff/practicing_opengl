#ifndef VOXEL_MAP_HPP
#define VOXEL_MAP_HPP

#include <glad/glad.h>
#include <application/imgui_wrapper/imgui_wrapper.hpp>
#include <texture/texture.hpp>
#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <framebuffer/framebuffer.hpp>
#include <shader/shader.hpp>
#include <model/model.hpp>
#include <light/directional/light_directional.hpp>
#include <utils/utils.cpp>
#include <unordered_map>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdlib.h>

struct VoxelMap
{
    GLsizei m_size;
    ShaderProgram* m_voxelmap_program;
    FrameBuffer* m_voxelmap_framebuffer;
    Camera* m_directional_light_camera;
    GLuint m_texture_id;
    GLubyte* m_texture_data;

    Camera* m_camera;
    FrameBuffer* m_camera_framebuffer;
    ShaderProgram* m_camera_program;
    glm::vec4 m_camera_framebuffer_clear_color;

    VoxelMap(int size);
    
    void m_Update();
    void m_Render();    
    void m_Clear();
    void m_Generate(std::unordered_map<std::string, Model*>& models);
    void m_ShaderSetup();
    void m_Uniforms(
        std::unordered_map<std::string, DirectionalLight*>& dir_lights,
        std::unordered_map<std::string, Camera*>& cameras
    );
    void m_VoxelmapSetup();

    void m_Gui();

    void m_CameraSetup();
    void m_CameraFramebufferSetup();
    void m_CameraShaderSetup();
    void m_CameraUniforms();
    void m_CameraUpdate();
    void m_CameraRender(std::unordered_map<std::string, Model*>& models);
};

#endif