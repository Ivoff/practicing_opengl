#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <env.hpp>
#include <string>

struct Texture
{
    GLuint m_id;
    GLuint m_target;
    GLuint m_tex_unit;
    GLuint m_format;
    unsigned char *m_data;
    int m_width;
    int m_height;
    int m_channels;
    std::string m_type;
    std::string m_path;

    Texture(std::string path, GLuint target);
    void m_SetWrapping(GLuint axis, GLuint wrapping);    
    void m_SetFiltering(GLuint filtering, GLuint scale);        
    void m_GenTex(bool auto_mipmap);    
    void m_DefaultConfig();
    void m_Activate();
    void m_Activate(GLuint tex_unit);
    void m_Bind();    
    void m_Free();    
    ~Texture();
    
    //TODO: função de gerar mipmap manualmente    
};

#endif