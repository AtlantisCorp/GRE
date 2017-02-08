//////////////////////////////////////////////////////////////////////
//
//  Event.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 23/11/2015.
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

#include "Event.h"
#include "EventProceeder.h"

GreBeginNamespace

Event::Event(const EventProceeder * emitter , const EventType & etype)
: iEmitter ( nullptr ) , iType ( etype ) , iShouldStopPropagating( false )
{
    GreAutolock ;
    iEmitter = new EventProceederUser ( emitter ) ;
    
    if ( !iEmitter )
        throw GreExceptionWithText("'iEmitter' couldn't be allocated.") ;
}

Event::~Event() noexcept(false)
{
    if ( iEmitter )
        delete iEmitter ;
}

const SpecializedCountedObjectUser<EventProceeder> & Event::getEmitter() const
{
    GreAutolock ;
    
    if ( !iEmitter )
    {
        throw GreExceptionWithText ( "Bad 'Event::iEmitter' property." ) ;
    }
    
    return * iEmitter ;
}

const EventType& Event::getType() const
{
    GreAutolock ;
    return iType;
}

bool Event::shouldStopPropagating() const
{
    GreAutolock ;
    return iShouldStopPropagating ;
}

void Event::setShouldStopPropagating(bool value)
{
    GreAutolock ;
    iShouldStopPropagating = value ;
}

// ---------------------------------------------------------------------------------------------------

KeyDownEvent::KeyDownEvent ( const EventProceeder * emitter , Key key )
: Gre::Event( emitter , EventType::KeyDown )
, iKey(key)
{
    
}

Event* KeyDownEvent::clone() const
{
    return new KeyDownEvent ( iEmitter->lock().getObject() , iKey ) ;
}

// ---------------------------------------------------------------------------------------------------

KeyUpEvent::KeyUpEvent ( const EventProceeder * emitter , Key key )
: Gre::Event( emitter , EventType::KeyUp )
, iKey(key)
{
    
}

Event* KeyUpEvent::clone() const
{
    return new KeyUpEvent ( iEmitter->lock().getObject() , iKey ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowMovedEvent::WindowMovedEvent ( const EventProceeder * emitter , int left , int top )
: Gre::Event( emitter , EventType::WindowMoved )
, Left(left) , Top(top)
{
    
}

Event* WindowMovedEvent::clone() const
{
    return new WindowMovedEvent ( iEmitter->lock().getObject() , Left , Top ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowSizedEvent::WindowSizedEvent ( const EventProceeder * emitter , int width , int height )
: Gre::Event( emitter , EventType::WindowSized )
, Width(width) , Height(height)
{
    
}

Event* WindowSizedEvent::clone() const
{
    return new WindowSizedEvent ( iEmitter->lock().getObject() , Width , Height ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowExposedEvent::WindowExposedEvent ( const EventProceeder * emitter , const Surface& surface )
: Gre::Event( emitter , EventType::WindowExposed )
, iSurface(surface)
{
    
}

Event* WindowExposedEvent::clone() const
{
    return new WindowExposedEvent ( iEmitter->lock().getObject() , iSurface ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowHiddenEvent::WindowHiddenEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowExposed )
{
    
}

Event* WindowHiddenEvent::clone() const
{
    return new WindowHiddenEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowWillCloseEvent::WindowWillCloseEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowWillClose )
{
    
}

Event* WindowWillCloseEvent::clone() const
{
    return new WindowWillCloseEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowTitleChangedEvent::WindowTitleChangedEvent ( const EventProceeder * emitter , const std::string& title )
: Gre::Event( emitter , EventType::WindowTitleChanged )
, iTitle(title)
{
    
}

Event* WindowTitleChangedEvent::clone() const
{
    return new WindowTitleChangedEvent ( iEmitter->lock().getObject() , iTitle ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowAttachContextEvent::WindowAttachContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowAttachContext )
{
    
}

Event* WindowAttachContextEvent::clone() const
{
    return new WindowAttachContextEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowDetachContextEvent::WindowDetachContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowDetachContext )
{
    
}

Event* WindowDetachContextEvent::clone() const
{
    return new WindowDetachContextEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowFocusedEvent::WindowFocusedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowFocused )
{
    
}

Event* WindowFocusedEvent::clone() const
{
    return new WindowFocusedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowUnfocusedEvent::WindowUnfocusedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowUnfocused )
{
    
}

Event* WindowUnfocusedEvent::clone() const
{
    return new WindowUnfocusedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowClosedEvent::WindowClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowClosed )
{
    
}

Event* WindowClosedEvent::clone() const
{
    return new WindowClosedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

LastWindowClosedEvent::LastWindowClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::LastWindowClosed )
{
    
}

Event* LastWindowClosedEvent::clone() const
{
    return new LastWindowClosedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetWillCloseEvent::RenderTargetWillCloseEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetWillClose )
{
    
}

Event* RenderTargetWillCloseEvent::clone() const
{
    return new RenderTargetWillCloseEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetClosedEvent::RenderTargetClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetClosed )
{
    
}

Event* RenderTargetClosedEvent::clone() const
{
    return new RenderTargetClosedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetChangedRenderContextEvent::RenderTargetChangedRenderContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetChangedRenderContext )
{
    
}

Event* RenderTargetChangedRenderContextEvent::clone() const
{
    return new RenderTargetChangedRenderContextEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetChangedFramebufferEvent::RenderTargetChangedFramebufferEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetChangedFramebuffer )
{
    
}

Event* RenderTargetChangedFramebufferEvent::clone() const
{
    return new RenderTargetChangedFramebufferEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RendererRegisteredTargetEvent::RendererRegisteredTargetEvent ( const EventProceeder * emitter , SpecializedCountedObjectUser < EventProceeder > * target )
: Gre::Event( emitter , EventType::RendererRegisteredTarget )
, Target(target)
{
    
}

RendererRegisteredTargetEvent::~RendererRegisteredTargetEvent() noexcept ( false )
{
    if ( Target )
    {
        delete Target ;
    }
}

Event* RendererRegisteredTargetEvent::clone() const
{
    if ( Target ) {
        return new RendererRegisteredTargetEvent ( iEmitter->lock().getObject() , new EventProceederUser(*Target) ) ;
    } else {
        return new RendererRegisteredTargetEvent ( iEmitter->lock().getObject() , nullptr ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

RendererUnregisteredTargetEvent::RendererUnregisteredTargetEvent ( const EventProceeder * emitter , SpecializedCountedObjectUser < EventProceeder > * target )
: Gre::Event( emitter , EventType::RendererUnregisteredTarget )
, Target(target)
{
    
}

RendererUnregisteredTargetEvent::~RendererUnregisteredTargetEvent() noexcept ( false )
{
    if ( Target )
    {
        delete Target ;
    }
}

Event* RendererUnregisteredTargetEvent::clone() const
{
    if ( Target ) {
        return new RendererUnregisteredTargetEvent ( iEmitter->lock().getObject() , new EventProceederUser(*Target) ) ;
    } else {
        return new RendererUnregisteredTargetEvent ( iEmitter->lock().getObject() , nullptr ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

UpdateEvent::UpdateEvent( const EventProceeder * emitter , const Duration& t )
: Event( emitter , EventType::Update )
, elapsedTime(t)
{
    
}

Event* UpdateEvent::clone() const
{
    return new UpdateEvent ( iEmitter->lock().getObject() , elapsedTime ) ;
}

// ---------------------------------------------------------------------------------------------------

ResourceUnloadedEvent::ResourceUnloadedEvent( const EventProceeder * emitter )
: Event( emitter , EventType::ResourceUnloaded )
{
    
}

ResourceUnloadedEvent::~ResourceUnloadedEvent() noexcept ( false )
{
    
}

Event* ResourceUnloadedEvent::clone() const
{
    return new ResourceUnloadedEvent ( iEmitter->lock().getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

CustomEvent::CustomEvent ( const EventProceeder * emitter , const std::map < std::string , Variant > & properties )
: Gre::Event( emitter , EventType::Custom )
, Properties(properties)
{
    
}

Event* CustomEvent::clone() const
{
    return new CustomEvent ( iEmitter->lock().getObject() , Properties ) ;
}

GreEndNamespace