//////////////////////////////////////////////////////////////////////
//
//  DarwinGlRenderFramebuffer.h
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

#ifndef DarwinGlRenderFramebuffer_h
#define DarwinGlRenderFramebuffer_h

#include "DarwinGlHeader.h"
#include "FrameBuffer.h"

namespace DarwinGl
{
    //////////////////////////////////////////////////////////////////////
    /// @brief RenderFramebuffer for OpenGl on macOS.
    //////////////////////////////////////////////////////////////////////
    class DLL_PUBLIC DarwinGlRenderFramebuffer : public Gre::RenderFramebufferPrivate
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlRenderFramebuffer ( const std::string& name );
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual ~DarwinGlRenderFramebuffer () ;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Make the FrameBuffer usable.
        //////////////////////////////////////////////////////////////////////
        virtual void bind() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Make the FrameBuffer unusable.
        //////////////////////////////////////////////////////////////////////
        virtual void unbind() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Attachs the given texture object to the given Attachement.
        //////////////////////////////////////////////////////////////////////
        virtual void setAttachement(const Gre::RenderFramebufferAttachement& attachement, Gre::TextureHolder& holder);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the Maximum Width a Framebuffer Attachement can
        /// have.
        //////////////////////////////////////////////////////////////////////
        virtual int getMaximumWidth() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the Maximum Height a Framebuffer Attachement can
        /// have.
        //////////////////////////////////////////////////////////////////////
        virtual int getMaximumHeight() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the Maximum number of Color Attachement a Framebuffer
        /// can have.
        //////////////////////////////////////////////////////////////////////
        virtual int getMaximumColorAttachementCount() const;
        
    protected:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Calls the appropriate function from given args. This function
        /// is not thread-safe , and should be call only from 'setAttachement'.
        //////////////////////////////////////////////////////////////////////
        void iFramebufferTexture ( Gre::TextureHolder& holder, const Gre::RenderFramebufferAttachement& attachement );
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the appropriate GLenum Attachement.
        //////////////////////////////////////////////////////////////////////
        GLenum iGetAttachement ( const Gre::RenderFramebufferAttachement& attachement ) const;
        
    protected:
        
        /// @brief OpenGl Object.
        GLuint iFramebuffer;
    };
}

#endif /* DarwinGlRenderFramebuffer_h */
