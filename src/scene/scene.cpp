#include <scene/scene.hpp>

void Scene::destroy()
{    
    // current_program->m_destroy();
}

Camera* Scene::GetCurrentCamera()
{
    if (current_camera_index == 0)
    {
        return cameras["perspective_cam"];
    }
    else if (current_camera_index == 1)
    {
        return cameras["light_camera"];
    }
}

void Scene::ShadowFramebufferSetup()
{
    framebuffers["framebuffer"] = new FrameBuffer(2048, 2048);

    textures["depth_tex"] = new Texture(
        GL_TEXTURE_2D, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        framebuffers["framebuffer"]->m_width, 
        framebuffers["framebuffer"]->m_height, 
        NULL
    );
    textures["depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["depth_tex"]->m_SetBorderColor(glm::vec3(0.0f));        
    
    framebuffers["framebuffer"]->m_Bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    framebuffers["framebuffer"]->m_AttachDepth(*textures["depth_tex"]);
    framebuffers["framebuffer"]->m_Check();
    framebuffers["framebuffer"]->m_Unbind();
}

void Scene::ShadowThumbnailSetup()
{
    framebuffers["thumb_framebuffer"] = new FrameBuffer(300, 300);

    textures["thumb_depth_tex"] = new Texture(
        GL_TEXTURE_2D, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        framebuffers["thumb_framebuffer"]->m_width,
        framebuffers["thumb_framebuffer"]->m_height,
        NULL
    );    
    textures["thumb_depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["thumb_depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["thumb_depth_tex"]->m_SetBorderColor(glm::vec3(1.0f));
    
    textures["thumb_color_tex"] = new Texture(
        GL_TEXTURE_2D, 
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        framebuffers["thumb_framebuffer"]->m_width,
        framebuffers["thumb_framebuffer"]->m_height, 
        NULL
    );
    textures["thumb_color_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["thumb_color_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["thumb_color_tex"]->m_SetBorderColor(glm::vec3(1.0f));
    
    framebuffers["thumb_framebuffer"]->m_Bind();
    framebuffers["thumb_framebuffer"]->m_AttachColor(*textures["thumb_color_tex"], "thumb_color_tex");
    framebuffers["thumb_framebuffer"]->m_AttachDepth(*textures["thumb_depth_tex"]);    
    framebuffers["thumb_framebuffer"]->m_Check();
    framebuffers["thumb_framebuffer"]->m_Unbind();    
}

void Scene::SetupShadowProgram()
{
    programs["shadow_program"] = new ShaderProgram({
        Shader::m_create("shaders/shadow/shadow.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/shadow/shadow.frag", GL_FRAGMENT_SHADER)
    });
}

void Scene::SetupDirLight()
{
    dir_lights["dir_light"]  = new DirectionalLight(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.60f, 0.60f, 0.60f),
        glm::vec3(0.85f, 0.85f, 0.85f) 
    );
    dir_lights["dir_light"]->m_position = glm::vec3(20.0f, 30.0f, 10.0f);
    dir_lights["dir_light"]->m_direction = dir_lights["dir_light"]->m_position - 2.0f * dir_lights["dir_light"]->m_position;
    
}

void Scene::DirLightCameraSetup()
{
    cameras["light_camera"] = new Camera(90.0f, 0.1f, 75.0f, framebuffers["framebuffer"]->m_width, framebuffers["framebuffer"]->m_height);
    cameras["light_camera"]->m_SetOrtho(25.0f);

    glm::vec3 light_camera_z = glm::normalize(dir_lights["dir_light"]->m_direction);
    glm::vec3 light_camera_y = glm::normalize(glm::cross(light_camera_z, glm::vec3(0.0f, 0.0f, 1.0f)));    
    glm::vec3 light_camera_x = glm::normalize(glm::cross(light_camera_z, light_camera_y));

    cameras["light_camera"]->m_position = dir_lights["dir_light"]->m_position;
    cameras["light_camera"]->m_view_mat = glm::translate(cameras["light_camera"]->m_view_mat, cameras["light_camera"]->m_position);
    cameras["light_camera"]->m_up = light_camera_y;
    cameras["light_camera"]->m_front = light_camera_z;
    cameras["light_camera"]->m_UpdateEulerAngles();
    cameras["light_camera"]->m_LookAt(cameras["light_camera"]->m_front);
}

void Scene::CameraDirLightUpdate()
{
    cameras["light_camera"]->m_position = dir_lights["dir_light"]->m_position;
    cameras["light_camera"]->m_view_mat = glm::translate(cameras["light_camera"]->m_view_mat, cameras["light_camera"]->m_position);    
    cameras["light_camera"]->m_SetFrontDir(cameras["light_camera"]->m_yaw, cameras["light_camera"]->m_pitch);
    cameras["light_camera"]->m_UpdateEulerAngles();
    cameras["light_camera"]->m_LookAt(cameras["light_camera"]->m_front);
}

void Scene::DirLightUpdate()
{
    dir_lights["dir_light"]->m_position = cameras["light_camera"]->m_position;
    dir_lights["dir_light"]->m_direction = cameras["light_camera"]->m_front;
}

void Scene::SetupPointLight()
{
    point_lights["point_light"] = new PointLight(
        glm::vec3(-2.0f, 2.0f, -2.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f
    );
}

void Scene::ShadowProgramUniforms()
{
    programs["shadow_program"]->m_use();    
    programs["shadow_program"]->m_setUniform("view_mat", cameras["light_camera"]->m_view_mat);
    programs["shadow_program"]->m_setUniform("proj_mat", cameras["light_camera"]->m_ortho_mat);

    textures["depth_tex"]->m_Activate(GL_TEXTURE13);
    textures["depth_tex"]->m_Bind();

    programs["illumination_program"]->m_use();
    programs["illumination_program"]->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    programs["illumination_program"]->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);
    programs["illumination_program"]->m_setUniform("shadow_map_bias", shadow_map_bias);
    programs["illumination_program"]->m_setUniform("shadow_enable", shadow_enable);
    programs["illumination_program"]->m_setUniform("shadow_map", 13);
}

void Scene::SetupIlluminationProgram()
{
    GLuint vertex_shader = Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = Shader::m_create("shaders/fragment.frag", GL_FRAGMENT_SHADER);    
    programs["illumination_program"] = new ShaderProgram({vertex_shader, fragment_shader});
    programs["illumination_program"]->m_selected = true;
    
}

void Scene::IlluminationProgramUniforms()
{
    programs["illumination_program"]->m_use();    
    programs["illumination_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["illumination_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());    
    programs["illumination_program"]->m_setUniform("camera_pos", cameras["current_camera"]->m_position);
    
    programs["illumination_program"]->m_setUniform("dir_light.direction", dir_lights["dir_light"]->m_direction);
    programs["illumination_program"]->m_setUniform("dir_light.ambient", dir_lights["dir_light"]->m_ambient);
    programs["illumination_program"]->m_setUniform("dir_light.diffuse", dir_lights["dir_light"]->m_diffuse);
    programs["illumination_program"]->m_setUniform("dir_light.specular", dir_lights["dir_light"]->m_specular);
    programs["illumination_program"]->m_setUniform("directional_active", (int)directional_active);

    programs["illumination_program"]->m_setUniform("point_light.position", point_lights["point_light"]->m_position);
    programs["illumination_program"]->m_setUniform("point_light.ambient", point_lights["point_light"]->m_ambient);
    programs["illumination_program"]->m_setUniform("point_light.diffuse", point_lights["point_light"]->m_diffuse);
    programs["illumination_program"]->m_setUniform("point_light.specular", point_lights["point_light"]->m_specular);
    programs["illumination_program"]->m_setUniform("point_light.constant", point_lights["point_light"]->m_constant);
    programs["illumination_program"]->m_setUniform("point_light.linear", point_lights["point_light"]->m_linear);
    programs["illumination_program"]->m_setUniform("point_light.quadratic", point_lights["point_light"]->m_quadratic);
}

void Scene::SetupLightlessProgram()
{
    programs["lightless_program"] = new ShaderProgram({
        Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/lightless_fragment.frag", GL_FRAGMENT_SHADER)
    });
}

void Scene::LightlessProgramUniforms()
{
    programs["lightless_program"]->m_use();
    programs["lightless_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["lightless_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
    programs["lightless_program"]->m_setUniform("map_type", map_type);
}

void Scene::SetupLampProgram()
{
    programs["lamp_program"] = new ShaderProgram({
        Shader::m_create("shaders/lamp/lamp.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/lamp/lamp.frag", GL_FRAGMENT_SHADER)
    });

    models["point_light_model"] = new Model(PROJECT_ROOT + std::string("models/cube/cube.obj"), false);
    models["point_light_model"]->m_model_mat = glm::scale(models["point_light_model"]->m_model_mat, glm::vec3(0.25f));
    models["point_light_model"]->m_model_mat = glm::translate(models["point_light_model"]->m_model_mat, point_lights["point_light"]->m_position);
    models["point_light_model"]->m_shader = programs["lamp_program"];
}

void Scene::LampProgramUniforms()
{
    programs["lamp_program"]->m_use();
    programs["lamp_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["lamp_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
}

void Scene::LoadModels()
{
    models["sponza"] = new Model(PROJECT_ROOT + std::string("models/sponza/sponza.obj"), false);
    models["sponza"]->m_model_mat = glm::scale(models["sponza"]->m_model_mat, glm::vec3(0.02));

    // models["floor"] = new Model(PROJECT_ROOT + std::string("models/floor/floor.obj"), false);
    // models["container_0"] = new Model(PROJECT_ROOT + std::string("models/wood-container/wood-container.obj"), false);    
    // models["container_1"] = new Model(models["container_0"]);
    // models["container_2"] = new Model(models["container_0"]);
    // models["container_3"] = new Model(models["container_0"]);
    // models["container_4"] = new Model(models["container_0"]);

    // srand(time(NULL));

    // models["floor"]->m_model_mat = glm::translate(models["floor"]->m_model_mat, glm::vec3(0, 0, 0));
    // models["floor"]->m_model_mat = glm::scale(models["floor"]->m_model_mat, glm::vec3(10.0f, 1.0f, 10.0f));

    // models["container_0"]->m_model_mat = glm::translate(models["container_0"]->m_model_mat, glm::vec3(rand()%15+1, rand()%10+1, rand()%15+1));
    // models["container_0"]->m_model_mat = glm::scale(models["container_0"]->m_model_mat, glm::vec3(5.0f));

    // models["container_1"]->m_model_mat = glm::translate(models["container_1"]->m_model_mat, glm::vec3(rand()%15+1, rand()%10+1, rand()%15+1));
    // models["container_1"]->m_model_mat = glm::scale(models["container_1"]->m_model_mat, glm::vec3(5.0f));

    // models["container_2"]->m_model_mat = glm::translate(models["container_2"]->m_model_mat, glm::vec3(rand()%15+1, rand()%10+1, rand()%15+1));
    // models["container_2"]->m_model_mat = glm::scale(models["container_2"]->m_model_mat, glm::vec3(5.0f));

    // models["container_3"]->m_model_mat = glm::translate(models["container_3"]->m_model_mat, glm::vec3(rand()%15+1, rand()%10+1, rand()%15+1));
    // models["container_3"]->m_model_mat = glm::scale(models["container_3"]->m_model_mat, glm::vec3(5.0f));

    // models["container_4"]->m_model_mat = glm::translate(models["container_4"]->m_model_mat, glm::vec3(rand()%15+1, rand()%10+1, rand()%15+1));
    // models["container_4"]->m_model_mat = glm::scale(models["container_4"]->m_model_mat, glm::vec3(5.0f));
}

void Scene::UpdateModels()
{
    for (auto i = models.begin(); i != models.end(); i++)
    {
        programs["current_program"]->m_setUniform("normal_mat", i->second->m_GetNormalMat());
    }
}

void Scene::RenderShadowFramebuffer()
{
    glViewport(0, 0, framebuffers["framebuffer"]->m_width, framebuffers["framebuffer"]->m_height);
    framebuffers["framebuffer"]->m_Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = models.begin(); i != models.end(); i++)
    {
        i->second->m_Draw(programs["shadow_program"]);
    }
    framebuffers["framebuffer"]->m_Unbind();
}

void Scene::RenderShadowThumbnailFramebuffer()
{
    glViewport(0, 0, framebuffers["thumb_framebuffer"]->m_width, framebuffers["thumb_framebuffer"]->m_height);
    framebuffers["thumb_framebuffer"]->m_Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = models.begin(); i != models.end(); i++)
    {
        i->second->m_Draw(programs["shadow_program"]);
    }
    framebuffers["thumb_framebuffer"]->m_Unbind();
}

void Scene::LampUpdate()
{
    models["point_light_model"]->m_model_mat[3][0] = point_lights["point_light"]->m_position.x;
    models["point_light_model"]->m_model_mat[3][1] = point_lights["point_light"]->m_position.y;
    models["point_light_model"]->m_model_mat[3][2] = point_lights["point_light"]->m_position.z;
    glm::mat4 lamp_model_mat = glm::scale(models["point_light_model"]->m_model_mat, glm::vec3(models["point_light_model"]->m_scale));

    programs["lamp_program"]->m_use();
    programs["lamp_program"]->m_setUniform("model_mat", lamp_model_mat);
    programs["lamp_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["lamp_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
}

void Scene::PointLightUpdate()
{
    programs["current_program"]->m_setUniform("point_light.position", point_lights["point_light"]->m_position);

    point_lights["point_light"]->m_ambient = glm::vec3(point_lights["point_light"]->m_ambient[0]);
    point_lights["point_light"]->m_diffuse = glm::vec3(point_lights["point_light"]->m_diffuse[0]);
    point_lights["point_light"]->m_specular = glm::vec3(point_lights["point_light"]->m_specular[0]);
}

void Scene::CurrentProgramUpdate()
{
    programs["current_program"]->m_use();            
    programs["current_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["current_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());    
    programs["current_program"]->m_setUniform("camera_pos", cameras["current_camera"]->m_position);    
    programs["current_program"]->m_setUniform("directional_active", (int)directional_active);    

    PointLightUpdate();

    programs["current_program"]->m_setUniform("point_light.ambient", point_lights["point_light"]->m_ambient);
    programs["current_program"]->m_setUniform("point_light.diffuse", point_lights["point_light"]->m_diffuse);
    programs["current_program"]->m_setUniform("point_light.specular", point_lights["point_light"]->m_specular);
    programs["current_program"]->m_setUniform("point_light.constant", point_lights["point_light"]->m_constant);
    programs["current_program"]->m_setUniform("point_light.linear", point_lights["point_light"]->m_linear);
    programs["current_program"]->m_setUniform("point_light.quadratic", point_lights["point_light"]->m_quadratic);
}

void Scene::CurrentCameraUpdate(Keyboard* keyboard, float delta_time)
{
    if (current_camera_index != 1)
    {
        CameraDirLightUpdate();
    }

    cameras["current_camera"] = GetCurrentCamera();

    if (keyboard->m_Pressed('w'))
    {
        cameras["current_camera"]->m_position += delta_time * cameras["current_camera"]->m_speed * cameras["current_camera"]->m_front;
    }
    if (keyboard->m_Pressed('s'))
    {
        cameras["current_camera"]->m_position -= delta_time * cameras["current_camera"]->m_speed * cameras["current_camera"]->m_front;
    }
    if (keyboard->m_Pressed('a'))
    {
        cameras["current_camera"]->m_position -= delta_time * cameras["current_camera"]->m_speed * glm::normalize(glm::cross(cameras["current_camera"]->m_front, cameras["current_camera"]->m_up));
    }
    if (keyboard->m_Pressed('d'))
    {
        cameras["current_camera"]->m_position += delta_time * cameras["current_camera"]->m_speed * glm::normalize(glm::cross(cameras["current_camera"]->m_front, cameras["current_camera"]->m_up));
    }
    if (keyboard->m_Pressed(15)) // SHIFT
    {
        cameras["current_camera"]->m_position += delta_time * cameras["current_camera"]->m_speed * glm::vec3(0, 1, 0);
    }
    if (keyboard->m_Pressed(7)) // CTRL
    {
        cameras["current_camera"]->m_position += delta_time * (-cameras["current_camera"]->m_speed) * glm::vec3(0, 1, 0);
    }
    
    cameras["current_camera"]->m_LookAt(cameras["current_camera"]->m_front);
    cameras["current_camera"]->m_UpdateProjMat(
        cameras["current_camera"]->m_fov,
        cameras["current_camera"]->m_near,
        cameras["current_camera"]->m_far,
        cameras["current_camera"]->m_width, 
        cameras["current_camera"]->m_height
    );
}

void Scene::SceneGui(Mouse* mouse)
{
    ImGui::Begin("Things");

    if (ImGui::CollapsingHeader("Light Properties"))
    {
        if (ImGui::TreeNode("Direct lights"))
        {
            ImGui::Checkbox("Enable?", &directional_active);
            ImGui::DragFloat3("Position", &dir_lights["dir_light"] ->m_position[0], 0.25f, -999.0f, 999.0f, "%.3f");            
            // ImGui::DragFloat3("Direction", &dir_lights["dir_light"] ->m_direction[0], 0.25f, -999.0f, 999.0f, "%.3f");
            ImGui::DragFloat3("Ambient", &dir_lights["dir_light"]->m_ambient[0], 0.05f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Diffuse", &dir_lights["dir_light"]->m_diffuse[0], 0.05f, 0.0f, 5.0f, "%.2f");
            ImGui::DragFloat3("Specular", &dir_lights["dir_light"]->m_specular[0], 0.05f, 0.0f, 5.0f, "%.2f");

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point Lights"))
        {
            ImGui::DragFloat3("Position", &point_lights["point_light"]->m_position[0], 0.25f, -999.0f, 999.0f, "%.3f");
            ImGui::DragFloat3("Ambient", &point_lights["point_light"]->m_ambient[0], 0.05f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Diffuse", &point_lights["point_light"]->m_diffuse[0], 0.05f, 0.0f, 5.0f, "%.2f");
            ImGui::DragFloat3("Specular", &point_lights["point_light"]->m_specular[0], 0.05f, 0.0f, 5.0f, "%.2f");
            ImGui::DragFloat("Scale", &models["point_light_model"]->m_scale, 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat("Constant Attenuation", &point_lights["point_light"]->m_constant, 0.0001f, 0.0001f, 5.0f, "%.4f");
            ImGui::DragFloat("Linear Attenuation", &point_lights["point_light"]->m_linear, 0.0001f, 0.0001f, 5.0f, "%.4f");
            ImGui::DragFloat("Quadratic Attenuation", &point_lights["point_light"]->m_quadratic, 0.0001f, 0.0001f, 5.0f, "%.4f");

            ImGui::TreePop();
        }        
    }

    if (ImGui::CollapsingHeader("Camera Properties"))
    {
        ImGui::DragFloat("Near Plane", &cameras["current_camera"]->m_near, 0.01f, 0.0f, 100000.0f, "%.2f");
        ImGui::DragFloat("Far Plane", &cameras["current_camera"]->m_far, 10.0f, 0.0f, 100000.0f, "%.2f");
        ImGui::DragFloat("FOV", &cameras["current_camera"]->m_fov, 1.0f, 0.0f, 180.0f, "%.2f");

        const char* items_proj[] = {"perspective", "orthographic"};
        ImGui::Combo("Projection Type", &cameras["current_camera"]->m_curr_proj_mat, items_proj, 2);

        const char* items_cam[] = {"perspective_cam", "light_camera"};
        ImGui::Combo("Select Camera", &current_camera_index, items_cam, 2);
    }

    if (ImGui::CollapsingHeader("Control Properties"))
    {
        ImGui::DragFloat("Move Speed", &cameras["current_camera"]->m_speed, 0.05f, 0.0f, 100.0f, "%.2f");    
        ImGui::DragFloat("Mouse Sensibility", &mouse->m_sensitivity, 0.001f, 0.0f, 5.0f, "%.3f");
    }

    if (ImGui::CollapsingHeader("Scene Properties"))
    {
        if (ImGui::BeginMenu("Current Shader"))
        {        
            if (ImGui::MenuItem("illumination_program", "", &programs["illumination_program"]->m_selected))
            {
                programs["current_program"] = programs["illumination_program"];
                programs["illumination_program"]->m_selected = true;
                programs["lightless_program"]->m_selected = false;
            } 
            if (ImGui::MenuItem("lightless_program", "", &programs["lightless_program"]->m_selected))
            {
                programs["current_program"] = programs["lightless_program"];
                programs["lightless_program"]->m_selected = true;
                programs["illumination_program"]->m_selected = false;
            }        
            ImGui::EndMenu();
        }
        ImGui::SliderInt("Map Type", &map_type, 1, 4);  
    }

    if (ImGui::CollapsingHeader("Shadow Map"))
    {
        ImGui::DragFloat("Yaw", &cameras["light_camera"]->m_yaw, 1.0f);
        ImGui::DragFloat("Pitch", &cameras["light_camera"]->m_pitch, 1.0f, -89.0f, 89.0f);
        ImGui::Image((void*)textures["thumb_color_tex"]->m_id, ImVec2(300, 300), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        ImGui::SameLine();
        ImGui::Image((void*)textures["thumb_depth_tex"]->m_id, ImVec2(300, 300), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        ImGui::Separator();
        ImGui::Checkbox("Enable?", &shadow_enable);
        ImGui::InputFloat("Bias", &shadow_map_bias, 0.005, 0.1);
    }    

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void Scene::SceneDebugGui()
{
    ImGui::Begin("Debug");
        ImGui::Text("light_camera");     

        ImGui::Text("\tlight_view_mat:");
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_view_mat[0][0], cameras["light_camera"]->m_view_mat[0][1], cameras["light_camera"]->m_view_mat[0][2], cameras["light_camera"]->m_view_mat[0][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_view_mat[1][0], cameras["light_camera"]->m_view_mat[1][1], cameras["light_camera"]->m_view_mat[1][2], cameras["light_camera"]->m_view_mat[1][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_view_mat[2][0], cameras["light_camera"]->m_view_mat[2][1], cameras["light_camera"]->m_view_mat[2][2], cameras["light_camera"]->m_view_mat[2][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_view_mat[3][0], cameras["light_camera"]->m_view_mat[3][1], cameras["light_camera"]->m_view_mat[3][2], cameras["light_camera"]->m_view_mat[3][3]);

        ImGui::Text("\tlight_proj_mat:");
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_ortho_mat[0][0], cameras["light_camera"]->m_ortho_mat[0][1], cameras["light_camera"]->m_ortho_mat[0][2], cameras["light_camera"]->m_ortho_mat[0][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_ortho_mat[1][0], cameras["light_camera"]->m_ortho_mat[1][1], cameras["light_camera"]->m_ortho_mat[1][2], cameras["light_camera"]->m_ortho_mat[1][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_ortho_mat[2][0], cameras["light_camera"]->m_ortho_mat[2][1], cameras["light_camera"]->m_ortho_mat[2][2], cameras["light_camera"]->m_ortho_mat[2][3]);
        ImGui::Text("\t %.3f %.3f %.3f %.3f", cameras["light_camera"]->m_ortho_mat[3][0], cameras["light_camera"]->m_ortho_mat[3][1], cameras["light_camera"]->m_ortho_mat[3][2], cameras["light_camera"]->m_ortho_mat[3][3]);
    ImGui::End();

    cameras["light_camera"];
}

void Scene::TestProgram()
{
    programs["test_program"] = new ShaderProgram({
        Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/test.frag", GL_FRAGMENT_SHADER),
    });

    textures["depth_tex"]->m_Activate(GL_TEXTURE13);
    textures["depth_tex"]->m_Bind();

    programs["test_program"]->m_use();
    programs["test_program"]->m_setUniform("shadow_map", 13);
    programs["test_program"]->m_setUniform("shadow_map_bias", shadow_map_bias);    
    programs["test_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["test_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
    programs["test_program"]->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    programs["test_program"]->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);
}

void Scene::TestProgramUniforms()
{
    textures["depth_tex"]->m_Activate(GL_TEXTURE13);
    textures["depth_tex"]->m_Bind();

    programs["test_program"]->m_use();
    programs["test_program"]->m_setUniform("shadow_map", 13);
    programs["test_program"]->m_setUniform("shadow_map_bias", shadow_map_bias);    
    programs["test_program"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["test_program"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
    programs["test_program"]->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    programs["test_program"]->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);
}

void Scene::TestFrameBuffer(int width, int height)
{
    framebuffers["test_framebuffer"] = new FrameBuffer(width, height);
    textures["test_depth_tex"] = new Texture(
        GL_TEXTURE_2D, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        framebuffers["test_framebuffer"]->m_width, 
        framebuffers["test_framebuffer"]->m_height, 
        NULL
    );
    textures["test_depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["test_depth_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["test_depth_tex"]->m_SetBorderColor(glm::vec3(0.0f));
    
    textures["test_color_tex"] = new Texture(
        GL_TEXTURE_2D, 
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        framebuffers["test_framebuffer"]->m_width, 
        framebuffers["test_framebuffer"]->m_height, 
        NULL
    );
    textures["test_color_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["test_color_tex"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["test_color_tex"]->m_SetBorderColor(glm::vec3(0.0f));
    
    framebuffers["test_framebuffer"]->m_Bind();
    framebuffers["test_framebuffer"]->m_AttachColor(*textures["test_color_tex"], "test_color_tex");
    framebuffers["test_framebuffer"]->m_AttachDepth(*textures["test_depth_tex"]);
    framebuffers["test_framebuffer"]->m_Check();
    framebuffers["test_framebuffer"]->m_Unbind();
}

void Scene::RenderTestFrameBuffer()
{
    glViewport(0, 0, framebuffers["test_framebuffer"]->m_width, framebuffers["test_framebuffer"]->m_height);
    framebuffers["test_framebuffer"]->m_Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = models.begin(); i != models.end(); i++)
    {
        i->second->m_Draw(programs["test_program"]);
    }
    framebuffers["test_framebuffer"]->m_Unbind();
}

void Scene::TestProgram2()
{
    programs["test_program2"] = new ShaderProgram({
        Shader::m_create("shaders/vertex.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/test2.frag", GL_FRAGMENT_SHADER),
    });

    textures["depth_tex"]->m_Activate(GL_TEXTURE13);
    textures["depth_tex"]->m_Bind();

    programs["test_program2"]->m_use();
    programs["test_program2"]->m_setUniform("shadow_map", 13);
    programs["test_program2"]->m_setUniform("shadow_map_bias", shadow_map_bias);    
    programs["test_program2"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["test_program2"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
    programs["test_program2"]->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    programs["test_program2"]->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);
}

void Scene::TestProgramUniforms2()
{
    textures["depth_tex"]->m_Activate(GL_TEXTURE13);
    textures["depth_tex"]->m_Bind();

    programs["test_program2"]->m_use();
    programs["test_program2"]->m_setUniform("shadow_map", 13);
    programs["test_program2"]->m_setUniform("shadow_map_bias", shadow_map_bias);    
    programs["test_program2"]->m_setUniform("view_mat", cameras["current_camera"]->m_view_mat);
    programs["test_program2"]->m_setUniform("proj_mat", cameras["current_camera"]->m_GetProjMat());
    programs["test_program2"]->m_setUniform("light_view_mat", cameras["light_camera"]->m_view_mat);
    programs["test_program2"]->m_setUniform("light_proj_mat", cameras["light_camera"]->m_ortho_mat);
}

void Scene::TestFrameBuffer2(int width, int height)
{
    framebuffers["test_framebuffer2"] = new FrameBuffer(width, height);
    textures["test_depth_tex2"] = new Texture(
        GL_TEXTURE_2D, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        framebuffers["test_framebuffer2"]->m_width, 
        framebuffers["test_framebuffer2"]->m_height, 
        NULL
    );
    textures["test_depth_tex2"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["test_depth_tex2"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["test_depth_tex2"]->m_SetBorderColor(glm::vec3(0.0f));
    
    textures["test_color_tex2"] = new Texture(
        GL_TEXTURE_2D, 
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        framebuffers["test_framebuffer2"]->m_width, 
        framebuffers["test_framebuffer2"]->m_height, 
        NULL
    );
    textures["test_color_tex2"]->m_SetWrapping(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    textures["test_color_tex2"]->m_SetWrapping(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    textures["test_color_tex2"]->m_SetBorderColor(glm::vec3(0.0f));
    
    framebuffers["test_framebuffer2"]->m_Bind();
    framebuffers["test_framebuffer2"]->m_AttachColor(*textures["test_color_tex2"], "test_color_tex2");
    framebuffers["test_framebuffer2"]->m_AttachDepth(*textures["test_depth_tex2"]);
    framebuffers["test_framebuffer2"]->m_Check();
    framebuffers["test_framebuffer2"]->m_Unbind();
}

void Scene::RenderTestFrameBuffer2()
{
    glViewport(0, 0, framebuffers["test_framebuffer2"]->m_width, framebuffers["test_framebuffer2"]->m_height);
    framebuffers["test_framebuffer2"]->m_Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = models.begin(); i != models.end(); i++)
    {
        i->second->m_Draw(programs["test_program2"]);
    }
    framebuffers["test_framebuffer2"]->m_Unbind();
}

void Scene::SetupCamera(int width, int height)
{
    cameras["perspective_cam"] = new Camera(90.0f, 1.0f, 100.0f, width, height);
    cameras["current_camera"] = cameras["perspective_cam"];
}