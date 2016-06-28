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
    if(ikeyDown.find(k) != ikeyDown.end())
        return ikeyDown.at(k);
    return false;
}

void KeyboardPrivate::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    ikeyDown[e.key] = false;
}

void KeyboardPrivate::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    ikeyDown[e.key] = true;
}

// ---------------------------------------------------------------------------------------------------

Keyboard::Keyboard(Resource* resource)
: SpecializedResourceUser<KeyboardPrivate>(resource)
{
    
}

Keyboard::Keyboard(const ResourceUser& user)
: SpecializedResourceUser<KeyboardPrivate>(user)
{
    
}

Keyboard::Keyboard(const Keyboard& user)
: SpecializedResourceUser<Gre::KeyboardPrivate>(user)
{
    
}

Keyboard& Keyboard::operator=(const Gre::Keyboard &rhs)
{
    SpecializedResourceUser<KeyboardPrivate>::operator=(rhs);
    return *this;
}

Keyboard::~Keyboard()
{
    
}

bool Keyboard::isKeyDown(Key k) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isKeyDown(k);
    return false;
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
