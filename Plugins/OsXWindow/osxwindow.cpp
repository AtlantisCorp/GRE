//
//  osxwindow.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define BUILDING_DLL
#include "ResourceManager.h"
#include "WindowResource.h"

extern "C" DLL_PUBLIC void GCreateWindow (int x0,int y0,int wid,int hei);
extern "C" DLL_PUBLIC bool GPollEvent (void);
extern "C" DLL_PUBLIC bool GIsWindowClosed (void);
extern "C" DLL_PUBLIC void GInitRendererContext (void);
extern "C" DLL_PUBLIC void GWSetTitle (const char*);
extern "C" DLL_PUBLIC void GSwapBuffers(void);
extern "C" DLL_PUBLIC void GGetWindowSize (int*, int*);
extern "C" DLL_PUBLIC void GSetVSync (int vsync);
extern "C" DLL_PUBLIC int GHasVSync ();

#define KEYBUF_MAX 256

struct keybuf_t
{
    int key;
    int pressed;
};

keybuf_t keybuf [KEYBUF_MAX];
int      keybuf_sz = 0;

class DLL_PUBLIC OsXWindow : public WindowResource
{
public:
    
    OsXWindow (const std::string & name, const WindowPrivate& data)
    : WindowResource(name, data)
    {
        _isclosed = false;
    }
    
    ~OsXWindow ()
    {
        
    }
    
    bool pollEvent()
    {
        _isclosed = GIsWindowClosed();
        bool ret = GPollEvent();
        
        // Send key events
        while(keybuf_sz)
        {
            KeyDownEvent e(Key(keybuf[0].key));
            sendEvent(e);
            keybuf_sz--;
        }
        
        return ret;
    }
    
    bool isClosed() const
    { return _isclosed; }
    
    const std::string recommendedRenderer() const
    {
        return "OpenGl";
    }
    
    void associate (Renderer& renderer)
    {
        WindowResource::associate(renderer);
        GInitRendererContext();
    }
    
    void setTitle(const std::string& title)
    {
        GWSetTitle(title.c_str());
    }
    
    void swapBuffers ()
    {
        GSwapBuffers();
    }
    
    WindowSize getWindowSize() const
    {
        int wid, height;
        GGetWindowSize(&wid, &height);
        return std::make_pair(wid, height);
    }
    
    void setVerticalSync (bool vsync)
    {
        GSetVSync(vsync?1:0);
    }
    
    bool hasVerticalSync () const
    {
        return (bool) GHasVSync();
    }
    
private:
    
    bool _isclosed;
};

class DLL_PUBLIC OsXWindowLoader : public WindowLoader
{
public:
    
    OsXWindowLoader () { }
    ~OsXWindowLoader () { }
    
    ResourceLoader* clone() const
    { return (ResourceLoader*) new OsXWindowLoader; }
    
    Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const
    {
        WindowPrivate data;
        GCreateWindow(x0, y0, wid, height);
        
        return (Resource*) new OsXWindow (name, data);
    }
};

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "OS X Window System";
}

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    ResourceManager::Get().getWindowLoaderFactory().registers("OsXWindow", new OsXWindowLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}