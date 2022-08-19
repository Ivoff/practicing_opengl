#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <utils/utils.cpp>
#include <env.hpp>
#include <string>
#include <glm/vec3.hpp>

struct Texture
{
    GLuint m_id;
    GLuint m_target;
    GLuint m_tex_unit;
    GLuint m_format;
    unsigned char* m_data;
    int m_width;
    int m_height;
    int m_depth;
    int m_channels;
    std::string m_type;
    std::string m_path;


    Texture(){};
    Texture(GLuint target, GLuint format, GLuint type, int width, int height, int depth, void* data);
    Texture(GLuint target, GLuint format, GLuint type, int width, int height, void* data);
    Texture(std::string path, GLuint target, bool flip);
    void m_SetWrapping(GLuint axis, GLuint wrapping);
    void m_SetFiltering(GLuint filtering, GLuint scale);
    void m_GenTex(bool auto_mipmap, GLuint type);
    void m_DefaultConfig(GLuint type);
    void m_Activate();
    void m_Activate(GLuint tex_unit);
    void m_Bind();
    void m_Unbind();
    void m_Free();
    void m_SetBorderColor(glm::vec3 color);
    ~Texture();
    
    Texture WhiteTex();
    Texture BlackTex();    
    
    //TODO: função de gerar mipmap manualmente    
};

#endif