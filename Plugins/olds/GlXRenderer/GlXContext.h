//////////////////////////////////////////////////////////////////////
//
//  GlXContext.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 23/10/2016.
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

#ifndef GlXContext_h
#define GlXContext_h

#include "GlX11Headers.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class GlXContext : public Gre::RenderContextPrivate
{
public:
    
    POOLED ( Gre::Pools::Render )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GlXContext ( const std::string& name , GLint glmajor , GLint glminor , GLXContext sharedctxt );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~GlXContext() noexcept ( false );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderContext.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    void flush();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a custom property defined by given name.
    //////////////////////////////////////////////////////////////////////
    const void* getProperty ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffers the RenderContext should clear at each
    /// loop. Buffers available for clearing are listed in 'RenderContextClearBuffer'
    /// enum. Notes one instance of any RenderContextClearBuffer should be listed.
    //////////////////////////////////////////////////////////////////////
    void setClearBuffers ( const Gre::RenderContextClearBuffers& clearbuffs );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a Color used when clearing the ColorBufferBit.
    //////////////////////////////////////////////////////////////////////
    void setClearColor ( const Gre::Color& color );
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method that creates a RenderFramebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::RenderFramebufferHolder iCreateFramebuffer ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has attached a new RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowAttachContextEvent ( const Gre::WindowAttachContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has detached a new RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowDetachContextEvent ( const Gre::WindowDetachContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object will be closed.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowWillCloseEvent ( const Gre::WindowWillCloseEvent& e ) ;
    
protected:
    
    /// @brief GLX Context.
    GLXContext iGlContext ;
    
    /// @brief GLXDrawable associated with this context. ( Generally a window. )
    GLXDrawable iGlWindow ;
};

#endif /* GlXContext_h */
