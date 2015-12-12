//
//  Event.cpp
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#include "Event.h"

GRE_BEGIN_NAMESPACE

Event::Event(const EventType& etype)
: _mType(etype)
{
    
}

Event::~Event()
{
    
}

const EventType& Event::getType() const
{
    return _mType;
}

KeyDownEvent::KeyDownEvent(const Key& keypressed)
: Event(EventType::KeyDown)
{
    key = keypressed;
}

KeyDownEvent::~KeyDownEvent()
{
    
}

KeyUpEvent::KeyUpEvent(const Key& keyreleased)
: Event(EventType::KeyUp)
{
    key = keyreleased;
}

KeyUpEvent::~KeyUpEvent()
{
    
}

UpdateEvent::UpdateEvent()
: Event(EventType::Update)
{
    
}

UpdateEvent::~UpdateEvent()
{
    
}

GRE_END_NAMESPACE