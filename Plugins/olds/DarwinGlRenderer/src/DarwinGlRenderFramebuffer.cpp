//////////////////////////////////////////////////////////////////////
//
//  DarwinGlRenderFramebuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 03/10/2016.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#include "DarwinGlRenderFramebuffer.h"

namespace DarwinGl
{
    DarwinGlRenderFramebuffer::DarwinGlRenderFramebuffer(const std::string& name)
    : Gre::RenderFramebufferPrivate(name)
    , iFramebuffer(0)
    {
        GreResourceAutolock ;
        
        // Creating the RenderFramebuffer OpenGl object.
        glGenFramebuffers(1, &iFramebuffer);
    }
    
    DarwinGlRenderFramebuffer::~DarwinGlRenderFramebuffer()
    {
        GreResourceAutolock ;
        
        if ( iFramebuffer != 0 )
        {
            glDeleteFramebuffers(1, &iFramebuffer);
        }
    }
    
    void DarwinGlRenderFramebuffer::bind() const
    {
        GreResourceAutolock ;
        
        if ( iFramebuffer )
        {
            glBindFramebuffer(GL_FRAMEBUFFER, iFramebuffer);
        }
    }
    
    void DarwinGlRenderFramebuffer::unbind() const
    {
        GreResourceAutolock ;
        
        if ( iFramebuffer )
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
    
    void DarwinGlRenderFramebuffer::setAttachement(const Gre::RenderFramebufferAttachement &attachement, Gre::TextureHolder &holder)
    {
        GreResourceAutolock ;
        
        // Here , we will try to attach the given Texture to the Framebuffer using OpenGl
        // stuff. On success , we call the RenderFramebuffer::setAttachement basic function.
        
        if ( iFramebuffer )
        {
            glBindFramebuffer(GL_FRAMEBUFFER, iFramebuffer);
            iFramebufferTexture(holder, attachement);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            Gre::RenderFramebufferPrivate::setAttachement(attachement, holder);
        }
    }
    
    int DarwinGlRenderFramebuffer::getMaximumWidth() const
    {
        return 0;
    }
    
    int DarwinGlRenderFramebuffer::getMaximumHeight() const
    {
        return 0;
    }
    
    int DarwinGlRenderFramebuffer::getMaximumColorAttachementCount() const
    {
        GreResourceAutolock ;
        int res; glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
        return res;
    }
    
    void DarwinGlRenderFramebuffer::iFramebufferTexture(Gre::TextureHolder &holder, const Gre::RenderFramebufferAttachement &attachement)
    {
        if ( !holder.isInvalid() )
        {
            if ( holder->getType() == Gre::TextureType::TwoDimension )
            {
                GLenum target = GL_FRAMEBUFFER;
                GLenum attach = iGetAttachement(attachement);
                GLenum textarget = GL_TEXTURE_2D;
                GLuint texture = * ((const GLuint*) holder->getProperty("GlTexture"));
                GLint level = 0;
                
                glFramebufferTexture2D(target, attach, textarget, texture, level);
            }
        }
    }
    
    GLenum DarwinGlRenderFramebuffer::iGetAttachement(const Gre::RenderFramebufferAttachement &attachement) const
    {
        if ( (unsigned int) attachement <= (unsigned int) Gre::RenderFramebufferAttachement::Color31 )
        {
            return GL_COLOR_ATTACHMENT0 + (unsigned int) attachement;
        }
        
        if ( attachement == Gre::RenderFramebufferAttachement::Depth )
        {
            return GL_DEPTH_ATTACHMENT;
        }
        
        if ( attachement == Gre::RenderFramebufferAttachement::Stencil )
        {
            return GL_STENCIL_ATTACHMENT;
        }
        
        return GL_INVALID_ENUM;
    }
}
