//
//  Keyboard.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "Keyboard.h"

GreBeginNamespace

KeyboardPrivate::KeyboardPrivate(const std::string& name)
: Resource(name)
{
    setShouldTransmitEvents(true);
}

KeyboardPrivate::~KeyboardPrivate()
{
    
}

bool KeyboardPrivate::isKeyDown(Key k) const
{
    if(_keyDown.find(k) != _keyDown.end())
        return _keyDown.at(k);
    return false;
}

void KeyboardPrivate::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    _keyDown[e.key] = false;
}

void KeyboardPrivate::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    _keyDown[e.key] = true;
}

// ---------------------------------------------------------------------------------------------------

Keyboard::Keyboard()
: ResourceUser(), _kbd()
{
    
}

Keyboard::Keyboard(const Keyboard& rhs)
: ResourceUser(rhs), _kbd(rhs._kbd)
{
    
}

Keyboard::Keyboard(Keyboard&& rhs)
: ResourceUser(rhs), _kbd(std::move(rhs._kbd))
{
    
}

Keyboard::Keyboard(const ResourceUser& rhs)
: ResourceUser(rhs), _kbd(std::dynamic_pointer_cast<KeyboardPrivate>(rhs.lock()))
{
    
}

Keyboard& Keyboard::operator=(const Keyboard &rhs)
{
    ResourceUser::operator=(rhs);
    _kbd = rhs._kbd;
    return *this;
}

bool Keyboard::operator==(const Gre::Keyboard &rhs) const
{
    return _kbd.lock() == rhs._kbd.lock();
}

bool Keyboard::operator!=(const Gre::Keyboard &rhs) const
{
    return !(*this == rhs);
}

bool Keyboard::isKeyDown(Key k) const
{
    auto ptr = _kbd.lock();
    if(ptr)
        return ptr->isKeyDown(k);
    return false;
}

Keyboard::~Keyboard()
{
    
}

// ---------------------------------------------------------------------------------------------------

KeyboardLoader::KeyboardLoader()
{
    
}

KeyboardLoader::~KeyboardLoader()
{
    
}

bool KeyboardLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Keyboard;
}

Resource* KeyboardLoader::load(Resource::Type type, const std::string &name) const
{
    if(isTypeSupported(type))
    {
        return new KeyboardPrivate(name);
    }
    
    else
    {
        return nullptr;
    }
}

GreEndNamespace
