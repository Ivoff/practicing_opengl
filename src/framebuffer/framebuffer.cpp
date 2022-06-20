#include <framebuffer/framebuffer.hpp>

FrameBuffer::FrameBuffer(int width, int height) :
    m_width {width},
    m_height {height} 
{
    glGenFramebuffers(1, &m_id);    
}

void FrameBuffer::m_Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBuffer::m_Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::m_AttachColor(Texture texture, std::string name)
{
    int attach_count = GL_COLOR_ATTACHMENT0 + m_color_tex.size();
    m_color_attach[name] = attach_count;
    m_color_tex.emplace_back(texture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attach_count, GL_TEXTURE_2D, texture.m_id, 0);
}

void FrameBuffer::m_AttachDepth(Texture texture)
{
    m_depth_attach = texture.m_id;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.m_id, 0);
}

void FrameBuffer::m_AttachStencil(Texture texture)
{
    m_depth_attach = texture.m_id;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture.m_id, 0);
}

void FrameBuffer::m_AttachDepthStencil(Texture texture)
{
    m_depth_attach = texture.m_id;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, texture.m_id, 0);
}

void FrameBuffer::m_Check()
{
    GLuint result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (!(result == GL_FRAMEBUFFER_COMPLETE))
    {
        switch(result)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                printf("GL_FRAMEBUFFER_UNDEFINED is returned if the specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist\n");                
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete.\n");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT is returned if the framebuffer does not have at least one image attached to it.\n");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.\n");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.\n");
            break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                printf("GL_FRAMEBUFFER_UNSUPPORTED is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.\n\nGL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.\n");
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.\n");
            break;            
        }
        std::exit(0);
    }
}