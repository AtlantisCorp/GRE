//////////////////////////////////////////////////////////////////////
//
//  GlXContext.cpp
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

#include "GlXContext.h"
#include "GlXApplication.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

GlXContext::GlXContext ( const std::string& name , GLint major , GLint minor , GLXContext sharedctxt )
: Gre::RenderContextPrivate ( name )
, iGlContext(0)
, iGlWindow(0)
{
    GreResourceAutolock ;
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( app->getXDisplay() , DefaultScreen( app->getXDisplay() ) );
    
    // NOTE: It is not necessary to create or make current to a context before calling glXGetProcAddressARB .
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    
    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) || !glXCreateContextAttribsARB )
    {
        GreDebugPretty() << "'glXCreateContextAttribsARB()' not found. Using old-style GLX context." << Gre::gendl;
        iGlContext = glXCreateNewContext(app->getXDisplay(), GetBestFBConfig(app->getXDisplay()), GLX_RGBA_TYPE, sharedctxt, True);
    }
    
    else
    {
        int ctxt_attribs [] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB , major ,
            GLX_CONTEXT_MINOR_VERSION_ARB , minor ,
            None
        } ;
        
        iGlContext = glXCreateContextAttribsARB ( app->getXDisplay() , GetBestFBConfig(app->getXDisplay()) , sharedctxt , True , ctxt_attribs ) ;
    }
    
    if ( !iGlContext )
    {
        GreDebugPretty() << "Failed to create GLX OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Failed to create GLX Context." ) ;
    }
}

GlXContext::~GlXContext() noexcept ( false )
{
    GreResourceAutolock ;
    
    if ( !iGlContext )
    {
        GreDebugPretty() << "Failed to access GLX OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Failed to access GLX Context." ) ;
    }
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    glXMakeCurrent ( app->getXDisplay() , 0 , 0 ) ;
    glXDestroyContext ( app->getXDisplay() , iGlContext ) ;
}

void GlXContext::bind() const
{
    GreResourceAutolock ;
    
    if ( !iGlContext )
    {
        GreDebugPretty() << "Failed to access GLX OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Failed to access GLX Context." ) ;
    }
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    if ( !iGlWindow )
    {
        GreDebugPretty() << "No GlXWindow associated with this RenderContext." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No GlXWindow." ) ;
    }
    
    glXMakeCurrent(app->getXDisplay(), iGlWindow, iGlContext) ;
}

void GlXContext::unbind() const
{
    GreResourceAutolock ;
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    glXMakeCurrent(app->getXDisplay(), 0, 0) ;
}

void GlXContext::flush()
{
    GreResourceAutolock ;
    
    if ( !iGlContext )
    {
        GreDebugPretty() << "Failed to access GLX OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Failed to access GLX Context." ) ;
    }
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    if ( !iGlWindow )
    {
        GreDebugPretty() << "No GlXWindow associated with this RenderContext." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No GlXWindow." ) ;
    }
    
    glXSwapBuffers(app->getXDisplay(), iGlWindow);
}

const void* GlXContext::getProperty(const std::string &name) const
{
    GreResourceAutolock ;
    
    if ( name == "GLXContext" )
        return & iGlContext ;
    if ( name == "GLXDrawable" )
        return & iGlWindow ;
    return Gre::RenderContextPrivate::getProperty(name) ;
}

void GlXContext::setClearBuffers(const Gre::RenderContextClearBuffers &clearbuffs)
{
    
}

void GlXContext::setClearColor(const Gre::Color &color)
{
    GreResourceAutolock ;
    glClearColor( color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() ) ;
}

Gre::RenderFramebufferHolder GlXContext::iCreateFramebuffer(const std::string &name) const
{
    return Gre::RenderFramebufferHolder ( nullptr ) ;
}

void GlXContext::onWindowAttachContextEvent(const Gre::WindowAttachContextEvent &e)
{
    GreResourceAutolock ;
    
    if ( iGlWindow )
    {
        GreDebugPretty() << "'iGlWindow' already set. Please ensure the GlXContext receives the 'WindowDetachContextEvent'"
        << " before the 'WindowAttachContextEvent'." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Invalid Event processing." ) ;
    }
    
    Gre::ResourceHolder holder = e.getEmitter().lock() ;
    if ( !holder.isInvalid() )
    {
        Gre::WindowPrivate * win = reinterpret_cast<Gre::WindowPrivate*>( holder.get() ) ;
        iGlWindow = * ((const GLXWindow*) win->getProperty("GLXWindow"));
    }
}

void GlXContext::onWindowDetachContextEvent(const Gre::WindowDetachContextEvent &e)
{
    GreResourceAutolock ;
    iGlWindow = 0 ;
}

void GlXContext::onWindowWillCloseEvent(const Gre::WindowWillCloseEvent &e)
{
    GreResourceAutolock ;
    
    if ( !iGlContext )
    {
        GreDebugPretty() << "Failed to access GLX OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Failed to access GLX Context." ) ;
    }
    
    GlXApplication * app = GlXApplication::GetApplication() ;
    
    if ( !app )
    {
        GreDebugPretty() << "No Global Application found." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No Global Application." ) ;
    }
    
    if ( !iGlWindow )
    {
        GreDebugPretty() << "No GlXWindow associated with this RenderContext." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "No GlXWindow." ) ;
    }
    
    glXMakeCurrent(app->getXDisplay(), 0, 0);
    glXDestroyContext(app->getXDisplay(), iGlContext);
    iGlContext = 0;
    iGlWindow = 0;
}
