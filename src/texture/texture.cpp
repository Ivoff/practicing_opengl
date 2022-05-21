#include <texture/texture.hpp>

Texture::Texture(std::string path, GLuint target)
{
    m_target = target;    
    m_tex_unit = -1;

    if (path[0] != '/')
    {
        path = PROJECT_ROOT + path;
    }        
    // stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!m_data)
    {
        printf("Texture \"%s\" not loaded correctly\n", path.c_str());
        std::exit(0);
    }

    if (m_channels == 1)
    {
        m_format = GL_RED;
    }
    else if (m_channels == 3)
    {
        m_format = GL_RGB;
    }
    else if (m_channels == 4)
    {
        m_format = GL_RGBA;
    }

    glGenTextures(1, &m_id);
    m_Bind();
    m_DefaultConfig();
}

void Texture::m_GenTex(bool auto_mipmap)
{
    m_Bind();

    if (m_target == GL_TEXTURE_2D)
    {
        glTexImage2D(m_target, !auto_mipmap, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_data);
        glGenerateMipmap(m_target);
    }    
}

void Texture::m_Bind()
{    
    glBindTexture(m_target, m_id);
}

void Texture::m_SetWrapping(GLuint axis, GLuint wrapping)
{    
    glTexParameteri(m_target, axis, wrapping);
}

void Texture::m_SetFiltering(GLuint filtering, GLuint scale)
{    
    glTexParameteri(m_target, scale, filtering);
}

void Texture::m_Activate()
{
    if (m_tex_unit == -1)
    {
        return;
    }        

    glActiveTexture(m_tex_unit);
}

void Texture::m_Activate(GLuint tex_unit)
{    
    glActiveTexture(tex_unit);
}

void Texture::m_DefaultConfig()
{
    m_GenTex(true);
    m_SetFiltering(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_SetFiltering(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture Texture::WhiteTex()
{
    static Texture white_tex = Texture("textures/white.jpg", GL_TEXTURE_2D);
    return white_tex;
}

Texture Texture::BlackTex()
{
    static Texture black_tex = Texture("textures/black.jpg", GL_TEXTURE_2D);
    return black_tex;
}

void Texture::m_Free()
{
    stbi_image_free(m_data);
}

Texture::~Texture()
{    
}