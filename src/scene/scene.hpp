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
#include <framebuffer/framebuffer.hpp>
#include <application/keyboard/keyboard.hpp>
#include <application/imgui_wrapper/imgui_wrapper.hpp>
#include <application/mouse/mouse.hpp>
#include <voxel_map/voxel_map.hpp>
#include <stdlib.h>
#include <time.h>


struct Scene
{
    std::unordered_map<std::string, Camera*> cameras;
    std::unordered_map<std::string, ShaderProgram*> programs;
    std::unordered_map<std::string, DirectionalLight*> dir_lights;
    std::unordered_map<std::string, PointLight*> point_lights;
    std::unordered_map<std::string, Model*> models;
    std::unordered_map<std::string, FrameBuffer*> framebuffers;
    std::unordered_map<std::string, Texture*> textures;
    VoxelMap* voxelmap;

    float scale;
    bool directional_active;
    int map_type;
    int current_camera_index{0};    
    float shadow_map_bias;
    bool shadow_enable{false};
    bool omnidirectional_shadow_enable{false};
    float omnidirectional_shadow_bias;
    int test_type{0};
    int curr_frame{0};

    Camera* GetCurrentCamera();
    
    void SetupShadowProgram();
    void ShadowFramebufferSetup();
    void ShadowThumbnailSetup();
    void ShadowProgramUniforms();

    void SetupDirLight();
    void DirLightCameraSetup();
    void CameraDirLightUpdate();
    void DirLightUpdate();

    void SetupPointLight();

    void SetupIlluminationProgram();
    void IlluminationProgramUniforms();

    void SetupLightlessProgram();
    void LightlessProgramUniforms();    

    void SetupLampProgram();
    void LampProgramUniforms();
    void LampUpdate();
    void PointLightUpdate();

    void LoadModels();
    void UpdateModels();

    void RenderShadowFramebuffer();
    void RenderShadowThumbnailFramebuffer();

    void CurrentProgramUpdate();

    void CurrentCameraUpdate(Keyboard* keyboard, float delta_time);

    void SceneGui(Mouse* mouse);

    void SetupCamera(int width, int height); 

    void CubemapProgramSetup();
    void CubemapProgramUniforms();
    void CubemapProgramRender(int window_width, int window_height);
    
    void VoxelMapSetup();
    void VoxelMapUpdate();
    void VoxelMapRender();
    void VoxelMapGui();

    void destroy();
};

#endif