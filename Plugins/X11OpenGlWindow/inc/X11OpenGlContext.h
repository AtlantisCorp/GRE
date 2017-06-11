//////////////////////////////////////////////////////////////////////
//
//  X11OpenGlContext.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 17/04/2017.
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

#ifndef GRE_X11OPENGLCONTEXT_H
#define GRE_X11OPENGLCONTEXT_H

#include "X11OpenGl.h"

//////////////////////////////////////////////////////////////////////
/// @brief Creates a GLXContext with given informations. 
//////////////////////////////////////////////////////////////////////
GLXContext CreateGlXContext (Display* display , 
							 GLXFBConfig bestFbc , 
							 int glmajor ,
							 int glminor ,
							 const Gre::ResourceLoaderOptions& ops);
							 
//////////////////////////////////////////////////////////////////////
/// @brief Destroys given GLX Context.
//////////////////////////////////////////////////////////////////////
void DestroyGlXContext (Display* display , GLXContext ctxt) ;

//////////////////////////////////////////////////////////////////////
/// @brief A Container to help manipulating X11 OpenGL Contexts.
/// This object does not destroy or create any Rendering Context. This 
/// job is done by the Window Loader when creating a new Window.
//////////////////////////////////////////////////////////////////////
class X11Context : public Gre::RenderContext
{
public:
    
    POOLED ( Gre::Pools::Render ) 
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    X11Context ( const std::string& name , ::Display* dpy , ::GLXDrawable win , ::GLXContext ctxt ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~X11Context () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void flush() const ;
    
protected:
    
    /// @brief X11 Display.
    ::Display* iX11Display ;
    
    /// @brief X11 Drawable associated to this context.
    ::GLXDrawable iX11Drawable ;
    
    /// @brief X11 Context.
    ::GLXContext iX11Context ;
};

#endif // GRE_X11OPENGLCONTEXT_H
