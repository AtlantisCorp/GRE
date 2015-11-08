//
//  Renderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Renderer.h"

RendererResource::RendererResource (const std::string& name)
: Resource(name)
{
    
}


Renderer::Renderer (const Renderer& renderer)
: ResourceUser(renderer.lock())
{
    
}

Renderer::Renderer (const ResourceUser& renderer)
: ResourceUser(renderer.lock())
{
    
}

Renderer::~Renderer()
{
    
}

Renderer& Renderer::operator=(const ResourceUser &ruser)
{
    ResourceUser::_resource = ruser.lock();
    return *this;
}

void Renderer::render()
{
    auto ptr = lock();
    if(ptr)
    {
        RendererResource* usable = dynamic_cast<RendererResource*>(ptr.get());
        usable->render();
    }
}

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

bool RendererLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Renderer;
}

Resource* RendererLoader::load (Resource::Type type, const std::string& name) const
{
    return nullptr;
}

ResourceLoader* RendererLoader::clone() const
{
    return new RendererLoader();
}
