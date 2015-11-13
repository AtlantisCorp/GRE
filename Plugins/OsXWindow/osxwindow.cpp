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

extern "C" DLL_PUBLIC void GCreateWindow (int x0,int y0,int wid,int hei);
extern "C" DLL_PUBLIC bool GPollEvent (void);
extern "C" DLL_PUBLIC bool GIsWindowClosed (void);
extern "C" DLL_PUBLIC void GInitRendererContext (void);
extern "C" DLL_PUBLIC void GWSetTitle (const char*);

class DLL_PUBLIC OsXWindow : public WindowResource
{
public:
    
    OsXWindow (const std::string & name, const WindowPrivate& data)
    : WindowResource(name, data)
    {
        std::cout << "[OsX Window] Created !" << std::endl;
        _isclosed = false;
    }
    
    ~OsXWindow ()
    {
        std::cout << "[OsX Window] Destroyed !" << std::endl;
    }
    
    bool pollEvent()
    {
        _isclosed = GIsWindowClosed();
        return GPollEvent();
    }
    
    bool isClosed() const
    { return _isclosed; }
    
    const std::string recommendedRenderer() const
    {
        return "OpenGl";
    }
    
    void associate (Renderer renderer)
    {
        WindowResource::associate(renderer);
        GInitRendererContext();
    }
    
    void setTitle(const std::string& title)
    {
        GWSetTitle(title.c_str());
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