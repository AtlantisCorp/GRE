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

class DLL_PUBLIC RendererResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    RendererResource (const std::string& name);
    
    virtual ~RendererResource () { }
    
    virtual void render () { }
};

class DLL_PUBLIC Renderer : public ResourceUser
{
public:
    
    Renderer (const Renderer& renderer);
    explicit Renderer (const ResourceUser& ruser);
    
    virtual ~Renderer();
    
    Renderer& operator = (const ResourceUser& ruser);
    
    void render();
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
