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
    unsigned char *m_data;
    int m_width;
    int m_height;
    int m_channels;

    Texture(std::string path, GLuint target, GLuint tex_unit);    
    void m_set_wrapping(GLuint axis, GLuint wrapping);
    void m_set_filtering(GLuint filtering, GLuint scale);    
    void m_gen_tex(GLuint tex_data_type, GLuint pixel_data_type, bool auto_mipmap);
    void m_activate();
    void m_bind();
    void m_free();
    ~Texture();
    
    //TODO: função de gerar mipmap manualmente    
};

#endif