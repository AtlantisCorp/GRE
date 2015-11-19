//
//  Renderer.h
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_Renderer_h
#define GResource_Renderer_h

#include "Resource.h"
#include "Window.h"

class DLL_PUBLIC RendererResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    RendererResource (const std::string& name);
    
    virtual ~RendererResource () { }
    
    virtual void associateWindow (Window& window) { _window = window; }
    
    virtual void render () { }
    
    virtual void renderExample () { }
    
    void setFramerate(float fps) { _wantedFps = fps; }
    
    float getCurrentFramerate() const { return _currentFps; }
    
    virtual void beginRender();
    virtual void endRender();
    
    virtual void setClearColor(const Color& color) { }
    virtual void setClearDepth(float depth) { }
    
protected:
    
    Window _window;
    float  _wantedFps; ///< @brief Hold the desired framerate.
    float  _currentFps; ///< @brief Hold the current fps.
    
private:
    
    double _begin_time;
    double _end_time;
    double _sum_time;
    int     _sum_frames;
    struct timeval _previous;
};

class DLL_PUBLIC Renderer : public ResourceUser
{
public:
    
    Renderer (Renderer&& movref);
    Renderer (const Renderer& renderer);
    explicit Renderer (const ResourceUser& ruser);
    
    virtual ~Renderer();
    
    Renderer& operator = (const ResourceUser& ruser);
    Renderer& operator = (const Renderer& ruser);
    
    void render();
    void renderExample();
    
    void associateWindow (Window& window);
    
    void setFramerate (float fps);
    float getCurrentFramerate () const;
    void beginRender();
    void endRender();
    
    void setClearColor(const Color& color);
    void setClearDepth(float depth);
    
private:
    
    std::weak_ptr<RendererResource> _mRenderer;
};

class DLL_PUBLIC DLL_PUBLIC RendererLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    RendererLoader();
    virtual ~RendererLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

typedef ResourceLoaderFactory<RendererLoader> RendererLoaderFactory;

#endif
