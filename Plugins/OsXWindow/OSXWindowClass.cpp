//
//  OSXWindowClass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#include "OSXWindow.h"

keybuf_t keybuf [KEYBUF_MAX];
int      keybuf_sz = 0;

OsXWindow::OsXWindow (const std::string & name, const WindowPrivate& data)
: WindowResource(name, data)
{
    _isclosed = false;
}

OsXWindow::~OsXWindow ()
{
    
}

bool OsXWindow::pollEvent()
{
    _isclosed = GIsWindowClosed();
    bool ret = GPollEvent();
    
    // Send key events
    while(keybuf_sz)
    {
        if(keybuf[0].pressed > 0) {
            KeyDownEvent e(Key(keybuf[0].key));
            sendEvent(e);
            keybuf_sz--;
        } else {
            KeyUpEvent e(Key(keybuf[0].key));
            sendEvent(e);
            keybuf_sz--;
        }
    }
    
    return ret;
}

bool OsXWindow::isClosed() const
{
    return _isclosed;
}

const std::string OsXWindow::recommendedRenderer() const
{
    return "OpenGl";
}

void OsXWindow::associate (Renderer& renderer)
{
    WindowResource::associate(renderer);
    GInitRendererContext();
}

void OsXWindow::setTitle(const std::string& title)
{
    GWSetTitle(title.c_str());
}

void OsXWindow::swapBuffers ()
{
    GSwapBuffers();
}

WindowSize OsXWindow::getWindowSize() const
{
    int wid, height;
    GGetWindowSize(&wid, &height);
    return std::make_pair(wid, height);
}

void OsXWindow::setVerticalSync (bool vsync)
{
    GSetVSync(vsync?1:0);
}

bool OsXWindow::hasVerticalSync () const
{
    return GHasVSync()>0?true:false;
}

bool OsXWindow::isExposed () const
{
    return windowExposed;
}

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
    WindowPrivate data;
    GCreateWindow(x0, y0, wid, height);
    
    return (Resource*) new OsXWindow (name, data);
}
