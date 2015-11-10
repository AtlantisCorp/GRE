//
//  Window.h
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef __GResource__Window__
#define __GResource__Window__

#include "Resource.h"
#include "Renderer.h"

struct WindowPrivate
{
    
};

class DLL_PUBLIC WindowResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    WindowResource (const std::string& name, const WindowPrivate& winData);
    
    virtual ~WindowResource();
    
    virtual bool pollEvent() { return false; }
    
    virtual bool isClosed() const { return true; }
    
    virtual const std::string recommendedRenderer() const { return ""; }
    
    virtual void associate(Renderer renderer) { _associatedRenderer = renderer; }
    
    Renderer getAssociatedRenderer() { return _associatedRenderer; }
    
protected:
    
    WindowPrivate _data;
    Renderer      _associatedRenderer;
};

class DLL_PUBLIC Window : public ResourceUser
{
public:
    
    Window (const Window& window);
    explicit Window (const ResourceUser& ruser);
    
    virtual ~Window();
    
    Window& operator = (const ResourceUser& ruser);
    
    bool pollEvent();
    bool isClosed() const;
    const std::string recommendedRenderer() const;
    
    void associate(Renderer renderer);
    
    Renderer getAssociatedRenderer();
};

class DLL_PUBLIC WindowLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    WindowLoader();
    virtual ~WindowLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

typedef ResourceLoaderFactory<WindowLoader> WindowLoaderFactory;

#endif /* defined(__GResource__Window__) */
