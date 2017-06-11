//////////////////////////////////////////////////////////////////////
//
//  X11OpenGlContext.cpp
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

#include "X11OpenGlContext.h"

// -----------------------------------------------------------------------------
// GLX Constants.

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

// -----------------------------------------------------------------------------
// GlX Context Utilities.

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;

    char buffer [256] ;
    XGetErrorText( dpy , ev->error_code , buffer , 255 ) ;

    GreDebug( "[ERRO] X11 Error (" ) << (int) ev->error_code << ") : " << (const char*) buffer << Gre::gendl ;

    return 0;
}

GLXContext CreateGlXContext( Display* display , GLXFBConfig bestFbc , int glmajor , int glminor , const Gre::ResourceLoaderOptions& ops )
{
	//////////////////////////////////////////////////////////////////////
	// Gets GLX Extension list.

	const char* glExts = glXQueryExtensionsString( display , DefaultScreen(display) ) ;

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0 ;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte*) "glXCreateContextAttribsARB" ) ;

	GLXContext ctxt = 0 ;

	//////////////////////////////////////////////////////////////////////
	// Installs error handler.

	ctxErrorOccurred = false ;
	int (*oldHandler) (Display*, XErrorEvent*) = XSetErrorHandler ( &ctxErrorHandler ) ;

	//////////////////////////////////////////////////////////////////////
	// Tries to get GLX Context.

	int context_attribs [] =
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB , glmajor ,
		GLX_CONTEXT_MINOR_VERSION_ARB , glminor ,
		GLX_CONTEXT_PROFILE_MASK_ARB , GLX_CONTEXT_CORE_PROFILE_BIT_ARB ,
		None
	};

	ctxt = glXCreateContextAttribsARB ( display , bestFbc , 0 , True , context_attribs ) ;
	XSync ( display , False ) ;

	if ( ctxErrorOccurred || !ctxt )
	{
		GreDebug( "[WARN] glXCreateContextAttribsARB() failed." ) << Gre::gendl ;
		XSetErrorHandler( oldHandler ) ;
		return 0 ;
	}

	GreDebug( "[INFO] GLX Context created (" ) << glmajor << "." << glminor << ")." << Gre::gendl ;
	return ctxt ;
}

void DestroyGlXContext(Display* display , GLXContext ctxt)
{
	glXMakeCurrent(display, None, None);
	glXDestroyContext(display, ctxt);
}

// -----------------------------------------------------------------------------
// X11Context implementation.

X11Context::X11Context(const std::string& name, ::Display* dpy, ::GLXDrawable win, ::GLXContext ctxt): RenderContext(name)
{
    iX11Display  = dpy ;
    iX11Drawable = win ;
    iX11Context  = ctxt ;
}

X11Context::~X11Context()
{

}

void X11Context::bind() const
{
    GreAutolock ;

    if ( !iX11Context )
    return ;

    glXMakeCurrent( iX11Display , iX11Drawable , iX11Context ) ;
}

void X11Context::unbind() const
{
    GreAutolock ;

    if ( !iX11Context )
    return ;

    glXMakeCurrent( iX11Display , None , None ) ;
}

void X11Context::flush() const
{
    GreAutolock ;

    if ( !iX11Context )
    return ;

    glXSwapBuffers( iX11Display , iX11Drawable ) ;
}
