#ifndef OMNIDIRECTIONAL_SHADOW_HPP
#define OMNIDIRECTIONAL_SHADOW_HPP

#include <light/point/light_point.hpp>
#include <texture/texture.hpp>
#include <framebuffer/framebuffer.hpp>
#include <shader/shader.hpp>
#include <model/model.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <string>

struct OmnidirectionalShadow
{
    Texture* m_cubemap;
    Texture* m_depthmap;
    PointLight* m_point_light;
    FrameBuffer* m_framebuffer;
    ShaderProgram* m_program;
    glm::mat4 m_proj_mat;
    float m_near;
    float m_far;

    OmnidirectionalShadow(PointLight* point_light);
    void m_Update();
    void m_Render(std::unordered_map<std::string, Model*>& models);
    void m_Uniforms();
};


#endif