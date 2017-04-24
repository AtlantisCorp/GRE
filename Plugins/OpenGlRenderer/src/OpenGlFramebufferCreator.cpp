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
    if ( att == Gre::RenderFramebufferAttachement::Depth ) return GL_DEPTH_ATTACHMENT ;
    else if ( att == Gre::RenderFramebufferAttachement::Stencil ) return GL_STENCIL_ATTACHMENT ;
    else if ( att == Gre::RenderFramebufferAttachement::DepthStencil ) return GL_DEPTH_STENCIL_ATTACHMENT ;

    int color = (int) att - (int) Gre::RenderFramebufferAttachement::Color0 ;

    if ( color < GL_MAX_COLOR_ATTACHMENTS && color >= 0 )
    return GL_COLOR_ATTACHMENT0 + color ;

    return GL_INVALID_ENUM ;
}

GLenum translateGlDrawBuffer ( const Gre::RenderColorBuffer & buffer )
{
    if ( buffer == Gre::RenderColorBuffer::FrontLeft ) return GL_FRONT_LEFT ;
    else if ( buffer == Gre::RenderColorBuffer::FrontRight ) return GL_FRONT_RIGHT ;
    else if ( buffer == Gre::RenderColorBuffer::BackLeft ) return GL_BACK_LEFT ;
    else if ( buffer == Gre::RenderColorBuffer::BackRight ) return GL_BACK_RIGHT ;
    else if ( buffer == Gre::RenderColorBuffer::Front ) return GL_FRONT ;
    else if ( buffer == Gre::RenderColorBuffer::Back ) return GL_BACK ;
    else if ( buffer == Gre::RenderColorBuffer::Left ) return GL_LEFT ;
    else if ( buffer == Gre::RenderColorBuffer::Right ) return GL_RIGHT ;
    else if ( buffer == Gre::RenderColorBuffer::FrontAndBack ) return GL_FRONT_AND_BACK ;

    int color = (int) buffer - (int) Gre::RenderColorBuffer::Color0 ;

    if ( color < GL_MAX_COLOR_ATTACHMENTS && color >= 0 )
    return GL_COLOR_ATTACHMENT0 + color ;

    return GL_NONE ;
}

std::string translateGlFramebufferError ( GLenum error )
{
    if ( error == GL_FRAMEBUFFER_UNDEFINED ) return "GL_FRAMEBUFFER_UNDEFINED" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ) return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHEMENT" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ) return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHEMENT" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ) return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ) return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" ;
    if ( error == GL_FRAMEBUFFER_UNSUPPORTED ) return "GL_FRAMEBUFFER_INCOMPLETE_UNSUPPORTED" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ) return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" ;
    if ( error == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ) return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" ;

    return "GL_NONE" ;
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
        // Creates an hypothetic renderbuffer for framebuffer.
        setAttachmentBuffer (Gre::RenderFramebufferAttachement::Color0 ,
                             { 1024 , 1024 } ,
                             Gre::InternalPixelFormat::RGB ) ;

        iWriteBuffer = Gre::RenderColorBuffer::Color0 ;
        iReadBuffer = Gre::RenderColorBuffer::Color0 ;
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
        glDrawBuffer(translateGlDrawBuffer(iWriteBuffer));
        glReadBuffer(translateGlDrawBuffer(iReadBuffer));
        iBinded = true ;

        if ( iViewport.getArea() )
        {
            //////////////////////////////////////////////////////////////////////
            // We use the set Viewport to change it.

            glGetIntegerv ( GL_VIEWPORT , &iGlViewport[0] ) ;
            glViewport(iViewport.left, iViewport.top, iViewport.width, iViewport.height);
        }
    }
}

void OpenGlFramebuffer::unbind() const
{
    GreAutolock ;

    if ( iGlFramebuffer )
    {
        //////////////////////////////////////////////////////////////////////
        // Restores the viewport if we changed it.

        if ( iViewport.getArea() )
        glViewport(iGlViewport[0], iGlViewport[1], iGlViewport[2], iGlViewport[3]);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        iBinded = false ;
    }
}

bool OpenGlFramebuffer::binded () const
{
    GreAutolock ; return iBinded ;
}

bool OpenGlFramebuffer::_bindAttachment ( const Gre::FramebufferAttachment & attachment ) const
{
    if ( !iGlFramebuffer )
    return false ;

    bool shouldunbind = false ;

    if ( !binded() )
    {
        bind () ;
        shouldunbind = true ;
    }

    bool result = true ;

    if ( attachment.type == Gre::RenderFramebufferAttachementType::Texture )
    result = _bindglTexture ( attachment ) ;

    else if ( attachment.type == Gre::RenderFramebufferAttachementType::Renderbuffer )
    result = _bindglRenderbuffer ( attachment ) ;

    if ( shouldunbind )
    unbind () ;

    return result ;
}

bool OpenGlFramebuffer::_bindglTexture ( const Gre::FramebufferAttachment & attachment ) const
{
    if ( attachment.texture.isInvalid() )
    return false ;

    GLenum target = translateGlTexture ( attachment.texture -> getType() ) ;
    GLuint texture = reinterpret_cast<const OpenGlTexture*>(attachment.texture.getObject())->getGlTexture() ;

    glBindTexture ( target , texture ) ;

    if ( target == GL_TEXTURE_1D )
    {
        glFramebufferTexture1D(GL_FRAMEBUFFER,
                               translateGlAttachement(attachment.layer),
                               target, texture, 0);
    }

    else if ( target == GL_TEXTURE_2D )
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               translateGlAttachement(attachment.layer),
                               target, texture, 0);
    }

    else if ( target == GL_TEXTURE_3D )
    {
        glFramebufferTexture3D(GL_FRAMEBUFFER,
                               translateGlAttachement(attachment.layer),
                               target, texture, 0, 0);
    }

    glBindTexture ( target , 0 ) ;
    return true ;
}

bool OpenGlFramebuffer::_bindglRenderbuffer ( const Gre::FramebufferAttachment & attachment ) const
{
    //////////////////////////////////////////////////////////////////////
    // Creates a Renderbuffer with given parameters.

    GLuint renderbuffer ;
    glGenRenderbuffers ( 1 , &renderbuffer ) ;
    glBindRenderbuffer ( GL_RENDERBUFFER , renderbuffer ) ;

    glRenderbufferStorage (GL_RENDERBUFFER ,
                           translateGlInternalPixelFormat(attachment.renderbuffer.format) ,
                           attachment.renderbuffer.size.first ,
                           attachment.renderbuffer.size.second) ;

    //////////////////////////////////////////////////////////////////////
    // Attaches the renderbuffer to the framebuffer.

    glFramebufferRenderbuffer (GL_FRAMEBUFFER , translateGlAttachement(attachment.layer) ,
                               GL_RENDERBUFFER , renderbuffer) ;

    //////////////////////////////////////////////////////////////////////
    // Unbinds the renderbuffer and registers it.

    glBindRenderbuffer ( GL_RENDERBUFFER , 0 ) ;
    iRenderbuffers [attachment.layer] = renderbuffer ;

    return true ;
}

void OpenGlFramebuffer::_unbindAttachment ( const Gre::FramebufferAttachment & attachment ) const
{
    if ( attachment.type != Gre::RenderFramebufferAttachementType::Renderbuffer )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Finds the renderbuffer , destroy it and unset it from the framebuffer.

    bool shouldunbind = false ;

    if ( !binded() )
    {
        bind () ;
        shouldunbind = true ;
    }

    auto it = iRenderbuffers.find ( attachment.layer ) ;

    if ( it != iRenderbuffers.end() )
    {
        glDeleteRenderbuffers ( 1 , & it->second ) ;
        iRenderbuffers.erase ( it ) ;

        //////////////////////////////////////////////////////////////////////
        // Notes we don't have to set the framebuffer attachment to 0 . When deleting
        // a renderbuffer object , the bound framebuffer will automatically unset
        // it. Notes unbound framebuffer don't unset the renderbuffer.
    }

    if ( shouldunbind )
    unbind () ;
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
