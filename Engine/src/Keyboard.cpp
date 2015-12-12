//
//  Keyboard.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "Keyboard.h"

GRE_BEGIN_NAMESPACE

KeyboardPrivate::KeyboardPrivate(const std::string& name)
: ListenerPrivate(name)
{
    
}

KeyboardPrivate::~KeyboardPrivate()
{
    
}

void KeyboardPrivate::onEvent(const Event &e)
{
    if(e.getType() == EventType::KeyDown)
    {
        const KeyDownEvent& kde = e.to<KeyDownEvent>();
        _keyDown[kde.key] = true;
    }
    
    else if(e.getType() == EventType::KeyUp)
    {
        const KeyUpEvent& kue = e.to<KeyUpEvent>();
        _keyDown[kue.key] = false;
    }
    
    if(e.getType() == EventType::KeyDown || e.getType() == EventType::KeyUp || e.getType() == EventType::Update)
        sendEvent(e);
    
    ListenerPrivate::onEvent(e);
}

bool KeyboardPrivate::isKeyDown(Key k) const
{
    if(_keyDown.find(k) != _keyDown.end())
        return _keyDown.at(k);
    return false;
}

Keyboard::Keyboard(const std::string& name)
: Listener(new KeyboardPrivate(name))
{
    
}

Keyboard::Keyboard(const Keyboard& rhs)
: Listener(rhs)
{
    
}

Keyboard::Keyboard(ListenerPrivate* rhs)
: Listener(rhs)
{
    
}

Keyboard::~Keyboard()
{
    
}

Keyboard::operator Emitter &()
{
    return *(dynamic_cast<Emitter*>(_mListener.get()));
}

Keyboard::operator const Emitter &() const
{
    return *(dynamic_cast<const Emitter*>(_mListener.get()));
}

bool Keyboard::isKeyDown(Key k) const
{
    auto ptr = std::dynamic_pointer_cast<KeyboardPrivate>(_mListener);
    if(ptr)
        return ptr->isKeyDown(k);
    return false;
}

GRE_END_NAMESPACE
