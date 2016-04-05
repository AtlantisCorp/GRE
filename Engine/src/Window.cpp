//
//  Window.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Window.h"
#include "WindowResource.h"

GreBeginNamespace

Window::Window()
: ResourceUser(), RenderTarget(), _mWindow()
{
    
}

Window::Window (Window&& rmove)
: ResourceUser(rmove), RenderTarget(rmove), _mWindow(std::move(rmove._mWindow))
{
    
}

Window::Window (const Window& window)
: ResourceUser(window.lock()), RenderTarget(window), _mWindow(window._mWindow)
{
    
}

Window::Window(const ResourceUser& ruser)
: ResourceUser(ruser.lock()), RenderTarget(ruser), _mWindow(std::dynamic_pointer_cast<WindowResource>(ruser.lock()))
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

void Window::setTitle(const std::string &title)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->setTitle(title);
}

Surface Window::getSurface() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->getSurface();
    
    return { 0, 0, 0, 0 };
}

void Window::update()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->update();
}

bool Window::isExposed() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->isExposed();
    return false;
}

bool Window::hasRenderContext() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->hasRenderContext();
    return false;
}

void Window::setRenderContext(const Gre::RenderContext &renderCtxt)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->setRenderContext(renderCtxt);
}

RenderContext& Window::getRenderContext()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->getRenderContext();
    return RenderContext::Null;
}



void Window::addLoopBehaviour(LoopBehaviour behaviour)
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->addLoopBehaviour(behaviour);
}

void Window::clearLoopBehaviour()
{
    auto ptr = _mWindow.lock();
    if(ptr)
        ptr->clearLoopBehaviour();
}

bool Window::hasBeenClosed() const
{
    auto ptr = _mWindow.lock();
    if(ptr)
        return ptr->hasBeenClosed();
    return true;
}

Window Window::Null = Window();

// ---------------------------------------------------------------------------------------------------

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

GreEndNamespace

