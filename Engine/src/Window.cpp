//////////////////////////////////////////////////////////////////////
//
//  Window.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 07/11/2015.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#include "Window.h"
#include "WindowResource.h"

GreBeginNamespace

Window::Window(const WindowPrivate* pointer)
: RenderTarget(pointer), SpecializedResourceUser<WindowPrivate>(pointer)
{
    
}

Window::Window (const WindowHolder& holder)
: RenderTarget(holder.get()), SpecializedResourceUser<WindowPrivate>(holder)
{
    
}

Window::Window(const Window& user)
: RenderTarget(user), SpecializedResourceUser<WindowPrivate>(user)
{
    
}

Window::~Window()
{
    
}

SpecializedResourceHolder<WindowPrivate> Window::lock()
{
    return SpecializedResourceUser<WindowPrivate>::lock();
}

const SpecializedResourceHolder<WindowPrivate> Window::lock() const
{
    return SpecializedResourceUser<WindowPrivate>::lock();
}

bool Window::pollEvent()
{
    auto ptr = lock();
    if(ptr)
        return ptr->pollEvent();
    
    return false;
}

bool Window::isClosed() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isClosed();
    
    return true;
}

const std::string Window::recommendedRenderer() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->recommendedRenderer();
    
    return "";
}

void Window::setTitle(const std::string &title)
{
    auto ptr = lock();
    if(ptr)
        ptr->setTitle(title);
}

Surface Window::getSurface() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSurface();
    
    return { 0, 0, 0, 0 };
}

void Window::update()
{
    auto ptr = lock();
    if(ptr)
        ptr->update();
}

bool Window::isExposed() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isExposed();
    return false;
}

bool Window::hasRenderContext() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->hasRenderContext();
    return false;
}

void Window::setRenderContext(const Gre::RenderContext &renderCtxt)
{
    auto ptr = lock();
    if(ptr)
        ptr->setRenderContext(renderCtxt);
}

RenderContext& Window::getRenderContext()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getRenderContext();
    return RenderContext::Null;
}



void Window::addLoopBehaviour(LoopBehaviour behaviour)
{
    auto ptr = lock();
    if(ptr)
        ptr->addLoopBehaviour(behaviour);
}

void Window::clearLoopBehaviour()
{
    auto ptr = lock();
    if(ptr)
        ptr->clearLoopBehaviour();
}

bool Window::hasBeenClosed() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->hasBeenClosed();
    return true;
}

Window Window::Null = Window(nullptr);

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

