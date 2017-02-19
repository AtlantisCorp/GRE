//////////////////////////////////////////////////////////////////////
//
//  GlXWindow.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 17/10/2016.
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

#include "GlXWindow.h"
#include "GlXApplication.h"

static int visual_attribs[] =
{
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    //GLX_SAMPLE_BUFFERS  , 1,
    //GLX_SAMPLES         , 4,
    None
};

static GLXFBConfig iBestFbc = 0 ;

extern "C" GLXFBConfig GetBestFBConfig ( Display* display )
{
    if ( !iBestFbc )
    {
        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
        
        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
        
        if ( !fbc )
        {
            GreDebugPretty() << "'glXChooseFBConfig' failed." << Gre::gendl ;
            throw Gre::GreExceptionWithText ( "'glXChooseFBConfig' failed." ) ;
        }
        
        GreDebugPretty() << "Found '" << fbcount << "' matching FB configs." << Gre::gendl ;
        
        int i;
        for ( i = 0 ; i < fbcount ; ++i )
        {
            XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
            
            if ( vi )
            {
                int samp_buf, samples;
                
                glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );
                
                GreDebugPretty() << "  Matching fbconfig '" << i << "', visual ID 0x" << vi->visualid << " : SAMPLE_BUFFERS = " << samp_buf << ", SAMPLES = " << samples << "." << Gre::gendl ;
                
                if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) )
                    best_fbc = i, best_num_samp = samples;
                if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                    worst_fbc = i, worst_num_samp = samples;
            }
            
            XFree( vi );
        }
        
        GLXFBConfig bestFbc = fbc[ best_fbc ];
        XFree(fbc);
        
        iBestFbc = bestFbc ;
        return bestFbc ;
    }
    
    else
    {
        return iBestFbc ;
    }
}

GlXWindow::GlXWindow(const std::string & name, int x0, int y0, int wid, int height)
: Gre::WindowPrivate(name)
{
    GreResourceAutolock ;
    
    // Here we have to create the X11 Window objects .
    
    GlXApplication * GlobalApp = GlXApplication::GetApplication() ;
    
    if ( !GlobalApp )
    {
        GreDebugPretty() << "'GlobalApp' isn't initialized." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXWindow" , "'GlobalApp' isn't initialized." ) ;
    }
    
    int glxMajor , glxMinor ;
    
    if ( !glXQueryVersion(GlobalApp->getXDisplay(), &glxMajor, &glxMinor) )
    {
        GreDebugPretty() << "'glXQueryVersion' failed." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXWindow" , "'glXQueryVersion' failed." ) ;
    }
    
    if ( (glxMajor == 1 && glxMinor < 3) || (glxMajor < 1) )
    {
        GreDebugPretty() << "'glXVersion' is invalid ( < 1.3 )." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXWindow" , "'glXVersion' is invalid ( < 1.3 )." ) ;
    }
    
    // Getting Framebuffer Configs.
    GLXFBConfig bestfbc = GetBestFBConfig(GlobalApp->getXDisplay()) ;
    
    // Get a Visual.
    XVisualInfo * vi = glXGetVisualFromFBConfig(GlobalApp->getXDisplay(), bestfbc) ;
    
    if ( !vi )
    {
        GreDebugPretty() << "'glXGetVisualFromFBConfig' failed." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXWindow" , "'glXGetVisualFromFBConfig' failed." ) ;
    }
    
    GreDebugPretty() << "Chosen visual ID = '0x" << vi->visualid << "'." << Gre::gendl ;
    
    // Creating Color Map.
    
    XSetWindowAttributes swa ;
    Colormap cmap ;
    swa.colormap = cmap = XCreateColormap(GlobalApp->getXDisplay(),
                                          RootWindow(GlobalApp->getXDisplay(), vi->screen),
                                          vi->visual, AllocNone) ;
    swa.background_pixmap = None ;
    swa.border_pixel = 0 ;
    swa.event_mask = StructureNotifyMask ;
    
    // Creating Window.
    
    ::Window win = XCreateWindow(GlobalApp->getXDisplay(), RootWindow(GlobalApp->getXDisplay(), vi->screen),
                               x0, y0, wid, height, 0, vi->depth, InputOutput, vi->visual,
                               CWBorderPixel|CWColormap|CWEventMask, &swa);
    
    if ( !win )
    {
        GreDebugPretty() << "'XCreateWindow' failed." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXWindow" , "'XCreateWindow' failed." ) ;
    }
    
    iWindow = win ;
    iFBConfig = bestfbc ;
    iColorMap = cmap ;
    XFree(vi);
    
    // Window Managers Protocols.
    
    iwmDeleteMessage = XInternAtom(GlobalApp->getXDisplay(), "WM_DELETE_WINDOW", False) ;
    XSetWMProtocols(GlobalApp->getXDisplay(), iWindow, &iwmDeleteMessage, 1);
    
    // Window Events we take.
    
    XSelectInput(GlobalApp->getXDisplay(), iWindow,
                 VisibilityChangeMask |
                 FocusChangeMask |
                 StructureNotifyMask );
    
    // Set base flags .
    
    iSurface.left = x0 ;
    iSurface.top = y0 ;
    iSurface.width = wid ;
    iSurface.height = height ;
    iClosed = false ;
    iExposed = false ;
    iFocused = false ;
    
    // Title and map the Window .
    
    XStoreName(GlobalApp->getXDisplay(), iWindow, "GlXWindow Title") ;
    XMapWindow(GlobalApp->getXDisplay(), iWindow) ;
}

GlXWindow::~GlXWindow() noexcept ( false )
{
    GreResourceAutolock ;
    
    GlXApplication * GlobalApp = GlXApplication::GetApplication() ;
    
    if ( !GlobalApp )
    {
        GreDebugPretty() << "'GlobalApp' isn't initialized." << Gre::gendl ;
        throw Gre::GreExceptionWithText ( "'GlobalApp' isn't initialized." ) ;
    }
    
    if ( iWindow )
    {
        XDestroyWindow(GlobalApp->getXDisplay(), iWindow);
        XFreeColormap(GlobalApp->getXDisplay(), iColorMap);
    }
}

void GlXWindow::bind()
{
    
}

void GlXWindow::unbind()
{
    
}

void GlXWindow::bindFramebuffer()
{
    
}

void GlXWindow::unbindFramebuffer()
{
    
}

bool GlXWindow::isAvailableForDrawing() const
{
    GreResourceAutolock ;
    return iExposed ;
}

const void* GlXWindow::getProperty(const std::string &name) const
{
    GreResourceAutolock ;
    
    if ( name == "GLXWindow" )
        return & iWindow ;
    return Gre::WindowPrivate::getProperty(name);
}

void GlXWindow::iSetTitle(const std::string &title) const
{
    GreResourceAutolock ;
    
    GlXApplication * GlobalApp = GlXApplication::GetApplication() ;
    
    if ( !GlobalApp )
    {
        GreDebugPretty() << "'GlobalApp' isn't initialized." << Gre::gendl ;
        throw Gre::GreExceptionWithText ( "'GlobalApp' isn't initialized." ) ;
    }
    
    XStoreName(GlobalApp->getXDisplay(), iWindow, title.c_str());
}

void GlXWindow::iSetRenderContext(const Gre::RenderContextHolder &ctxt) const
{
    
}

void GlXWindow::onUpdateEvent(const Gre::UpdateEvent &e)
{
    GreResourceAutolock ;
    
    GlXApplication * GlobalApp = GlXApplication::GetApplication() ;
    
    if ( !GlobalApp )
    {
        GreDebugPretty() << "'GlobalApp' isn't initialized." << Gre::gendl ;
        throw Gre::GreExceptionWithText ( "'GlobalApp' isn't initialized." ) ;
    }
    
    // Checks for events for this Window.
    
    XEvent nextevent ;
    
    if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, MapNotify, &nextevent) )
    {
        iExposed = true ;
        
        Gre::WindowExposedEvent * event = new Gre::WindowExposedEvent ( this , iSurface ) ;
        sendEvent(event);
    }
    
    else if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, UnmapNotify, &nextevent) )
    {
        iExposed = false ;
        
        Gre::WindowHiddenEvent * event = new Gre::WindowHiddenEvent ( this ) ;
        sendEvent(event);
    }
    
    else if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, ConfigureNotify, &nextevent) )
    {
        XConfigureEvent xce = nextevent.xconfigure;
        
        if ( xce.width != iSurface.width || xce.height != iSurface.height )
        {
            iSurface.width = xce.width ;
            iSurface.height = xce.height ;
            
            Gre::WindowSizedEvent * event = new Gre::WindowSizedEvent ( this , xce.width , xce.height ) ;
            sendEvent(event);
        }
        
        if ( xce.x != iSurface.left || xce.y != iSurface.top )
        {
            iSurface.left = xce.x ;
            iSurface.top = xce.y ;
            
            Gre::WindowMovedEvent * event = new Gre::WindowMovedEvent ( this , xce.x , xce.y ) ;
            sendEvent(event);
        }
    }
    
    else if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, ClientMessage, &nextevent) )
    {
        if ( nextevent.xclient.data.l[0] == iwmDeleteMessage && !iClosed )
        {
            // Window will close. When sending 'WindowWillCloseEvent' , the RenderContext should detach
            // from this Window and destroy itself . This is the default behaviour but that is not
            // garanteed.
            
            Gre::WindowWillCloseEvent * event = new Gre::WindowWillCloseEvent ( this ) ;
            sendEvent(event);
            
            Gre::RenderTargetWillCloseEvent * event2 = new Gre::RenderTargetWillCloseEvent ( this ) ;
            sendEvent(event2);
            
            // Set the 'iClosed' flag to true. The Window Manager should check this flag
            // at each frame.
            
            iClosed = true ;
            
            // Destroy the Window .
            
            XDestroyWindow(GlobalApp->getXDisplay(), iWindow);
            XFreeColormap(GlobalApp->getXDisplay(), iColorMap);
            
            iWindow = 0 ;
            iColorMap = 0 ;
            
            // Send an Event for after closing.
            
            Gre::WindowClosedEvent * event3 = new Gre::WindowClosedEvent ( this ) ;
            sendEvent(event3);
            
            Gre::RenderTargetClosedEvent * event4 = new Gre::RenderTargetClosedEvent ( this ) ;
            sendEvent(event4);
        }
    }
    
    else if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, FocusIn, &nextevent) )
    {
        iFocused = true ;
        
        Gre::WindowFocusedEvent * event = new Gre::WindowFocusedEvent ( this ) ;
        sendEvent(event);
    }
    
    else if ( XCheckTypedWindowEvent(GlobalApp->getXDisplay(), iWindow, FocusOut, &nextevent) )
    {
        iFocused = false ;
        
        Gre::WindowUnfocusedEvent * event = new Gre::WindowUnfocusedEvent ( this ) ;
        sendEvent(event);
    }
}

// ---------------------------------------------------------------------------------------

GlXWindowLoader::GlXWindowLoader()
{
    
}

GlXWindowLoader::~GlXWindowLoader() noexcept ( false )
{
    
}

Gre::WindowHolder GlXWindowLoader::load(const std::string &name, int x0, int y0, int wid, int height) const
{
    return Gre::WindowHolder ( new GlXWindow(name, x0, y0, wid, height) ) ;
}

Gre::ResourceLoader * GlXWindowLoader::clone() const
{
    return new GlXWindowLoader () ;
}

bool GlXWindowLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}
