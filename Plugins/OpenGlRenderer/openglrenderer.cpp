//
//  openglrenderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"
#include <OpenGL/gl.h>

class OpenGlRenderer : public RendererResource
{
public:
    
    OpenGlRenderer (const std::string& name)
    : RendererResource(name)
    {
        
    }
    
    ~OpenGlRenderer ()
    {
        
    }
    
    void render ()
    {
        glClearColor(1.0F,1.0F,1.0F,0.0F);
        glClearDepth(1.0F);
        glDisable(GL_DEPTH_TEST);
        
        glViewport(0,0,1024,768);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,(float)1024-1,(float)768-1,0,-1,1);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glShadeModel(GL_FLAT);
        glPointSize(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glColor3ub(0,0,0);
    }
};

class OpenGlRendererLoader : public RendererLoader
{
public:
    
    OpenGlRendererLoader () { }
    
    ResourceLoader* clone() const
    { return (ResourceLoader*) new OpenGlRendererLoader; }
    
    Resource* load (Resource::Type type, const std::string& name) const
    {
        return dynamic_cast<Resource*>(new OpenGlRenderer(name));
    }
};

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "OpenGl Renderer";
}

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    ResourceManager::Get().getRendererLoaderFactory().registers("OpenGlLoader", new OpenGlRendererLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}
