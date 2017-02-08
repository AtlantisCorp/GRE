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

GreBeginNamespace

Window::Window(const std::string& name)
: RenderTarget(name) , iRenderContext(nullptr)
{
    iTitle = name;
    iSurface.top = 0;
    iSurface.left = 0;
    iSurface.width = 0;
    iSurface.height = 0;
    iExposed = false;
    iClosed = false;
    iFocused = false;
}

Window::~Window() noexcept(false)
{
    // Note : A subclass should always check that the Window has correctly been
    // closed before destruction.
}

bool Window::isClosed() const
{
    GreAutolock ; return iClosed;
}

void Window::setTitle(const std::string &title)
{
    GreAutolock ; iTitle = title;
    
    // Call internal function .
    iSetTitle(title);
}

Surface Window::getSurface() const
{
    GreAutolock ;
    return iSurface;
}

bool Window::isExposed() const
{
    GreAutolock ;
    return iExposed;
}

bool Window::holdsRenderContext() const
{
    GreAutolock ;
    return !iRenderContext.isInvalid();
}

void Window::setRenderContext(const RenderContextUser &renderCtxt)
{
    GreAutolock ;
    
    if ( holdsRenderContext() )
    {
        iRenderContext->unbind();
        
        // Here , we send an event to inform the RenderContext that this Window is detaching it.
        EventHolder event ( new WindowDetachContextEvent ( this ) ) ;
        iRenderContext->onEvent(event) ;
        
        removeListener ( RenderContextUser ( iRenderContext ) ) ;
    }
    
    iRenderContext = renderCtxt.lock();
    addListener( renderCtxt );
    
    // Call the internal function .
    iSetRenderContext(iRenderContext);
    
    // Notifiate the new RenderContext from attaching this Window .
    EventHolder event ( new WindowAttachContextEvent ( this ) ) ;
    iRenderContext->onEvent(event) ;
}

RenderContextHolder Window::getRenderContext()
{
    GreAutolock ;
    return iRenderContext;
}

const RenderContextHolder Window::getRenderContext() const
{
    GreAutolock ;
    return iRenderContext;
}

RenderFramebufferHolder Window::getFramebuffer()
{
    return RenderFramebufferHolder ( nullptr ) ;
}

const RenderFramebufferHolder Window::getFramebuffer() const
{
    return RenderFramebufferHolder ( nullptr ) ;
}

bool Window::holdsFramebuffer() const
{
    return false ;
}

void Window::onUpdateEvent(const Gre::UpdateEvent &e)
{
    // We just call the parent's onUpdateEvent function. A subclass should treat Window's events
    // specifically in this function.
    RenderTarget::onUpdateEvent(e);
}

// ---------------------------------------------------------------------------------------------------

WindowLoader::WindowLoader()
{
    
}

WindowLoader::~WindowLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowManager::WindowManager( const std::string& name )
: Gre::SpecializedResourceManager<Window, WindowLoader> ( name )
, iGlobalKeylistener()
{
    
}

WindowManager::~WindowManager() noexcept ( false ) 
{
    
}

WindowUser WindowManager::load ( const std::string & name , const WindowInfo & info )
{
    GreAutolock ;
    
    WindowLoader * bestloader = iFindBestLoader ( std::string() ) ;
    
    if ( !bestloader )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "No loader found." << Gre::gendl ;
#endif
        return WindowUser ( nullptr ) ;
    }
    
    WindowHolder holder = bestloader->load ( name , info ) ;
    
    if ( holder.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "Window '" << name << "' could not be loaded." << Gre::gendl ;
#endif
        return WindowUser ( nullptr ) ;
    }
    
    iHolders.add ( holder ) ;
    addListener ( WindowUser(holder) ) ;
    iGlobalKeylistener.addListener( EventProceederUser(holder) ) ;
    
    return holder ;
}

void WindowManager::addGlobalKeyListener ( const EventProceederUser & proceeder )
{
    GreAutolock ;
    assert ( !proceeder.isInvalid() && "'proceeder' is invalid." ) ;
    iGlobalKeylistener.addListener(proceeder);
}

void WindowManager::pollEvents ( const Duration& elapsed ) const
{
    _pollEvents () ;
}

void WindowManager::onUpdateEvent(const Gre::UpdateEvent &e)
{
    GreAutolock ;
    
    // Check if a Window needs to be unregistered.
    for ( auto it = iHolders.begin(); it != iHolders.end(); it++ )
    {
        WindowHolder win = (*it) ;
        
        if ( !win.isInvalid() )
        {
            if ( win->isClosed() )
            {
                // We must unregister the Window from here. This will invalidate the iterator , so we can relaunch
                // the function .
                remove ( win ) ;
                onUpdateEvent ( e ) ;
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------

WindowManager::KeyListener::KeyListener ()
{
    
}

WindowManager::KeyListener::~KeyListener()
{
    
}

void WindowManager::KeyListener::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    EventHolder holder ( &e ) ;
    sendEvent ( holder ) ;
}

void WindowManager::KeyListener::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    EventHolder holder ( &e ) ;
    sendEvent( holder ) ;
}

GreEndNamespace

