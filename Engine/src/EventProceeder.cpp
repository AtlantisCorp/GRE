//////////////////////////////////////////////////////////////////////
//
//  EventProceeder.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 01/11/2016.
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

#include "EventProceeder.h"

GreBeginNamespace

EventProceeder::EventProceeder ()
: Gre::ReferenceCountedObject()
, iTransmitBehaviour( EventProceederTransmitBehaviour::SendsAfter )
{
    
}

EventProceeder::~EventProceeder() noexcept ( false )
{
    
}

void EventProceeder::sendEvent(EventHolder &holder)
{
    GreAutolock ;
    
    if ( holder.isInvalid() )
        return ;
    
    // Processes non-filtered listeners. Those listeners will receive every events, as no filters
    // applies.
    
    for ( EventProceederUser listener : iListeners)
    {
        if ( !listener.isInvalid() ) {
            listener.lock() -> onEvent(holder) ;
        }
        
        if ( holder->shouldStopPropagating() ) {
            return ;
        }
    }
    
    // Processes filtered listeners. Those listeners will receive only events that matches with
    // the given filter. Notes, we must make a EventProceederUser conversion as the key iterator
    // is always constant. The 'onEvent()' function should not destroy the object, or moves it in
    // another memory location.
    
    for ( auto it = iFilteredListeners.begin() ; it != iFilteredListeners.end() ; it++ )
    {
        if ( !it->first.isInvalid() )
        {
            if ( std::find(it->second.begin(), it->second.end(), holder->getType()) != it->second.end() )
            {
                EventProceederUser(it->first).lock()->onEvent(holder) ;
            }
            
            if ( holder->shouldStopPropagating() ) {
                return ;
            }
        }
    }
}

void EventProceeder::sendEvent(EventHolder &holder) const
{
    const_cast<EventProceeder*>(this)->sendEvent(holder) ;
}

void EventProceeder::onEvent(EventHolder &holder)
{
    GreAutolock ;
    
    if ( !holder.isInvalid() )
    {
        if ( iTransmitBehaviour == EventProceederTransmitBehaviour::SendsBefore &&
             !holder->noSublisteners() )
        {
            sendEvent(holder);
        }
        
        auto event = holder.getObject() ;
        
        // Treat the Event here. Try to find the correct function.
        
        switch ( event->getType() )
        {
            case EventType::KeyDown:
                onKeyDownEvent(event->to<KeyDownEvent>());
                break;
                
            case EventType::KeyUp:
                onKeyUpEvent(event->to<KeyUpEvent>());
                break;
                
            case EventType::LeftMousePress:
                onLeftMousePressEvent(event->to<LeftMousePressEvent>());
                break;
                
            case EventType::LeftMouseRelease:
                onLeftMouseReleaseEvent(event->to<LeftMouseReleaseEvent>());
                break;
                
            case EventType::RightMousePress:
                onRightMousePressEvent(event->to<RightMousePressEvent>());
                break;
                
            case EventType::RightMouseRelease:
                onRightMouseReleaseEvent(event->to<RightMouseReleaseEvent>());
                break;
                
            case EventType::MouseExitedWindow:
                onMouseExitedWindowEvent(event->to<MouseExitedWindowEvent>());
                break;
                
            case EventType::MouseEnteredWindow:
                onMouseEnteredWindowEvent(event->to<MouseEnteredWindowEvent>());
                break;
                
            case EventType::CursorMoved:
                onCursorMovedEvent(event->to<CursorMovedEvent>());
                break;
                
            case EventType::Update:
                onUpdateEvent(event->to<UpdateEvent>());
                break;
                
            case EventType::WindowSized:
                onWindowSizedEvent(event->to<WindowSizedEvent>());
                break;
                
            case EventType::WindowMoved:
                onWindowMovedEvent(event->to<WindowMovedEvent>());
                break;
                
            case EventType::WindowExposed:
                onWindowExposedEvent(event->to<WindowExposedEvent>());
                break;
                
            case EventType::WindowWillClose:
                onWindowWillCloseEvent(event->to<WindowWillCloseEvent>());
                break;
                
            case EventType::WindowTitleChanged:
                onWindowTitleChangedEvent(event->to<WindowTitleChangedEvent>());
                break;
                
            case EventType::WindowAttachContext:
                onWindowAttachContextEvent(event->to<WindowAttachContextEvent>());
                break;
                
            case EventType::WindowDetachContext:
                onWindowDetachContextEvent(event->to<WindowDetachContextEvent>());
                break;
                
            case EventType::WindowFocused:
                onWindowFocusedEvent(event->to<WindowFocusedEvent>());
                break;
                
            case EventType::WindowUnfocused:
                onWindowUnfocusedEvent(event->to<WindowUnfocusedEvent>());
                break;
                
            case EventType::WindowClosed:
                onWindowClosedEvent(event->to<WindowClosedEvent>());
                break;
                
            case EventType::LastWindowClosed:
                onLastWindowClosed(event->to<LastWindowClosedEvent>());
                break;
                
            case EventType::RenderTargetWillClose:
                onRenderTargetWillCloseEvent(event->to<RenderTargetWillCloseEvent>());
                break;
                
            case EventType::RenderTargetClosed:
                onRenderTargetClosedEvent(event->to<RenderTargetClosedEvent>());
                break;
                
            case EventType::RenderTargetChangedRenderContext:
                onRenderTargetChangedRenderContextEvent(event->to<RenderTargetChangedRenderContextEvent>());
                break;
                
            case EventType::RenderTargetChangedFramebuffer:
                onRenderTargetChangedFramebufferEvent(event->to<RenderTargetChangedFramebufferEvent>());
                break;
                
            case EventType::RendererRegisteredTarget:
                onRendererRegisteredTargetEvent(event->to<RendererRegisteredTargetEvent>());
                break;
                
            case EventType::RendererUnregisteredTarget:
                onRendererUnregisteredTargetEvent(event->to<RendererUnregisteredTargetEvent>());
                break;
                
            case EventType::RenderScenePreRender:
                onRenderScenePreRenderEvent(event->to<RenderScenePreRenderEvent>());
                break;
                
            case EventType::RenderScenePostRender:
                onRenderScenePostRenderEvent(event->to<RenderScenePostRenderEvent>());
                break;
                
            case EventType::ResourceUnloaded:
                onResourceUnloadedEvent(event->to<ResourceUnloadedEvent>());
                break;
                
            case EventType::PositionChanged:
                onPositionChangedEvent(event->to<PositionChangedEvent>());
                break;
                
            case EventType::DirectionChanged:
                onDirectionChangedEvent(event->to<DirectionChangedEvent>());
                break;
                
            case EventType::Custom:
                onCustomEvent(event->to<CustomEvent>());
                break;
                
            default:
#ifdef GreIsDebugMode
                GreDebugPretty() << "Unknown EventType '" << (uint32_t) event->getType() << "'." << std::endl;
#endif
                break;
        }
        
        // Launches the EventHolderCallback.
        
        std::vector< EventHolderCallback > & callbacks = iNextCallbacks[event->getType()] ;
        
        for ( EventHolderCallback & callback : callbacks )
        {
            callback ( holder ) ;
        }
        
        iNextCallbacks[event->getType()].clear();
        
        // See if we have to send the Event to listeners.
        
        if ( iTransmitBehaviour == EventProceederTransmitBehaviour::SendsAfter &&
             !holder->noSublisteners() )
        {
            sendEvent(holder);
        }
    }
}

void EventProceeder::listen(SpecializedCountedObjectUser<Gre::EventProceeder> &proceeder) const
{
    EventProceederHolder p = proceeder.lock() ;
    
    if ( !p.isInvalid() )
    {
        p->addListener( EventProceederUser(this) ) ;
    }
}

void EventProceeder::addListener( const SpecializedCountedObjectUser<Gre::EventProceeder> & proceeder )
{
    GreAutolock ;
    iListeners.push_back(proceeder);
}

const std::vector< SpecializedCountedObjectUser<EventProceeder> > & EventProceeder::getListeners() const
{
    GreAutolock ;
    return iListeners;
}

void EventProceeder::removeListener(const SpecializedCountedObjectUser<Gre::EventProceeder> &proceeder)
{
    GreAutolock ;
    
    for ( auto it = iListeners.begin() ; it != iListeners.end() ; it++ )
    {
        if ( (*it) == proceeder )
        {
            iListeners.erase(it);
            break ;
        }
    }
}

void EventProceeder::clearListeners()
{
    GreAutolock ;
    iListeners.clear();
}

void EventProceeder::setTransmitBehaviour(const Gre::EventProceederTransmitBehaviour &behaviour)
{
    GreAutolock ;
    iTransmitBehaviour = behaviour ;
}

const EventProceederTransmitBehaviour & EventProceeder::getTransmitBehaviour() const
{
    GreAutolock ;
    return iTransmitBehaviour ;
}

void EventProceeder::addNextEventCallback(const Gre::EventType &type, EventHolderCallback callback)
{
    GreAutolock ;
    iNextCallbacks[type].push_back(callback);
}

void EventProceeder::clearNextEventCallback()
{
    GreAutolock ;
    iNextCallbacks.clear();
}

void EventProceeder::clear()
{
    GreAutolock ;
    
    clearListeners();
    clearNextEventCallback();
    iTransmitBehaviour = EventProceederTransmitBehaviour::SendsAfter;
    iFilteredListeners.clear() ;
}

void EventProceeder::addFilteredListener(const EventProceederUser &listener, const std::vector<EventType> &filters)
{
    GreAutolock ; iFilteredListeners [listener] = filters ;
}

void EventProceeder::onUpdateEvent(const Gre::UpdateEvent &e)
{
    
}

void EventProceeder::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    
}

void EventProceeder::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    
}

void EventProceeder::onLeftMousePressEvent(const Gre::LeftMousePressEvent &e)
{
    
}

void EventProceeder::onLeftMouseReleaseEvent(const Gre::LeftMouseReleaseEvent &e)
{
    
}

void EventProceeder::onRightMousePressEvent(const Gre::RightMousePressEvent &e)
{
    
}

void EventProceeder::onRightMouseReleaseEvent(const Gre::RightMouseReleaseEvent &e)
{
    
}

void EventProceeder::onMouseExitedWindowEvent(const Gre::MouseExitedWindowEvent &e)
{
    
}

void EventProceeder::onMouseEnteredWindowEvent(const Gre::MouseEnteredWindowEvent &e)
{
    
}

void EventProceeder::onCursorMovedEvent(const Gre::CursorMovedEvent &e)
{
    
}

void EventProceeder::onWindowMovedEvent(const Gre::WindowMovedEvent &e)
{
    
}

void EventProceeder::onWindowExposedEvent(const Gre::WindowExposedEvent &e)
{
    
}

void EventProceeder::onWindowAttachContextEvent(const Gre::WindowAttachContextEvent &e)
{
    
}

void EventProceeder::onWindowDetachContextEvent(const Gre::WindowDetachContextEvent &e)
{
    
}

void EventProceeder::onWindowSizedEvent(const Gre::WindowSizedEvent &e)
{
    
}

void EventProceeder::onWindowWillCloseEvent(const Gre::WindowWillCloseEvent &e)
{
    
}

void EventProceeder::onWindowTitleChangedEvent(const Gre::WindowTitleChangedEvent &e)
{
    
}

void EventProceeder::onWindowFocusedEvent(const Gre::WindowFocusedEvent &e)
{
    
}

void EventProceeder::onWindowUnfocusedEvent(const Gre::WindowUnfocusedEvent &e)
{
    
}

void EventProceeder::onWindowClosedEvent(const Gre::WindowClosedEvent &e)
{
    
}

void EventProceeder::onLastWindowClosed(const Gre::LastWindowClosedEvent &e)
{
    
}

void EventProceeder::onRenderTargetWillCloseEvent(const Gre::RenderTargetWillCloseEvent &e)
{
    
}

void EventProceeder::onRenderTargetClosedEvent(const Gre::RenderTargetClosedEvent &e)
{
    
}

void EventProceeder::onRenderTargetChangedRenderContextEvent(const Gre::RenderTargetChangedRenderContextEvent &e)
{
    
}

void EventProceeder::onRenderTargetChangedFramebufferEvent(const Gre::RenderTargetChangedFramebufferEvent &e)
{
    
}

void EventProceeder::onRendererRegisteredTargetEvent(const Gre::RendererRegisteredTargetEvent &e)
{
    
}

void EventProceeder::onRendererUnregisteredTargetEvent(const Gre::RendererUnregisteredTargetEvent &e)
{
    
}

void EventProceeder::onRenderScenePreRenderEvent(const Gre::RenderScenePreRenderEvent &e)
{
    
}

void EventProceeder::onRenderScenePostRenderEvent(const Gre::RenderScenePostRenderEvent &e)
{
    
}

void EventProceeder::onResourceUnloadedEvent(const Gre::ResourceUnloadedEvent &e)
{
    
}

void EventProceeder::onPositionChangedEvent(const Gre::PositionChangedEvent &e)
{
    
}

void EventProceeder::onDirectionChangedEvent(const Gre::DirectionChangedEvent &e)
{
    
}

void EventProceeder::onCustomEvent ( const CustomEvent & e )
{
    
}

GreEndNamespace
