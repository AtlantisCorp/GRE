//////////////////////////////////////////////////////////////////////
//
//  Keyboard.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 11/12/2015.
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

Keyboard::Keyboard(const KeyboardPrivate* resource)
: Gre::ResourceUser(resource)
, SpecializedResourceUser<KeyboardPrivate>(resource)
{
    
}

Keyboard::Keyboard(const KeyboardHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<KeyboardPrivate>(holder)
{
    
}

Keyboard::Keyboard(const Keyboard& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::KeyboardPrivate>(user)
{
    
}

Keyboard::~Keyboard() noexcept(false)
{
    
}

bool Keyboard::isKeyDown(Key k) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isKeyDown(k);
    return false;
}

Keyboard Keyboard::Null = Keyboard ( nullptr );

// ---------------------------------------------------------------------------------------------------

KeyboardLoader::KeyboardLoader()
{
    
}

KeyboardLoader::~KeyboardLoader() noexcept(false)
{
    
}

KeyboardHolder KeyboardLoader::load(const std::string &name) const
{
    return KeyboardHolder ( new KeyboardPrivate ( name ) );
}

bool KeyboardLoader::isLoadable(const std::string &filepath) const
{
    return true;
}

ResourceLoader* KeyboardLoader::clone() const
{
    return new KeyboardLoader();
}

GreEndNamespace
