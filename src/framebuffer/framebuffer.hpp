#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <texture/texture.hpp>
#include <utils/utils.cpp>

struct FrameBuffer
{
    GLuint m_id;
    int m_width;
    int m_height;
    std::unordered_map<std::string, GLuint> m_color_attach;
    std::vector<Texture> m_color_tex;
    GLuint m_depth_attach;
    GLuint m_stencil_attach;

    FrameBuffer(int width, int height);
    void m_AttachColor(Texture texture, std::string name);
    void m_AttachDepth(Texture texture);
    void m_AttachStencil(Texture texture);
    void m_AttachDepthStencil(Texture texture);    
    void m_Bind();
    void m_Unbind();
    void m_Check();
};

#endif