//
//  Window.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Window.h"
#include "WindowResource.h"

GRE_BEGIN_NAMESPACE

WindowResource::WindowResource (const std::string& name, const WindowPrivate& winData)
: Resource(name), _associatedRenderer(ResourceUser::Null)
{
    
}

WindowResource::~WindowResource()
{
    
}

void WindowResource::beginUpdate()
{
    pollEvent();
    
    if(_associatedRenderer.expired() == false)
        _associatedRenderer.beginRender();
}

void WindowResource::endUpdate()
{
    if(_associatedRenderer.expired() == false)
        _associatedRenderer.endRender();
    
    swapBuffers();
}

Window::Window()
: ResourceUser(), _mWindow()
{
    
}

Window::Window (Window&& rmove)
: ResourceUser(rmove), _mWindow(std::move(rmove._mWindow))
{
    
}

Window::Window (const Window& window)
: ResourceUser(window.lock()), _mWindow(window._mWindow)
{
    
}

Window::Window(const ResourceUser& ruser)
: ResourceUser(ruser.lock()), _mWindow(std::dynamic_pointer_cast<WindowResource>(ruser.lock()))
{
    
}

Window::~Window()
{
    
}

Window& Window::operator=(const ResourceUser& ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mWindow = std::dynamic_pointer_cast<WindowResource>(ruser.lock());
    return *this;
}

Window& Window::operator=(const Window &wuser)
{
    ResourceUser::_resource = wuser.lock();
    _mWindow = wuser._mWindow;
    return *this;
}

bool Window::pollEvent()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->pollEvent();
    
    return false;
}

bool Window::isClosed() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->isClosed();
    
    return true;
}

const std::string Window::recommendedRenderer() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->recommendedRenderer();
    
    return "";
}

void Window::associate(Renderer& renderer)
{
    auto ptr = _mWindow.lock();
    if(ptr)
    {
        ptr->associate(renderer);
        renderer.associateWindow(*this);
    }
}

Renderer Window::getAssociatedRenderer()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->getAssociatedRenderer();
    
    return Renderer(ResourceUser::Null);
}

void Window::setTitle(const std::string &title)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->setTitle(title);
}

void Window::swapBuffers()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->swapBuffers();
}

WindowSize Window::getWindowSize() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->getWindowSize();
    
    return std::make_pair(0, 0);
}

void Window::setVerticalSync(bool vsync)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->setVerticalSync(vsync);
}

bool Window::hasVerticalSync() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->hasVerticalSync();
    
    return false;
}

Listener& Window::addListener(const std::string& name)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->addListener(name);
    
    return Listener::Null;
}

Listener Window::getListener(const std::string& name)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->getListener(name);
    
    return Listener::Null;
}

void Window::removeListener(const std::string& name)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->removeListener(name);
}

void Window::beginUpdate()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->beginUpdate();
}

void Window::endUpdate()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->endUpdate();
}

bool Window::isExposed() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->isExposed();
    return false;
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

GRE_END_NAMESPACE

