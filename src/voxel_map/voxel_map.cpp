#include <voxel_map/voxel_map.hpp>

VoxelMap::VoxelMap(int size)
{
    m_voxelmap_dimensions.x = size;
    m_voxelmap_dimensions.y = size;
    m_voxelmap_dimensions.z = size;

    GL_STMT(glGenTextures(1, &m_texture_id));
    GL_STMT(glBindTexture(GL_TEXTURE_3D, m_texture_id));
    
    GL_STMT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_STMT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_STMT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    
    GL_STMT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
    GL_STMT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);    
    
    // GL_STMT(glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, m_size, m_size, m_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture_data));
    GL_STMT(glTexStorage3D(
        GL_TEXTURE_3D, 
        m_voxelmap_mipmap_levels, 
        GL_RGBA8, 
        m_voxelmap_dimensions.x, 
        m_voxelmap_dimensions.y, 
        m_voxelmap_dimensions.z
    ));
    GL_STMT(glBindImageTexture(0, m_texture_id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8));

    m_texture_data = (GLubyte*) calloc(size * size * size * 4, sizeof(GLubyte));

    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_3D, m_texture_id);

    m_ShaderSetup();
    m_VoxelmapSetup();
    m_CameraSetup();
    m_CameraFramebufferSetup();
    m_CameraShaderSetup();
    
    // m_Clear();
}

void VoxelMap::m_Clear()
{
    glBindTexture(GL_TEXTURE_3D, m_texture_id);
    
    int offset = 0;
    int size = m_voxelmap_dimensions.x * m_voxelmap_dimensions.y * m_voxelmap_dimensions.z * 4;
    
    for(int i = 0; i < size; i += 1)
    {
        m_texture_data[i] = 0;
    }

    GL_STMT(glTexSubImage3D(
        GL_TEXTURE_3D, 
        0, 
        offset, 
        offset, 
        offset, 
        m_voxelmap_dimensions.x, 
        m_voxelmap_dimensions.y, 
        m_voxelmap_dimensions.z, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        m_texture_data
    ));
    
    glBindTexture(GL_TEXTURE_3D, 0);
}

void VoxelMap::m_ShaderSetup()
{
    GL_STMT(m_voxelmap_program = new ShaderProgram({
        Shader::m_create("shaders/voxel_map_gen/voxel_map_gen.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/voxel_map_gen/voxel_map_gen.geom", GL_GEOMETRY_SHADER),
        Shader::m_create("shaders/voxel_map_gen/voxel_map_gen.frag", GL_FRAGMENT_SHADER)
    }));
}

void VoxelMap::m_Uniforms(
    std::unordered_map<std::string, DirectionalLight*>& dir_lights, 
    std::unordered_map<std::string, Camera*>& cameras
)
{
    m_voxelmap_program->m_use();
    m_voxelmap_program->m_setUniform("view_mat", m_camera->m_view_mat);
    m_voxelmap_program->m_setUniform("proj_mat", m_camera->m_ortho_mat);

    // m_voxelmap_program->m_setUniform("view_mat", cameras["perspective_cam"]->m_view_mat);
    // m_voxelmap_program->m_setUniform("proj_mat", cameras["perspective_cam"]->m_proj_mat);

    m_voxelmap_program->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    m_voxelmap_program->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);

    m_voxelmap_program->m_setUniform("dir_light.direction", dir_lights["dir_light"]->m_direction);
    m_voxelmap_program->m_setUniform("dir_light.ambient", dir_lights["dir_light"]->m_ambient);
    m_voxelmap_program->m_setUniform("dir_light.diffuse", dir_lights["dir_light"]->m_diffuse);
    m_voxelmap_program->m_setUniform("dir_light.specular", dir_lights["dir_light"]->m_specular);

    m_voxelmap_program->m_setUniform("directional_shadow_map", 13);
    m_voxelmap_program->m_setUniform("voxel_map", 0);
    m_voxelmap_program->m_setUniform("shadow_map_bias", 0.005f);
    m_voxelmap_program->m_setUniform("shadow_active", shadow_active);
}

void VoxelMap::m_VoxelmapSetup()
{
    m_voxelmap_framebuffer = new FrameBuffer(m_voxelmap_dimensions.x, m_voxelmap_dimensions.y);
    Texture color_tex = Texture(GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE, m_voxelmap_dimensions.x, m_voxelmap_dimensions.y, NULL);
    Texture depth_tex = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_FLOAT, m_voxelmap_dimensions.x, m_voxelmap_dimensions.y, NULL);
    m_voxelmap_framebuffer->m_Bind();
    m_voxelmap_framebuffer->m_AttachColor(color_tex, "color_attach");
    m_voxelmap_framebuffer->m_AttachDepth(depth_tex);
    m_voxelmap_framebuffer->m_Check();
    m_voxelmap_framebuffer->m_Unbind();
}

void VoxelMap::m_Generate(std::unordered_map<std::string, Model*>& models)
{
    m_voxelmap_framebuffer->m_Bind();

    GL_STMT(glViewport(0, 0, m_voxelmap_dimensions.x, m_voxelmap_dimensions.y));
    GL_STMT(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
    GL_STMT(glDisable(GL_CULL_FACE));
    GL_STMT(glDisable(GL_DEPTH_TEST));
	GL_STMT(glDisable(GL_BLEND));
    GL_STMT(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_STMT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    GL_STMT(glBindImageTexture(0, m_texture_id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8));
    models["sponza"]->m_Draw(m_voxelmap_program);
    
    GL_STMT(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
	GL_STMT(glEnable(GL_BLEND));
    GL_STMT(glEnable(GL_DEPTH_TEST));

    GL_STMT(glBindTexture(GL_TEXTURE_3D, m_texture_id));

    // m_Clear();

    GLint width = 0;
    GLint height = 0;
    GLint depth = 0;
    GL_STMT(glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width));
    GL_STMT(glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &height));
    GL_STMT(glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &depth));
    
    free(m_texture_data);
    m_texture_data = (GLubyte*) calloc(width * height * depth * 4, sizeof(GLubyte));
        
    GL_STMT(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
    GL_STMT(glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture_data));
    glGenerateMipmap(GL_TEXTURE_3D);

    m_voxelmap_framebuffer->m_Unbind();
}

void VoxelMap::m_VoxelmapSetRead()
{
    GL_STMT(glBindImageTexture(0, m_texture_id, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8));
}

void VoxelMap::m_CameraSetup()
{
    m_camera = new Camera(90.0f, 0.1f, 45.0f, 300, 300);
    m_camera->m_SetOrtho(37.0f);
    m_camera->m_position = glm::vec3(-1.0f, 18.0f, -25.0f);
    
    glm::vec3 direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    m_camera->m_front = direction;
    m_camera->m_UpdateEulerAngles();

    m_camera->m_LookAt(direction);
}

void VoxelMap::m_CameraFramebufferSetup()
{
    m_camera_framebuffer = new FrameBuffer(m_camera->m_width, m_camera->m_height);
 
    Texture* color_attach = new Texture(GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE, m_camera_framebuffer->m_width, m_camera_framebuffer->m_width, NULL);
    color_attach->m_SetFiltering(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    color_attach->m_SetFiltering(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    color_attach->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    color_attach->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    Texture* depth_attach = new Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_FLOAT, m_camera_framebuffer->m_width, m_camera_framebuffer->m_width, NULL);

    m_camera_framebuffer->m_Bind();
    m_camera_framebuffer->m_AttachColor(*color_attach, "color_attach");
    m_camera_framebuffer->m_AttachDepth(*depth_attach);
    m_camera_framebuffer->m_Check();
    m_camera_framebuffer->m_Unbind();

    m_camera_framebuffer_clear_color = glm::vec4(0.951f, 0.956f, 0.005f, 0.000f);
}

void VoxelMap::m_CameraShaderSetup()
{
    // reutilizando os shaders para a thumbnail do directional shadow map
    m_camera_program = new ShaderProgram({
        Shader::m_create("shaders/shadow/shadow.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/shadow/shadow.frag", GL_FRAGMENT_SHADER)
    });
    m_CameraUniforms();
}

void VoxelMap::m_CameraUniforms()
{
    m_camera_program->m_use();    
    m_camera_program->m_setUniform("view_mat", m_camera->m_view_mat);
    m_camera_program->m_setUniform("proj_mat", m_camera->m_ortho_mat);
}

void VoxelMap::m_CameraUpdate()
{
    m_camera->m_UpdateProjMat(
        m_camera->m_fov,
        m_camera->m_near,
        m_camera->m_far,
        m_camera->m_width,
        m_camera->m_height
    );

    m_camera->m_LookAt(m_camera->m_front);
    m_CameraUniforms();
}

void VoxelMap::m_CameraRender(std::unordered_map<std::string, Model*>& models)
{
    m_camera_framebuffer->m_Bind();    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClearColor(
        m_camera_framebuffer_clear_color[0],
        m_camera_framebuffer_clear_color[1],
        m_camera_framebuffer_clear_color[2],
        m_camera_framebuffer_clear_color[3]
    );
    glViewport(0, 0, m_camera->m_width, m_camera->m_height);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    models["sponza"]->m_Draw(m_camera_program);
    m_camera_framebuffer->m_Unbind();
}

void VoxelMap::m_Gui()
{
    ImGui::Begin("VoxelMap");
        if (ImGui::CollapsingHeader("Camera"))
        {
            if(ImGui::TreeNode("Render"))
            {
                ImGui::ColorEdit4("Clear COlor", &m_camera_framebuffer_clear_color[0]);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Attributes"))
            {
                ImGui::InputFloat("Near Plane", &m_camera->m_near, 1.0f);
                ImGui::InputFloat("Far Plane", &m_camera->m_far, 1.0f);
                ImGui::InputFloat("Ortho Volume size", &m_camera->m_ortho_dimensions[1], 1.0f);
                m_camera->m_ortho_dimensions[0] = -m_camera->m_ortho_dimensions[1];
                m_camera->m_ortho_dimensions[2] = -m_camera->m_ortho_dimensions[1];
                m_camera->m_ortho_dimensions[3] = m_camera->m_ortho_dimensions[1];

                ImGui::TreePop();
            }
            ImGui::DragFloat3("Position", &m_camera->m_position[0], 1.0f, -999.0f, 999.0f, "%.2f");
            ImGui::DragFloat3("Direction", &m_camera->m_front[0], 0.01f, -1.0f, 1.0f, "%.2f");

            ImGui::Image(
                (void*)m_camera_framebuffer->m_color_tex[0].m_id, 
                ImVec2(m_camera_framebuffer->m_width, m_camera_framebuffer->m_width), 
                ImVec2(0.0f, 1.0f), 
                ImVec2(1.0f, 0.0f),
                ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
        }
        if (ImGui::CollapsingHeader("Voxel Map"))
        {
            ImGui::InputInt3("Dimensions", &m_voxelmap_dimensions[0]);
            ImGui::InputInt("Mipmap Levels", &m_voxelmap_mipmap_levels, 1, 1);
            ImGui::Checkbox("Shadow Map?", &shadow_active);
            m_voxelmap_generate = ImGui::Button("Generate");
        }
        if (ImGui::CollapsingHeader("Cone Tracing"))
        {
            ImGui::InputFloat("Initial Offset", &voxel_initial_offset, 0.1f, 0.5f, "%.2f");
            if (voxel_initial_offset < 0.0001f)
            {
                voxel_initial_offset = 0.01f;
            }
            const char* items_cam[] = {"6", "9", "5", "1"};
            ImGui::Combo("Directions", &directions_index, items_cam, 4);
            ImGui::InputFloat("Voxel Size", &voxel_size, 0.5f, 0.5f, "%.2f");
            if (voxel_size < 1)
            {
                voxel_size = 1;
            }
            ImGui::Checkbox("Wheights?", &weight_active);
        }

        ImGui::Checkbox("Indirect Light?", &indirect_light_active);
        if (indirect_light_active)
        {
            ImGui::Checkbox("Only Indirect Light", &only_indirect_light_active);
            if (only_indirect_light_active)
            {
                ImGui::Checkbox("Indirect Light High Contrast", &only_indirect_light_high_contrast_active);
            }
            ImGui::Checkbox("Dark Places Indirect Light Help", &dark_places_help_active);
        }

    ImGui::End();
}