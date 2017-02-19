//////////////////////////////////////////////////////////////////////
//
//  EventProceeder.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 30/10/2016.
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

#ifndef EventProceeder_h
#define EventProceeder_h

#include "Pools.h"
#include "Event.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @brief Behaviour on transmitting the Event when receiving it.
////////////////////////////////////////////////////////////////////////
enum class EventProceederTransmitBehaviour
{
    /// @brief Sends the received event before treating.
    SendsBefore ,
    
    /// @brief Sends the received event after treating it.
    SendsAfter ,
    
    /// @brief Does not send the event.
    None
};

////////////////////////////////////////////////////////////////////////
/// @brief Defines a base interface for Emitter/Receiver objects.
////////////////////////////////////////////////////////////////////////
class EventProceeder : public ReferenceCountedObject
{
public:
    
    POOLED( Pools::Referenced )
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    EventProceeder () ;
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~EventProceeder () noexcept ( false ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Sends an Event to its Listener.
    ////////////////////////////////////////////////////////////////////////
    virtual void sendEvent ( EventHolder & holder ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Sends an Event to its Listener.
    ////////////////////////////////////////////////////////////////////////
    virtual void sendEvent ( EventHolder & holder ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Determines the behaviour of this object when receiving Events.
    ////////////////////////////////////////////////////////////////////////
    virtual void onEvent ( EventHolder & holder ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Listens to the given EventProceeder.
    /// Calls 'proceeder :: addListener'.
    //////////////////////////////////////////////////////////////////////
    virtual void listen ( SpecializedCountedObjectUser<EventProceeder> & proceeder ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Registers a new EventProceeder.
    ////////////////////////////////////////////////////////////////////////
    virtual void addListener ( const SpecializedCountedObjectUser<EventProceeder> & proceeder ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the list of EventProceeder.
    ////////////////////////////////////////////////////////////////////////
    virtual const std::vector < SpecializedCountedObjectUser < EventProceeder > > & getListeners () const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Removes an EventProceeder.
    ////////////////////////////////////////////////////////////////////////
    virtual void removeListener ( const SpecializedCountedObjectUser<EventProceeder> & proceeder ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Removes every EventProceeder.
    ////////////////////////////////////////////////////////////////////////
    virtual void clearListeners () ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Changes the Transmit Behaviour.
    ////////////////////////////////////////////////////////////////////////
    virtual void setTransmitBehaviour ( const EventProceederTransmitBehaviour& behaviour ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Transmit Behaviour.
    ////////////////////////////////////////////////////////////////////////
    virtual const EventProceederTransmitBehaviour & getTransmitBehaviour () const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Adds a callback for the next event to be proceeded.
    ////////////////////////////////////////////////////////////////////////
    virtual void addNextEventCallback ( const EventType & type , EventHolderCallback callback ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Resets the callbacks.
    ////////////////////////////////////////////////////////////////////////
    virtual void clearNextEventCallback () ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Resets the proceeder.
    ////////////////////////////////////////////////////////////////////////
    virtual void clear () ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// [thread-safe]
    ///
    /// The UpdateEvent can be emitted when Window objects themself, using
    /// ResourceManager::loop() or Window::update().
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is up.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyUpEvent(const KeyUpEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is down.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyDownEvent(const KeyDownEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onLeftMousePressEvent ( const LeftMousePressEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onLeftMouseReleaseEvent ( const LeftMouseReleaseEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRightMousePressEvent ( const RightMousePressEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRightMouseReleaseEvent ( const RightMouseReleaseEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onMouseExitedWindowEvent ( const MouseExitedWindowEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onMouseEnteredWindowEvent ( const MouseEnteredWindowEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowMovedEvent ( const WindowMovedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowExposedEvent ( const WindowExposedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has attached a new RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowAttachContextEvent ( const WindowAttachContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has detached a new RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowDetachContextEvent ( const WindowDetachContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has sized its surface .
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowSizedEvent ( const WindowSizedEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object will be closed.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowWillCloseEvent ( const WindowWillCloseEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowTitleChangedEvent ( const WindowTitleChangedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowFocusedEvent ( const WindowFocusedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowUnfocusedEvent ( const WindowUnfocusedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowClosedEvent ( const WindowClosedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onLastWindowClosed ( const LastWindowClosedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a RenderTarget will be destroyed.
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderTargetWillCloseEvent ( const RenderTargetWillCloseEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a RenderTarget has been destroyed.
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderTargetClosedEvent ( const RenderTargetClosedEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a RenderTarget changed its RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderTargetChangedRenderContextEvent ( const RenderTargetChangedRenderContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a RenderTarget changed its RenderFramebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderTargetChangedFramebufferEvent ( const RenderTargetChangedFramebufferEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRendererRegisteredTargetEvent ( const RendererRegisteredTargetEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRendererUnregisteredTargetEvent ( const RendererUnregisteredTargetEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderScenePreRenderEvent ( const RenderScenePreRenderEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderScenePostRenderEvent ( const RenderScenePostRenderEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onResourceUnloadedEvent ( const ResourceUnloadedEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onCustomEvent ( const CustomEvent & e ) ;
    
protected:
    
    /// @brief Listeners registered in this object.
    std::vector < SpecializedCountedObjectUser<EventProceeder> > iListeners ;
    
    /// @brief Transmit Behaviour. (Default is 'SendsAfter')
    EventProceederTransmitBehaviour iTransmitBehaviour ;
    
    /// @brief List of functions to do on next event call of given type.
    std::map < EventType , std::vector < EventHolderCallback > > iNextCallbacks ;
};

/// @brief SpecializedCountedObjectHolder for EventProceeder.
typedef SpecializedCountedObjectHolder < EventProceeder > EventProceederHolder ;

/// @brief SpecializedCountedObjectUser for EventProceeder.
typedef SpecializedCountedObjectUser < EventProceeder > EventProceederUser ;

GreEndNamespace

#endif /* EventProceeder_h */
