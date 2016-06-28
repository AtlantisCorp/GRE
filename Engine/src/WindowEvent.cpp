//
//  WindowEvents.cpp
//  GRE
//
//  Created by Jacques Tronconi on 06/04/2016.
//
//

#include "WindowEvent.h"

GreBeginNamespace

WindowEvent::WindowEvent(const Window& emitter)
: Gre::Event(EventType::Window), iEmitter(emitter)
{

}

WindowEvent::WindowEvent(const WindowEvent& event)
: Gre::Event(event), iEmitter(event.iEmitter)
{
    
}

WindowEvent::~WindowEvent()
{
    
}

const Window& WindowEvent::getEmitter() const
{
    return iEmitter;
}

Window& WindowEvent::getEmitter()
{
    return iEmitter;
}

// ---------------------------------------------------------------------------------------------------

WindowSizedEvent::WindowSizedEvent(const Window& emitter, const Surface& newsize)
: Gre::WindowEvent(emitter), iSurface(newsize)
{
    _mType = EventType::WindowSized;
}

WindowSizedEvent::WindowSizedEvent(const WindowSizedEvent& event)
: Gre::WindowEvent(event), iSurface(event.iSurface)
{
    _mType = EventType::WindowSized;
}

WindowSizedEvent::~WindowSizedEvent()
{
    
}

const Surface& WindowSizedEvent::getSurface() const
{
    return iSurface;
}

GreEndNamespace
