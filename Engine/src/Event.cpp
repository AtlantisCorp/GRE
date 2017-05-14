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

    iEmitter = new Holder < EventProceeder > ( emitter ) ;
    iNoSublisteners = false ;

    if ( !iEmitter )
        throw GreExceptionWithText("'iEmitter' couldn't be allocated.") ;
}

Event::~Event() noexcept(false)
{
    if ( iEmitter )
        delete iEmitter ;
}

const Holder<EventProceeder> & Event::getEmitter() const
{
    GreAutolock ;

    if ( !iEmitter )
    {
        throw GreExceptionWithText ( "Bad 'Event::iEmitter' property." ) ;
    }

    return * iEmitter ;
}

const EventProceeder* Event::getEmitterPointer() const
{
    GreAutolock ; return iEmitter->getObject() ;
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

bool Event::noSublisteners() const
{
    GreAutolock ; return iNoSublisteners ;
}

void Event::setNoSublisteners(bool value)
{
    GreAutolock ; iNoSublisteners = value ;
}

// ---------------------------------------------------------------------------------------------------

KeyDownEvent::KeyDownEvent ( const EventProceeder * emitter , Key key , int mods )
: Gre::Event( emitter , EventType::KeyDown )
, iKey(key) , iModifiers(mods)
{

}

Event* KeyDownEvent::clone() const
{
    return new KeyDownEvent ( iEmitter->getObject() , iKey , iModifiers ) ;
}

// ---------------------------------------------------------------------------------------------------

KeyUpEvent::KeyUpEvent ( const EventProceeder * emitter , Key key , int mods )
: Gre::Event( emitter , EventType::KeyUp )
, iKey(key) , iModifiers(mods)
{

}

Event* KeyUpEvent::clone() const
{
    return new KeyUpEvent ( iEmitter->getObject() , iKey , iModifiers ) ;
}

// ---------------------------------------------------------------------------------------------------

CursorMovedEvent::CursorMovedEvent ( const EventProceeder* emitter , const float deltax , const float deltay )
: Gre::Event ( emitter , EventType::CursorMoved ) , DeltaX ( deltax ) , DeltaY ( deltay )
{

}

Event* CursorMovedEvent::clone() const
{
    return new CursorMovedEvent ( iEmitter->getObject() , DeltaX , DeltaY ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowMovedEvent::WindowMovedEvent ( const EventProceeder * emitter , int left , int top )
: Gre::Event( emitter , EventType::WindowMoved )
, Left(left) , Top(top)
{

}

Event* WindowMovedEvent::clone() const
{
    return new WindowMovedEvent ( iEmitter->getObject() , Left , Top ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowSizedEvent::WindowSizedEvent ( const EventProceeder * emitter , int width , int height )
: Gre::Event( emitter , EventType::WindowSized )
, Width(width) , Height(height)
{

}

Event* WindowSizedEvent::clone() const
{
    return new WindowSizedEvent ( iEmitter->getObject() , Width , Height ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowExposedEvent::WindowExposedEvent ( const EventProceeder * emitter , const Surface& surface )
: Gre::Event( emitter , EventType::WindowExposed )
, iSurface(surface)
{

}

Event* WindowExposedEvent::clone() const
{
    return new WindowExposedEvent ( iEmitter->getObject() , iSurface ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowHiddenEvent::WindowHiddenEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowExposed )
{

}

Event* WindowHiddenEvent::clone() const
{
    return new WindowHiddenEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowWillCloseEvent::WindowWillCloseEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowWillClose )
{

}

Event* WindowWillCloseEvent::clone() const
{
    return new WindowWillCloseEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowTitleChangedEvent::WindowTitleChangedEvent ( const EventProceeder * emitter , const std::string& title )
: Gre::Event( emitter , EventType::WindowTitleChanged )
, iTitle(title)
{

}

Event* WindowTitleChangedEvent::clone() const
{
    return new WindowTitleChangedEvent ( iEmitter->getObject() , iTitle ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowAttachContextEvent::WindowAttachContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowAttachContext )
{

}

Event* WindowAttachContextEvent::clone() const
{
    return new WindowAttachContextEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowDetachContextEvent::WindowDetachContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowDetachContext )
{

}

Event* WindowDetachContextEvent::clone() const
{
    return new WindowDetachContextEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowFocusedEvent::WindowFocusedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowFocused )
{

}

Event* WindowFocusedEvent::clone() const
{
    return new WindowFocusedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowUnfocusedEvent::WindowUnfocusedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowUnfocused )
{

}

Event* WindowUnfocusedEvent::clone() const
{
    return new WindowUnfocusedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

WindowClosedEvent::WindowClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::WindowClosed )
{

}

Event* WindowClosedEvent::clone() const
{
    return new WindowClosedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

LastWindowClosedEvent::LastWindowClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::LastWindowClosed )
{

}

Event* LastWindowClosedEvent::clone() const
{
    return new LastWindowClosedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetWillCloseEvent::RenderTargetWillCloseEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetWillClose )
{

}

Event* RenderTargetWillCloseEvent::clone() const
{
    return new RenderTargetWillCloseEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetClosedEvent::RenderTargetClosedEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetClosed )
{

}

Event* RenderTargetClosedEvent::clone() const
{
    return new RenderTargetClosedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetChangedRenderContextEvent::RenderTargetChangedRenderContextEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetChangedRenderContext )
{

}

Event* RenderTargetChangedRenderContextEvent::clone() const
{
    return new RenderTargetChangedRenderContextEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderTargetChangedFramebufferEvent::RenderTargetChangedFramebufferEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderTargetChangedFramebuffer )
{

}

Event* RenderTargetChangedFramebufferEvent::clone() const
{
    return new RenderTargetChangedFramebufferEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RendererRegisteredTargetEvent::RendererRegisteredTargetEvent ( const EventProceeder * emitter , Holder < EventProceeder > * target )
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
        return new RendererRegisteredTargetEvent ( iEmitter->getObject() , new Holder < EventProceeder > (*Target) ) ;
    } else {
        return new RendererRegisteredTargetEvent ( iEmitter->getObject() , nullptr ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

RendererUnregisteredTargetEvent::RendererUnregisteredTargetEvent ( const EventProceeder * emitter , Holder < EventProceeder > * target )
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
        return new RendererUnregisteredTargetEvent ( iEmitter->getObject() , new Holder < EventProceeder > (*Target) ) ;
    } else {
        return new RendererUnregisteredTargetEvent ( iEmitter->getObject() , nullptr ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

RenderScenePreRenderEvent::RenderScenePreRenderEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderScenePreRender )
{

}

Event* RenderScenePreRenderEvent::clone() const
{
    return new RenderScenePreRenderEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderScenePostRenderEvent::RenderScenePostRenderEvent ( const EventProceeder * emitter )
: Gre::Event( emitter , EventType::RenderScenePostRender )
{

}

Event* RenderScenePostRenderEvent::clone() const
{
    return new RenderScenePostRenderEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

UpdateEvent::UpdateEvent( const EventProceeder * emitter , const Duration& t )
: Event( emitter , EventType::Update )
, elapsedTime(t)
{

}

Event* UpdateEvent::clone() const
{
    return new UpdateEvent ( iEmitter->getObject() , elapsedTime ) ;
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
    return new ResourceUnloadedEvent ( iEmitter->getObject() ) ;
}

// ---------------------------------------------------------------------------------------------------

PositionChangedEvent::PositionChangedEvent ( const EventProceeder* emitter , const Vector3& pos )
: Gre::Event(emitter, EventType::PositionChanged) , Position(pos)
{

}

Event* PositionChangedEvent::clone() const
{
    return new PositionChangedEvent ( iEmitter->getObject() , Position ) ;
}

// ---------------------------------------------------------------------------------------------------

DirectionChangedEvent::DirectionChangedEvent ( const EventProceeder* emitter , const Vector3& dir )
: Gre::Event(emitter, EventType::DirectionChanged) , Direction(dir)
{

}

Event* DirectionChangedEvent::clone() const
{
    return new DirectionChangedEvent ( iEmitter->getObject() , Direction ) ;
}

// ---------------------------------------------------------------------------------------------------

CustomEvent::CustomEvent ( const EventProceeder * emitter , const std::map < std::string , Variant > & properties )
: Gre::Event( emitter , EventType::Custom )
, Properties(properties)
{

}

Event* CustomEvent::clone() const
{
    return new CustomEvent ( iEmitter->getObject() , Properties ) ;
}

GreEndNamespace
