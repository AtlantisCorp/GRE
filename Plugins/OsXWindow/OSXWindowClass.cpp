//
//  OSXWindowClass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#include "OSXWindow.h"

OsXWindow::OsXWindow(const std::string & name, int x0, int y0, int wid, int hei)
: WindowResource(name)
{
    _nsWindow = NULL;
    _mClosed = false;
    _hasBeenClosed = false;
    NsCreateWindow(&_nsWindow, x0, y0, wid, hei);
    
    if(_nsWindow == NULL)
    {
        GreDebugPretty() << "Impossible to create Window (Os X Plugin)." << std::endl;
    }
}

OsXWindow::~OsXWindow ()
{
    NsDestroyWindow(&_nsWindow);
    _nsWindow = NULL;
}

bool OsXWindow::pollEvent()
{
    if(!_mClosed)
    {
        bool ret = NsPollEvent();
        WindowBufEntry* nsWindowEntry = NsGetWindowBufEntry(&_nsWindow);
        
        if(nsWindowEntry)
        {
            while(nsWindowEntry->keybuf_sz)
            {
                if(nsWindowEntry->keybufs[0].pressed > 0) {
                    KeyDownEvent e(Key(nsWindowEntry->keybufs[0].key));
                    sendEvent(e);
                    nsWindowEntry->keybuf_sz--;
                } else {
                    KeyUpEvent e(Key(nsWindowEntry->keybufs[0].key));
                    sendEvent(e);
                    nsWindowEntry->keybuf_sz--;
                }
            }
            
            if(nsWindowEntry->sizeChanged)
            {
                _mSurface.width = nsWindowEntry->newWidth;
                _mSurface.height = nsWindowEntry->newHeight;
                _mSurface.left = nsWindowEntry->newX;
                _mSurface.top = nsWindowEntry->newY;
                nsWindowEntry->sizeChanged = false;
                
                WindowSizedEvent e;
                e.surface = _mSurface;
                onEvent(e);
            }
            
            _hasBeenClosed = _mExposed && nsWindowEntry->closed;
            _mClosed = nsWindowEntry->closed;
            _mExposed = nsWindowEntry->exposed;
            
            // We must double-check visibility because i don't find any notification
            // to indicate Exposure.
            if(!_mExposed)
                _mExposed = NsWindowIsVisible(&_nsWindow);
        }
        
        return ret;
    }
    
    return true;
}

bool OsXWindow::hasBeenClosed() const
{
    return _hasBeenClosed;
}

const std::string OsXWindow::recommendedRenderer() const
{
    // This is not just an information. For now, the RenderContext must
    // have an CGlId field in order for this Window to draw something
    // on screen.
    // In other words, i don't know any other method to draw on screen ^^...
    return "OpenGl";
}

void OsXWindow::setTitle(const std::string& title)
{
    NsSetWindowTitle(&_nsWindow, title.c_str());
}

void OsXWindow::swapBuffers ()
{
    NsWindowSwapBuffers(&_nsWindow);
}

void OsXWindow::setVerticalSync (bool vsync)
{
    NsWindowSetVertSync(&_nsWindow, vsync);
}

bool OsXWindow::hasVerticalSync () const
{
    return NsWindowIsVertSync(&_nsWindow);
}

void OsXWindow::bind()
{
    if(!getRenderContext().expired())
    {
        getRenderContext().bind();
    }
}

void OsXWindow::bindFramebuffer()
{
    
}

void OsXWindow::unbind()
{
    if(!getRenderContext().expired())
    {
        getRenderContext().flush();
        getRenderContext().unbind();
    }
}

void OsXWindow::unbindFramebuffer()
{
    
}

void OsXWindow::onRenderContextChanged()
{
    if(!getRenderContext().expired())
    {
        uintptr_t* _mContext = (uintptr_t*) ((const CGLContextObj*)getRenderContext().getCustomData("CGLContext"));
        // When RenderContext is changed, we must notifiate the CustomWindow
        // for it to change the OpenGlCustomView.
        NsWindowSetRenderContext(&_nsWindow, *((CGLContextObj*)_mContext));
    }
}

// ---------------------------------------------------------------------------------------------------

OsXWindowLoader::OsXWindowLoader ()
{
    
}

OsXWindowLoader::~OsXWindowLoader ()
{
    
}

ResourceLoader* OsXWindowLoader::clone() const
{
    return (ResourceLoader*) new OsXWindowLoader;
}

Resource* OsXWindowLoader::load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const
{
    return (Resource*) new OsXWindow (name, x0, y0, wid, height);
}
