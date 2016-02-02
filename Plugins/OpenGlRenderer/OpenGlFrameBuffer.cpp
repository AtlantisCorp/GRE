//
//  OpenGlFrameBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/01/2016.
//
//

#include "OpenGlFrameBuffer.h"

typedef GreExceptionWithText OpenGlGenFrameBuffer;
typedef GreExceptionWithText OpenGlDeleteFrameBuffer;
typedef GreExceptionWithText OpenGlBindFrameBuffer;
typedef GreExceptionWithText OpenGlMaxColorAttachement;
typedef GreExceptionWithText OpenGlGreAttachementToGl;

OpenGlFrameBuffer::OpenGlFrameBuffer(const std::string& name)
: FrameBufferPrivate(name), _mfboid(0)
{
#if GL_ARB_framebuffer_object
    // Collecting Maximum Properties.
    Gl::GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_mMaxColorAttachement);
    
#   if GL_ARB_framebuffer_no_attachements
    Gl::GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &_mMaxWidth);
    Gl::GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &_mMaxHeight);
    Gl::GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &_mMaxSamples);
#   endif
    
    Gl::GenFramebuffers(1, &_mfboid);
    
#   ifdef GreIsDebugMode
    if(!_mfboid)
    {
        GreDebugPretty() << "Oh no, the OpenGlFrameBuffer object '" << name << "' could not be generated..." << std::endl;
        throw OpenGlGenFrameBuffer(name + " (glGenFrameBuffers)");
    }
#   endif
    
#elif GL_EXT_framebuffer_object
    // This is the other way to create Framebuffer's, using the EXT functions if the ARB's are
    // not available.
    
    // Collecting Maximum Properties.
    Gl::GetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &_mMaxColorAttachement);
    GL::GenFramebuffers(1, &_mfboid);
    
#   ifdef GreIsDebugMode
    if(!_mfboid)
    {
        GreDebugPretty() << "Oh no, the OpenGlFrameBuffer object '" << name << "' could not be generated..." << std::endl;
        throw OpenGlGenFrameBuffer(name + " (glGenFrameBuffersEXT)");
    }
#   endif
    
#endif // GL_ARB_framebuffer_object
}

OpenGlFrameBuffer::~OpenGlFrameBuffer()
{
#if GL_ARB_framebuffer_object
    if(_mfboid)
    {
        Gl::DeleteFramebuffers(1, &_mfboid);
        _mfboid = 0;
    }
#   ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "_mfboid is not initialized but we are in debug mode. UNREACHABLE normally. ('" << getName() << "')." << std::endl;
        throw OpenGlDeleteFrameBuffer(getName() + " (glDeleteFrameBuffers)");
    }
#   endif
    
#elif GL_EXT_framebuffer_object
    
    if(_mfboid)
    {
        Gl::DeleteFramebuffers(1, &_mfboid);
        _mfboid = 0;
    }
#   ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "_mfboid is not initialized but we are in debug mode. UNREACHABLE normally. ('" << getName() << "')." << std::endl;
        throw OpenGlDeleteFrameBuffer(getName() + " (glDeleteFrameBuffersEXT)");
    }
#   endif

#endif // GL_ARB_framebuffer_object
}

void OpenGlFrameBuffer::bind() const
{
#if GL_ARB_framebuffer_object
    if(_mfboid)
    {
        Gl::BindFramebuffer(GL_DRAW_BUFFER, _mfboid);
    }
#   ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "('" << getName() << "') _mfboid is not initialized but tried to bind it anyway !" << std::endl;
        throw OpenGlBindFrameBuffer(getName() + " (glBindFrameBuffer)");
    }
#   endif
    
#elif GL_EXT_framebuffer_object
    
    if(_mfboid)
    {
        Gl::BindFramebuffer(GL_DRAW_BUFFER, _mfboid);
    }
#   ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "('" << getName() << "') _mfboid is not initialized but tried to bind it anyway !" << std::endl;
        throw OpenGlBindFrameBuffer(getName() + " (glBindFrameBufferEXT)");
    }
#   endif
    
#endif
}

void OpenGlFrameBuffer::unbind() const
{
    Gl::BindFramebuffer(GL_DRAW_BUFFER, 0);
}

GLuint GreAttachementToOpenGlAttachement(FrameBufferPrivate::Attachement attachement)
{
#if GL_ARB_framebuffer_object
    
    GLint glMaxColorAttachement;
    Gl::GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &glMaxColorAttachement);
    
    if(attachement == FrameBufferPrivate::Attachement::Depth)
        return GL_DEPTH_ATTACHMENT;
    if(attachement == FrameBufferPrivate::Attachement::Stencil)
        return GL_STENCIL_ATTACHMENT;

    if(attachement < FrameBufferPrivate::Attachement::Depth &&
       (uint32_t) attachement > 0 &&
       (uint32_t) attachement < glMaxColorAttachement)
        return GL_COLOR_ATTACHMENT0 + (uint32_t) attachement;
    
#   ifdef GreIsDebugMode
    if((uint32_t) attachement >= glMaxColorAttachement)
    {
        GreDebugPretty() << "Sorry, Color attachement " << (uint32_t) attachement << " not present on this Machine." << std::endl;
        throw OpenGlMaxColorAttachement(std::string("Max Color Attachement reached (") + std::to_string(glMaxColorAttachement) + ").");
    }
#   endif
    
#elif GL_EXT_framebuffer_object
    
    GLint glMaxColorAttachement;
    Gl::GetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &glMaxColorAttachement);
    
    if(attachement == FrameBufferPrivate::Attachement::Depth)
        return GL_DEPTH_ATTACHMENT_EXT;
    if(attachement == FrameBufferPrivate::Attachement::Stencil)
        return GL_STENCIL_ATTACHMENT_EXT;
    
    if(attachement < FrameBufferPrivate::Attachement::Depth &&
       (uint32_t) attachement > 0 &&
       (uint32_t) attachement < glMaxColorAttachement)
        return GL_COLOR_ATTACHMENT0_EXT + (uint32_t) attachement;
    
#   ifdef GreIsDebugMode
    if((uint32_t) attachement >= glMaxColorAttachement)
    {
        GreDebugPretty() << "Sorry, Color attachement " << (uint32_t) attachement << " not present on this Machine." << std::endl;
        throw OpenGlMaxColorAttachement(std::string("Max Color Attachement reached (") + std::to_string(glMaxColorAttachement) + ").");
    }
#   endif
    
#endif
    
    return 0;
}

void OpenGlFrameBuffer::attachTexture(FrameBufferPrivate::Attachement attachement, const Texture &texture)
{
    GLuint glAttach = GreAttachementToOpenGlAttachement(attachement);
#ifdef GreIsDebugMode
    if(glAttach == 0)
    {
        GreDebugPretty() << "Sorry, bad attachement given : " << (uint32_t) attachement << "." << std::endl;
        throw OpenGlGreAttachementToGl(getName() + " (GreAttachementToOpenGlAttachement)");
    }
#endif
    
    if(_mfboid)
    {
        Gl::BindFramebuffer(GL_DRAW_BUFFER, _mfboid);
        Gl::FramebufferTexture(GL_DRAW_BUFFER, glAttach,*((GLuint*) texture.getCustomData("OpenGlId")), 0);
    }
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "('" << getName() << "') _mfboid is not initialized but tried to bind it anyway !" << std::endl;
        throw OpenGlBindFrameBuffer(getName() + " (glFrameBufferTexture)");
    }
#endif
}

// ---------------------------------------------------------------------------------------------------

OpenGlFrameBufferLoader::OpenGlFrameBufferLoader()
: FrameBufferLoader()
{
    
}

OpenGlFrameBufferLoader::~OpenGlFrameBufferLoader()
{
    
}

Resource* OpenGlFrameBufferLoader::load(Resource::Type type, const std::string &name) const
{
    return new OpenGlFrameBuffer(name);
}

ResourceLoader* OpenGlFrameBufferLoader::clone() const
{
    return new OpenGlFrameBufferLoader();
}


