#include <omnidirectional_shadow/omnidirectional_shadow.hpp>

OmnidirectionalShadow::OmnidirectionalShadow(PointLight* point_light)
{
    m_point_light = point_light;
    m_framebuffer = new FrameBuffer(1024, 1024);
    m_program = new ShaderProgram({
        Shader::m_create("shaders/omnidirectional_shadow/omnidirectional_shadow.vert", GL_VERTEX_SHADER),
        Shader::m_create("shaders/omnidirectional_shadow/omnidirectional_shadow.geom", GL_GEOMETRY_SHADER),
        Shader::m_create("shaders/omnidirectional_shadow/omnidirectional_shadow.frag", GL_FRAGMENT_SHADER)
    });

    // m_cubemap = new Texture(
    //     GL_TEXTURE_CUBE_MAP, 
    //     GL_R32F, 
    //     GL_FLOAT, 
    //     m_framebuffer->m_width, 
    //     m_framebuffer->m_height, 
    //     NULL
    // );

    m_cubemap = new Texture(
        GL_TEXTURE_CUBE_MAP, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        m_framebuffer->m_width, 
        m_framebuffer->m_height, 
        NULL
    );
    
    m_cubemap->m_Activate(GL_TEXTURE14);
    m_cubemap->m_Bind();

    m_framebuffer->m_Bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // m_framebuffer->m_AttachColor(*m_cubemap, "depth_cubemap");
    m_framebuffer->m_AttachDepth(*m_cubemap);
    m_framebuffer->m_Check();
    m_framebuffer->m_Unbind();

    m_near = 1.0f;
    m_far = 50.0f;
    m_proj_mat = glm::perspective(glm::radians(90.0f), m_framebuffer->m_width*1.0f/m_framebuffer->m_height*1.0f, m_near, m_far);
    
    m_Uniforms();
}

void OmnidirectionalShadow::m_Update()
{
    m_Uniforms();
}

void OmnidirectionalShadow::m_Uniforms()
{
    m_program->m_use();

    glm::mat4 right = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    m_program->m_setUniform("proj_matrixes[0]", right);

    glm::mat4 left = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    m_program->m_setUniform("proj_matrixes[1]", left);

    glm::mat4 top = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_program->m_setUniform("proj_matrixes[2]", top);

    glm::mat4 bottom = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    m_program->m_setUniform("proj_matrixes[3]", bottom);

    glm::mat4 back = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    m_program->m_setUniform("proj_matrixes[4]", back);

    glm::mat4 front = m_proj_mat * glm::lookAt(m_point_light->m_position, m_point_light->m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    m_program->m_setUniform("proj_matrixes[5]", front);

    m_program->m_setUniform("light_pos", m_point_light->m_position);
    m_program->m_setUniform("far_plane", m_far);
    m_program->m_setUniform("near_plane", m_near);
}

void OmnidirectionalShadow::m_Render(std::unordered_map<std::string, Model*>& models)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, m_framebuffer->m_width, m_framebuffer->m_height);
    m_framebuffer->m_Bind();
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    models["sponza"]->m_Draw(m_program);
    // for (auto i = models.begin(); i != models.end(); i++)
    // {
    //     if (i->second->m_dont_render == false)
    //     {
    //         i->second->m_Draw(m_program);
    //     }
    // }
    m_framebuffer->m_Unbind();
}