//
//  OpenGlFrameBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/01/2016.
//
//

#include "OpenGlFrameBuffer.h"
#include "OpenGlRenderContext.h"

typedef GreExceptionWithText OpenGlGenFrameBuffer;
typedef GreExceptionWithText OpenGlDeleteFrameBuffer;
typedef GreExceptionWithText OpenGlBindFrameBuffer;
typedef GreExceptionWithText OpenGlMaxColorAttachement;
typedef GreExceptionWithText OpenGlGreAttachementToGl;
typedef GreExceptionWithText OpenGlNoFramebuffer;

OpenGlFrameBuffer::OpenGlFrameBuffer(const std::string& name)
: FrameBufferPrivate(name), _mfboid(0)
{
    // The Gl id should be generated when using the Framebuffer, in order to support
    // multi-context id generation.
    
    //construct();
}

OpenGlFrameBuffer::~OpenGlFrameBuffer()
{
    
}

void OpenGlFrameBuffer::construct()
{
    if(glGlobalContext)
    {
        if(glGlobalContext->getGl().IsFunctionSupported(glGlobalContext->getGl().GenFramebuffers))
        {
            // Collecting Maximum Properties.
            glGlobalContext->getGl().GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_mMaxColorAttachement);
            
#ifdef GL_MAX_FRAMEBUFFER_WIDTH
            glGlobalContext->getGl().GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &_mMaxWidth);
#endif
#ifdef GL_MAX_FRAMEBUFFER_HEIGHT
            glGlobalContext->getGl().GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &_mMaxHeight);
#endif
#ifdef GL_MAX_FRAMEBUFFER_SAMPLES
            glGlobalContext->getGl().GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &_mMaxSamples);
#endif
            
            GLuint newfbo = 0;
            glGlobalContext->getGl().GenFramebuffers(1, &newfbo);
            
#ifdef GreIsDebugMode
            if(!_mfboid)
            {
                GreDebugPretty() << "Oh no, the OpenGlFrameBuffer object '" << getName() << "' could not be generated..." << Gre::gendl;
                throw OpenGlGenFrameBuffer(getName() + " (glGenFrameBuffers)");
            }
#endif
            
            addId(glGlobalContext->getId(), newfbo);
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "No Framebuffer supported in this OpenGl implementation..." << Gre::gendl;
            throw OpenGlNoFramebuffer(getName() + " (No Framebuffer)");
        }
#endif
    }
}

void OpenGlFrameBuffer::destroy()
{
    if(glGlobalContext)
    {
        if(glGlobalContext->getGl().IsFunctionSupported(glGlobalContext->getGl().DeleteFramebuffers))
        {
            if(hasConstructedForContext(glGlobalContext->getId()))
            {
                GLuint fboid = getId(glGlobalContext->getId());
                glGlobalContext->getGl().DeleteFramebuffers(1, &fboid);
                removeId(glGlobalContext->getId());
            }
#ifdef GreIsDebugMode
            else
            {
                GreDebugPretty() << "_mfboid is not initialized but we are in debug mode. UNREACHABLE normally. ('" << getName() << "')." << Gre::gendl;
                throw OpenGlDeleteFrameBuffer(getName() + " (glDeleteFrameBuffers)");
            }
#endif
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "No Framebuffer supported in this OpenGl implementation..." << Gre::gendl;
            throw OpenGlNoFramebuffer(getName() + " (No Framebuffer)");
        }
#endif
    }
}

void OpenGlFrameBuffer::bind() const
{
    if(glGlobalContext)
    {
        if(glGlobalContext->getGl().IsFunctionSupported(glGlobalContext->getGl().BindFramebuffer))
        {
            if(hasConstructedForContext(glGlobalContext->getId()))
            {
                glGlobalContext->getGl().BindFramebuffer(GL_DRAW_BUFFER, getId(glGlobalContext->getId()));
            }
            
            else
            {
                // When binding, if not generated, we construct the Gl id and relaunch the function.
                //construct();
            }
            
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "No Framebuffer supported in this OpenGl implementation..." << Gre::gendl;
            throw OpenGlNoFramebuffer(getName() + " (No Framebuffer)");
        }
#endif
    }
}

void OpenGlFrameBuffer::unbind() const
{
    if(glGlobalContext)
    {
        if(glGlobalContext->getGl().IsFunctionSupported(glGlobalContext->getGl().BindFramebuffer))
        {
            glGlobalContext->getGl().BindFramebuffer(GL_DRAW_BUFFER, 0);
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "No Framebuffer supported in this OpenGl implementation..." << Gre::gendl;
            throw OpenGlNoFramebuffer(getName() + " (No Framebuffer)");
        }
#endif
    }
}

GLuint GreAttachementToOpenGlAttachement(FrameBufferPrivate::Attachement attachement)
{
#ifndef GL_MAX_COLOR_ATTACHMENTS
#   ifdef GL_MAX_COLOR_ATTACHMENTS_EXT
#       define GL_MAX_COLOR_ATTACHMENTS GL_MAX_COLOR_ATTACHMENTS_EXT
#       define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_EXT
#       define GL_STENCIL_ATTACHMENT GL_STENCIL_ATTACHMENT_EXT
#       define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#   endif
#endif
    
#ifndef GL_MAX_COLOR_ATTACHMENTS
#   ifdef GreIsDebugMode
    throw OpenGlNoFramebuffer("GreAttachementToOpenGlAttachement");
#   endif
    return 0;
#endif
    
    GLint glMaxColorAttachement;
    glGlobalContext->getGl().GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &glMaxColorAttachement);
    
    if(attachement == FrameBufferPrivate::Attachement::Depth)
        return GL_DEPTH_ATTACHMENT;
    if(attachement == FrameBufferPrivate::Attachement::Stencil)
        return GL_STENCIL_ATTACHMENT;

    if(attachement < FrameBufferPrivate::Attachement::Depth &&
       (uint32_t) attachement > 0 &&
       (uint32_t) attachement < glMaxColorAttachement)
        return GL_COLOR_ATTACHMENT0 + (uint32_t) attachement;
    
#ifdef GreIsDebugMode
    if((uint32_t) attachement >= glMaxColorAttachement)
    {
        GreDebugPretty() << "Sorry, Color attachement " << (uint32_t) attachement << " not present on this Machine." << Gre::gendl;
        throw OpenGlMaxColorAttachement(std::string("Max Color Attachement reached (") + std::to_string(glMaxColorAttachement) + ").");
    }
#endif
    
    return 0;
}

void OpenGlFrameBuffer::attachTexture(FrameBufferPrivate::Attachement attachement, const Texture &texture)
{
    if(glGlobalContext->getGl().IsFunctionSupported(glGlobalContext->getGl().FramebufferTexture))
    {
        GLuint glAttach = GreAttachementToOpenGlAttachement(attachement);
#ifdef GreIsDebugMode
        if(glAttach == 0)
        {
            GreDebugPretty() << "Sorry, bad attachement given : " << (uint32_t) attachement << "." << Gre::gendl;
            throw OpenGlGreAttachementToGl(getName() + " (GreAttachementToOpenGlAttachement)");
        }
#endif
        
        if(_mfboid)
        {
#ifndef GreIsDebugMode
            glGlobalContext->getGl().BindFramebuffer(GL_DRAW_BUFFER, _mfboid);
            glGlobalContext->getGl().FramebufferTexture(GL_DRAW_BUFFER, glAttach, *((GLuint*) texture.getCustomData("OpenGlId")), 0);
#else
            GLuint glTextureId = *((GLuint*) texture.getCustomData("OpenGlId"));
            if(glTextureId == 0)
            {
                GreDebugPretty() << "('" << getName() << "') glTextureId is invalid !" << Gre::gendl;
                throw OpenGlBindFrameBuffer(getName() + " (glFrameBufferTexture)");
            }
            
            else
            {
                glGlobalContext->getGl().BindFramebuffer(GL_DRAW_BUFFER, _mfboid);
                glGlobalContext->getGl().FramebufferTexture(GL_DRAW_BUFFER, glAttach, glTextureId, 0);
            }
#endif
        }
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "('" << getName() << "') _mfboid is not initialized but tried to bind it anyway !" << Gre::gendl;
            throw OpenGlBindFrameBuffer(getName() + " (glFrameBufferTexture)");
        }
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No Framebuffer supported in this OpenGl implementation..." << Gre::gendl;
        throw OpenGlNoFramebuffer(getName() + " (No Framebuffer)");
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


