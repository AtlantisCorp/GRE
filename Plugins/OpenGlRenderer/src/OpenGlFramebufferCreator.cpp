//////////////////////////////////////////////////////////////////////
//
//  OpenGlFramebufferCreator.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 10/03/2017.
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

#include "OpenGlRenderer.h"

// ---------------------------------------------------------------------------------------------------
// OpenGl Translations functions

GLenum translateGlAttachement ( const Gre::RenderFramebufferAttachement& att )
{
    if ( (int) att <= (int) Gre::RenderFramebufferAttachement::Color31 )
        return GL_COLOR_ATTACHMENT0 + (int) att ;
    else if ( att == Gre::RenderFramebufferAttachement::Depth )
        return GL_DEPTH_ATTACHMENT ;
    else if ( att == Gre::RenderFramebufferAttachement::Stencil )
        return GL_STENCIL_ATTACHMENT ;
    
    return GL_INVALID_ENUM ;
}

// ---------------------------------------------------------------------------------------------------
// OpenGl Framebuffer

OpenGlFramebuffer::OpenGlFramebuffer ( const std::string & name )
: Gre::RenderFramebuffer(name) , iGlFramebuffer(0)
{
    glGenFramebuffers(1, &iGlFramebuffer) ;
    
#ifdef GreIsDebugMode
    if ( !iGlFramebuffer ) {
        GreDebug("[WARN] Can't create OpenGl Framebuffer.") << Gre::gendl ;
    }
#endif
    
    if ( iGlFramebuffer )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, iGlFramebuffer) ;
        glDrawBuffer(GL_NONE) ;
        glReadBuffer(GL_NONE) ;
        glBindFramebuffer(GL_FRAMEBUFFER, 0) ;
    }
    
}

OpenGlFramebuffer::~OpenGlFramebuffer() noexcept ( false )
{
    if ( iGlFramebuffer )
        glDeleteFramebuffers(1, &iGlFramebuffer) ;
}

void OpenGlFramebuffer::bind() const
{
    GreAutolock ;
    
    if ( iGlFramebuffer )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, iGlFramebuffer);
    }
}

void OpenGlFramebuffer::unbind() const
{
    GreAutolock ;
    
    if ( iGlFramebuffer )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void OpenGlFramebuffer::setAttachement(const Gre::RenderFramebufferAttachement &attachement, Gre::TextureHolder &holder)
{
    if ( iGlFramebuffer && !holder.isInvalid() )
    {
        GLenum textarget = translateGlTexture(holder->getType()) ;
        GLuint texture = reinterpret_cast<OpenGlTexture*>(holder.getObject())->getGlTexture() ;
        
        glBindTexture(textarget, texture);
        glBindFramebuffer(GL_FRAMEBUFFER, iGlFramebuffer);
        
        if ( textarget == GL_TEXTURE_1D )
        {
            glFramebufferTexture1D(GL_FRAMEBUFFER,
                                   translateGlAttachement(attachement),
                                   textarget, texture, 0);
        }
        
        else if ( textarget == GL_TEXTURE_2D )
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   translateGlAttachement(attachement),
                                   textarget, texture, 0);
        }
        
        else if ( textarget == GL_TEXTURE_3D )
        {
            glFramebufferTexture3D(GL_FRAMEBUFFER,
                                   translateGlAttachement(attachement),
                                   textarget, texture, 0, 0);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(textarget, 0);
        
        Gre::RenderFramebuffer::setAttachement(attachement, holder);
    }
}

// ---------------------------------------------------------------------------------------------------

OpenGlFramebufferCreator::OpenGlFramebufferCreator ( const OpenGlRenderer* parent )
: Gre::RenderFramebufferInternalCreator() , iRenderer(parent)
{
    
}

OpenGlFramebufferCreator::~OpenGlFramebufferCreator()
{
    
}

Gre::RenderFramebuffer* OpenGlFramebufferCreator::load(const std::string &name, const Gre::ResourceLoaderOptions &options) const
{
    return new OpenGlFramebuffer (name) ;
}


