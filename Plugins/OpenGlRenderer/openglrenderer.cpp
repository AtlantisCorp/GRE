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
private:
    
    GLfloat _mClearColor [4];
    GLfloat _mClearDepth;
    
public:
    
    OpenGlRenderer (const std::string& name)
    : RendererResource(name)
    {
        _mClearColor[0] = 1.0f;
        _mClearColor[1] = 1.0f;
        _mClearColor[2] = 1.0f;
        _mClearColor[3] = 0.0f;
        _mClearDepth    = 1.0f;
    }
    
    ~OpenGlRenderer ()
    {
        
    }
    
    void setClearColor (const Color& color)
    {
        _mClearColor[0] = color.red;
        _mClearColor[1] = color.green;
        _mClearColor[2] = color.blue;
        _mClearColor[3] = color.alpha;
    }
    
    void setClearDepth (float depth)
    {
        _mClearDepth = depth;
    }
    
    void render ()
    {
        glClearColor(_mClearColor[0], _mClearColor[1], _mClearColor[2], _mClearColor[3]);
        glClearDepth(_mClearDepth);
        
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
    
    void renderExample ()
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDepthFunc(GL_ALWAYS);
        
        glClearColor(_mClearColor[0], _mClearColor[1], _mClearColor[2], _mClearColor[3]);
        glClearDepth(_mClearDepth);
        glClear(GL_COLOR_BUFFER_BIT);
        
        WindowSize sz = _window.getWindowSize();
        
        glViewport(0,0,sz.first,sz.second);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,(float)sz.first-1,(float)sz.second-1,0,-1,1);
        
        int x1,y1,x2,y2;
        int r,g,b;
        
        x1=rand()%sz.first;
        y1=rand()%sz.second;
        x2=rand()%sz.first;
        y2=rand()%sz.second;
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
        
        glColor3ub(r,g,b);
        glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
        glEnd();
        glFlush();
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
