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
#include <unordered_map>

struct VoxelMap
{
    glm::ivec3 m_voxelmap_dimensions;
    ShaderProgram* m_voxelmap_program;
    FrameBuffer* m_voxelmap_framebuffer;
    GLuint m_texture_id;
    GLubyte* m_texture_data;
    bool m_voxelmap_generate{false};
    int m_voxelmap_mipmap_levels{7};
    bool indirect_light_active{false};
    bool only_indirect_light_active{false};
    bool only_indirect_light_high_contrast_active{false};
    bool dark_places_help_active{false};
    bool shadow_active{true};
    bool weight_active{false};
    float voxel_initial_offset{1.5f};
    int directions_index{2};
    float voxel_size{1};    
    std::unordered_map<int, int> directions_map{
        {0, 6},
        {1, 9},
        {2, 5},
        {3, 1}
    };

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
    void m_VoxelmapSetRead();

    void m_Gui();

    void m_CameraSetup();
    void m_CameraFramebufferSetup();
    void m_CameraShaderSetup();
    void m_CameraUniforms();
    void m_CameraUpdate();
    void m_CameraRender(std::unordered_map<std::string, Model*>& models);
};

#endif