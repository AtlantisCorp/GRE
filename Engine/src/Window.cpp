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

WindowPrivate::WindowPrivate(const std::string& name)
: RenderTargetPrivate(name)
, iRenderContext(nullptr)
{
    iTitle = name;
    iSurface.top = 0;
    iSurface.left = 0;
    iSurface.width = 0;
    iSurface.height = 0;
    iExposed = false;
    iClosed = true;
    iLastUpdate = iLastUpdate.min();
}

WindowPrivate::~WindowPrivate() noexcept(false)
{
    // Note : A subclass should always check that
    // the Window has correctly been closed before
    // destruction.
}

bool WindowPrivate::pollEvent()
{
    return false;
}

bool WindowPrivate::isClosed() const
{
    return iClosed;
}

void WindowPrivate::setTitle(const std::string &title)
{
    iTitle = title;
}

Surface WindowPrivate::getSurface() const
{
    return iSurface;
}

void WindowPrivate::update()
{
    // Here we send an update event to listeners.
    if(iLastUpdate != iLastUpdate.min())
    {
        UpdateEvent ue; UpdateTime now = UpdateChrono::now();
        ue.elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - iLastUpdate);
        
        sendEvent(ue);
        
        iLastUpdate = now;
    }
    else
    {
        iLastUpdate = UpdateChrono::now();
    }
    
    // Then, we must call every LoopBehaviour functions.
    iLoopBehaviours.call();
}

bool WindowPrivate::isExposed() const
{
    return iExposed;
}

bool WindowPrivate::holdsRenderContext() const
{
    return !iRenderContext.isInvalid();
}

void WindowPrivate::setRenderContext(const RenderContext &renderCtxt)
{
    if ( holdsRenderContext() )
    {
        iRenderContext->unbind();
        removeListener(iRenderContext->getName());
    }
    
    iRenderContext = renderCtxt.lock();
    addListener(iRenderContext);
}

RenderContextHolder WindowPrivate::getRenderContext()
{
    return iRenderContext;
}

const RenderContextHolder WindowPrivate::getRenderContext() const
{
    return iRenderContext;
}

void WindowPrivate::addLoopBehaviour(LoopBehaviour behaviour)
{
    iLoopBehaviours.add(behaviour);
}

void WindowPrivate::clearLoopBehaviour()
{
    iLoopBehaviours.clear();
}

bool WindowPrivate::hasBeenClosed() const
{
    return true;
}

// ---------------------------------------------------------------------------------------------------

Window::Window(const WindowPrivate* pointer)
: Gre::ResourceUser(pointer)
, RenderTarget(pointer)
{
    
}

Window::Window (const WindowHolder& holder)
: Gre::ResourceUser(holder)
, RenderTarget(holder.get())
{
    
}

Window::Window(const Window& user)
: Gre::ResourceUser(user)
, RenderTarget(user)
{
    
}

Window::~Window()
{
    
}

WindowHolder Window::lock()
{
    return GreUserLockCast(WindowHolder, WindowPrivate, RenderTarget);
}

const WindowHolder Window::lock() const
{
    return GreUserConstLockCast(WindowHolder, WindowPrivate, RenderTarget);
}

bool Window::pollEvent()
{
    auto ptr = lock();
    if(ptr)
        return ptr->pollEvent();
    throw GreInvalidUserException("Window");
}

bool Window::isClosed() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isClosed();
    throw GreInvalidUserException("Window");
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
    throw GreInvalidUserException("Window");
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
    throw GreInvalidUserException("Window");
}

bool Window::holdsRenderContext() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->holdsRenderContext();
    throw GreInvalidUserException("Window");
}

void Window::setRenderContext(const Gre::RenderContext &renderCtxt)
{
    auto ptr = lock();
    if(ptr)
        ptr->setRenderContext(renderCtxt);
}

RenderContextHolder Window::getRenderContext()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getRenderContext();
    throw GreInvalidUserException("Window");
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
    throw GreInvalidUserException("Window");
}

Window Window::Null = Window(nullptr);

// ---------------------------------------------------------------------------------------------------

WindowLoader::WindowLoader()
{
    
}

WindowLoader::~WindowLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowManager::WindowManager()
{
    
}

WindowManager::~WindowManager()
{
    // We call 'clearWindows' in order to be sure every Windows was launched the
    // event WindowMustClose.
    
    clearWindows();
}

Window WindowManager::load(const std::string &name, int x0, int y0, int wid, int height)
{
    // As every WindowLoader installed should be able to load a Window, we should try every
    // WindowLoader as soon as one created our Window.
    
    if ( !name.empty() )
    {
        Window tmp = get(name);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Window Resource '" << name << "' is already present." << std::endl;
#endif
            return Window::Null;
        }
        
        for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
        {
            auto& loader = it->second;
            
            WindowHolder loaded = loader->load(name, x0, y0, wid, height);
            
            if ( !loaded.isInvalid() )
            {
                iWindows.add(loaded);
                return Window ( loaded );
            }
            
            else
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't load Window Resource '" << name << "' with loader '" << it->first << "'." << std::endl;
#endif
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't found any WindowLoader able to load Window Resource '" << name << "'." << std::endl;
#endif
        
        return Window ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Window ( nullptr );
    }
}

Window WindowManager::load(const WindowHolder &holder)
{
    if ( !holder.isInvalid() )
    {
        Window tmp = get(holder->getName());
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Window Resource '" << holder->getName() << "' is already loaded." << std::endl;
#endif
            return Window ( nullptr );
        }
        
        iWindows.add(holder);
        return Window ( holder );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'holder' is invalid." << std::endl;
#endif
        return Window ( nullptr );
    }
}

Window WindowManager::get(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( WindowHolder& holder : iWindows )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Window ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Window Resource '" << name << "' not found." << std::endl;
#endif
        return Window ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Window ( nullptr );
    }
}

const Window WindowManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( const WindowHolder& holder : iWindows )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Window ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Window Resource '" << name << "' not found." << std::endl;
#endif
        return Window ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Window ( nullptr );
    }
}

Window WindowManager::getFirstActive()
{
    for ( WindowHolder& holder : iWindows )
    {
        if ( !holder.isInvalid() )
        {
            if ( holder->isExposed() )
            {
                return Window ( holder );
            }
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Window Resource actived not found." << std::endl;
#endif
    return Window ( nullptr );
}

GreEndNamespace

