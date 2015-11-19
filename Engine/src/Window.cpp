//
//  Window.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Window.h"
#include "WindowResource.h"

WindowResource::WindowResource (const std::string& name, const WindowPrivate& winData)
: Resource(name), _associatedRenderer(ResourceUser::Null)
{
    
}

WindowResource::~WindowResource()
{
    
}

Window::Window()
: ResourceUser()
{
    
}

Window::Window (Window&& rmove)
: ResourceUser(rmove)
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

Window& Window::operator=(const Window &wuser)
{
    ResourceUser::_resource = wuser.lock();
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

void Window::associate(Renderer& renderer)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        usable->associate(renderer);
        renderer.associateWindow(*this);
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

void Window::setTitle(const std::string &title)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        usable->setTitle(title);
    }
}

void Window::swapBuffers()
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        usable->swapBuffers();
    }
}

WindowSize Window::getWindowSize() const
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->getWindowSize();
    }
    
    return std::make_pair(0, 0);
}

void Window::setVerticalSync(bool vsync)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        usable->setVerticalSync(vsync);
    }
}

bool Window::hasVerticalSync() const
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->hasVerticalSync();
    }
    
    return false;
}

Listener& Window::addListener(const std::string& name)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->addListener(name);
    }
    
    return Listener::Null;
}

Listener Window::getListener(const std::string& name)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->getListener(name);
    }
    
    return Listener::Null;
}

void Window::removeListener(const std::string& name)
{
    auto ptr = lock();
    if(ptr)
    {
        WindowResource* usable = dynamic_cast<WindowResource*>(ptr.get());
        return usable->removeListener(name);
    }
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



