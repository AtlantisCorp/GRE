//////////////////////////////////////////////////////////////////////
//
//  Keyboard.h
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

#ifndef GRE_Keyboard_h
#define GRE_Keyboard_h

#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Basic Resource to handle more easily Key Events.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC KeyboardPrivate : public Resource
{
public:
    
    POOLED(Pools::Event)
    
    KeyboardPrivate(const std::string& name);
    ~KeyboardPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given key is down.
    //////////////////////////////////////////////////////////////////////
    bool isKeyDown(Key k) const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is up.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyUpEvent(const KeyUpEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is down.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyDownEvent(const KeyDownEvent& e);
    
private:
    
    /// @brief Helper to store Key currently down.
    std::map<Key, bool> ikeyDown;
};

/// @brief SpecializedResourceHolder for KeyboardPrivate.
typedef SpecializedResourceHolder<KeyboardPrivate> KeyboardHolder;

/// @brief SpecializedResourceHolderList for KeyboardPrivate.
typedef SpecializedResourceHolderList<KeyboardPrivate> KeyboardHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief ResourceUser to manipulate the KeyboardPrivate class.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Keyboard : public SpecializedResourceUser<KeyboardPrivate>
{
public:
    
    POOLED(Pools::Event)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Keyboard(Resource* resource = nullptr);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Keyboard(const ResourceUser& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Keyboard(const Keyboard& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Keyboard& operator = (const Keyboard& rhs);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Keyboard();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given key is down.
    //////////////////////////////////////////////////////////////////////
    bool isKeyDown(Key k) const;
};

//////////////////////////////////////////////////////////////////////
/// @brief A simple ResourceLoader to permit to the ResourceManager
/// to load a Keyboard Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC KeyboardLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Event)
    
    KeyboardLoader();
    ~KeyboardLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::Keyboard.
    //////////////////////////////////////////////////////////////////////
    bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Keyboard.
    //////////////////////////////////////////////////////////////////////
    Resource* load(Resource::Type type, const std::string& name) const;
};

GreEndNamespace

#endif
