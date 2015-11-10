//
//  Window.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Window.h"

WindowResource::WindowResource (const std::string& name, const WindowPrivate& winData)
: Resource(name), _associatedRenderer(ResourceUser::Null)
{
    
}

WindowResource::~WindowResource()
{
    
}

Window::Window (const Window& window)
: ResourceUser(window.lock())
{
    
}

Window::Window(const ResourceUser& ruser)
: ResourceUser(ruser.lock())
{
    
}

Window::~Window()
{
    
}

Window& Window::operator=(const ResourceUser& ruser)
{
    ResourceUser::_resource = ruser.lock();
    return *this;
}

bool Window::pollEvent()
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->pollEvent();
    }
    
    return false;
}

bool Window::isClosed() const
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->isClosed();
    }
    
    return true;
}

const std::string Window::recommendedRenderer() const
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->recommendedRenderer();
    }
    
    return "";
}

void Window::associate(Renderer renderer)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        usable->associate(renderer);
    }
}

Renderer Window::getAssociatedRenderer()
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->getAssociatedRenderer();
    }
    
    return Renderer(ResourceUser::Null);
}

WindowLoader::WindowLoader()
{
    
}

WindowLoader::~WindowLoader()
{
    
}

bool WindowLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Window;
}

Resource* WindowLoader::load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const
{
    return nullptr;
}

ResourceLoader* WindowLoader::clone() const
{
    return new WindowLoader();
}



