//////////////////////////////////////////////////////////////////////
//
//  Window.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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

Window::Window(const std::string& name, const ResourceLoaderOptions& options)
: RenderTarget(name)
{
    iTitle = name;
    iSurface.top = 0;
    iSurface.left = 0;
    iSurface.width = 0;
    iSurface.height = 0;
    iExposed = false;
    iClosed = false;
    iFocused = false;
    iCenterCursor = false ;
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
    _setTitle(title);
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

bool Window::isFocused () const
{
    GreAutolock ; return iFocused ;
}

void Window::setCursorCentered(bool value)
{
    GreAutolock ; iCenterCursor = value ;
}

RenderFramebufferUser Window::getFramebuffer()
{
    return RenderFramebufferUser ( nullptr ) ;
}

const RenderFramebufferUser Window::getFramebuffer() const
{
    return RenderFramebufferUser ( nullptr ) ;
}

void Window::bindFramebuffer() const
{

}

void Window::unbindFramebuffer() const
{

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

    GreAutolock ;

    if ( iCenterCursor ) {
        centerCursor() ;
    }
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
, iGlobalListener ( new EventProceeder )
{
    // We must use this behaviour in order to let the Windows clean themself when sending
    // WindowWillCloseEvent. This behaviour does not change many things but the user may consider
    // this when designing its application.

    EventProceeder::setTransmitBehaviour(EventProceederTransmitBehaviour::SendsBefore);

    if ( !iGlobalListener.isInvalid() )
    iGlobalListener -> addListener(EventProceederUser(this));

    else
    {
        GreDebug ( "[WARN] Can't create Global Listener for Manager '" ) << name << "'." << gendl ;
        GreDebugThrow ( AllocationBadMemory( "EventProceeder" , sizeof(EventProceeder) ) ) ;
    }

    iEventLaunched = false ;
}

WindowManager::~WindowManager() noexcept ( false )
{

}

WindowUser WindowManager::load ( const std::string & name , const ResourceLoaderOptions & info )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
	// Loader specified to load a new Window is either one in the options under name 'Loader.Name', or the
	// first encountered loader in the registered ones (i.e. default one).

    WindowLoader * bestloader = findLoaderFromOptions ( info ) ;

    if ( !bestloader )
    {
        GreDebug ( "[INFO] No 'Loader.Name' found in options. Loading default loader." ) << gendl ;
        bestloader = findLoader () ;
    }

    if ( !bestloader )
    {
        GreDebug ( "[WARN] No loader found to load Window '" ) << name << "'." << gendl ;
        return WindowUser ( nullptr ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Tries to load the window.

    WindowHolder holder = bestloader->load ( name , info ) ;

    if ( holder.isInvalid() )
    {
        GreDebug ( "[WARN] Window '" ) << name << "' could not be loaded." << gendl ;
        return WindowUser ( nullptr ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Registers the Window.

    iHolders.add ( holder ) ;
    addListener ( WindowUser(holder) ) ;
    holder->addListener(EventProceederUser(iGlobalListener));

    GreDebug ( "[INFO] Loaded Window '" ) << name << "'." << gendl ;

    return holder ;
}

void WindowManager::addGlobalKeyListener ( const EventProceederUser & proceeder )
{
    GreAutolock ;
    assert ( !proceeder.isInvalid() && "'proceeder' is invalid." ) ;
    iGlobalListener->addListener(proceeder);
}

void WindowManager::pollEvents ( const Duration& elapsed ) const
{
    _pollEvents () ;
}

void WindowManager::onUpdateEvent(const Gre::UpdateEvent &e)
{
    // [02.19.2017] NOTES : No need to check for closed Window. In fact,
    // when a Window is about to close (WindowWillCloseEvent), it sends
    // an event to the global window listener, wich will also send an event
    // to this manager. This manager will redirect the event to the window
    // to notifiate it will close, and then will destroy it. This lets the
    // window release everything it needs before being released by the manager.

    /*
    // Check if a Window needs to be unregistered.
    for ( auto it = iHolders.begin(); it != iHolders.end(); it++ )
    {
        WindowHolder win = (*it) ;

        if ( !win.isInvalid() )
        {
            if ( win->isClosed() )
            {
                // Unregister the window from Listener's list and destroy it.
                removeListener(EventProceederUser(win));
                remove ( win ) ;

                onUpdateEvent ( e ) ;
            }
        }
    }
     */
}

void WindowManager::closeWindows ()
{
    GreAutolock ;

    clearListeners();
    iHolders.clear();
}

void WindowManager::onEvent(EventHolder &holder)
{
    if ( holder.isInvalid() )
        return ;

    GreAutolock ;

    if ( iEventLaunched )
        return ;

    iEventLaunched = true ;

    // If the holder is the global listener, we do not want to process ANY events, except the
    // WindowWillClose event which we listen in order to do proper cleaning. Being the global listener
    // means the emitter is one of the registered windows.

    if ( _findWindow ( holder->getEmitter().lock().getObject() ) )
    {
        if ( holder->getType() == EventType::WindowWillClose )
        {
            holder -> setNoSublisteners(true) ;
        }

        else
        {
            iEventLaunched = false ;
            return ;
        }
    }

    // If the holder is not the global listener, we can think this event is not related to any of the
    // children's windows. So we can treat normally this event.

    EventProceeder::onEvent(holder) ;
    iEventLaunched = false ;
}

bool WindowManager::_findWindow(const Gre::EventProceeder *window) const
{
    for ( const WindowHolder& holder : iHolders )
    if ( (const EventProceeder*) holder.getObject() == window )
    return true ;

    return false ;
}

void WindowManager::onWindowWillCloseEvent(const Gre::WindowWillCloseEvent &e)
{
    GreAutolock ;

    // We have to release the Window from the holder list.
    for ( auto it = iHolders.begin() ; it != iHolders.end() ; it++ )
    {
        WindowHolder & win = (*it) ;

        if ( !e.getEmitter().isInvalid() ) {
            if ( (EventProceeder*) win.getObject() == e.getEmitter().lock().getObject() ) {
                iHolders.erase(it) ;
                break ;
            }
        }
    }

    if ( iHolders.empty() ){
        // If we are empty, this means the last window has been closed. We now
        // can send the LastWindowClosed Event to the application.
        EventHolder lwc = EventHolder ( new LastWindowClosedEvent(this) ) ;
        sendEvent(lwc) ;
    }
}

GreEndNamespace
