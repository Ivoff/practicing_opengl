#include <texture/texture.hpp>

Texture::Texture(std::string path, GLuint target)
{
    m_target = target;
    
    path = PROJECT_ROOT + path;

    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!m_data)
    {
        printf("Texture \"%s\" not loaded correctly\n", path.c_str());
        std::exit(0);
    }

    glGenTextures(1, &m_id);
    glBindTexture(target, m_id);
}

void Texture::m_gen_tex(GLuint tex_data_type, GLuint pixel_data_type, bool auto_mipmap)
{
    m_bind();

    if (m_target == GL_TEXTURE_2D)
    {
        glTexImage2D(m_target, !auto_mipmap, tex_data_type, m_width, m_height, 0, pixel_data_type, GL_UNSIGNED_BYTE, m_data);
        glGenerateMipmap(m_target);
    }    
}

void Texture::m_bind()
{
    glBindTexture(m_target, m_id);
}

void Texture::m_set_wrapping(GLuint axis, GLuint wrapping)
{
    m_bind();
    glTexParameteri(m_target, axis, wrapping);
}

void Texture::m_set_filtering(GLuint filtering, GLuint scale)
{
    m_bind();
    glTexParameteri(m_target, scale, filtering);
}

void Texture::m_activate(GLuint tex_unit)
{
    m_tex_unit = tex_unit;
    
    glActiveTexture(m_tex_unit);
    m_bind();    
}

void Texture::m_free()
{
    stbi_image_free(m_data);
}

Texture::~Texture()
{
    m_free();
}