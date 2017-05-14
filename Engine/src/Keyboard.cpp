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

// ---------------------------------------------------------------------------------------------------

Keyboard::Keyboard(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{

}

Keyboard::~Keyboard() noexcept ( false )
{

}

bool Keyboard::isKeyDown(Key k) const
{
    if(ikeyDown.find(k) != ikeyDown.end())
        return ikeyDown.at(k);
    return false;
}

void Keyboard::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    ikeyDown[e.iKey] = false;
}

void Keyboard::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    ikeyDown[e.iKey] = true;
}

void Keyboard::unload()
{
    ikeyDown.clear() ;
}

// ---------------------------------------------------------------------------------------------------

KeyboardLoader::KeyboardLoader()
{

}

KeyboardLoader::~KeyboardLoader() noexcept(false)
{

}

KeyboardHolder KeyboardLoader::load(const std::string &name) const
{
    return KeyboardHolder ( new Keyboard ( name ) );
}

bool KeyboardLoader::isLoadable(const std::string &filepath) const
{
    return true;
}

ResourceLoader* KeyboardLoader::clone() const
{
    return new KeyboardLoader();
}

// ---------------------------------------------------------------------------------------------------

KeyboardManager::KeyboardManager ()
: Gre::SpecializedResourceManager < Keyboard , KeyboardLoader > ()
, iKeyboardListener(new KeyboardListener(this))
{

}

KeyboardManager::KeyboardManager ( const std::string & name )
: Gre::SpecializedResourceManager < Keyboard , KeyboardLoader > ( name )
, iKeyboardListener ( new KeyboardListener(this) )
{

}

KeyboardManager::~KeyboardManager () noexcept ( false )
{

}

KeyboardHolder KeyboardManager::load(const std::string &kbdname)
{
    GreAutolock ;

    KeyboardHolder kbd ( new Keyboard ( kbdname ) ) ;

    if ( !kbd.isInvalid() )
    {
        iHolders.add(kbd);
        addListener( KeyboardHolder(kbd) ) ;

        if ( !iKeyboardListener.isInvalid() )
        {
            kbd->addListener( iKeyboardListener ) ;
        }

        return kbd ;
    }

    else
    {
        return KeyboardHolder ( nullptr ) ;
    }
}

void KeyboardManager::addGlobalKeyListener(const Holder<Gre::EventProceeder> &listener)
{
    GreAutolock ; iGlobalKeyListeners.push_back(listener) ;
}

void KeyboardManager::clearGlobalKeyListeners()
{
    GreAutolock ; iGlobalKeyListeners.clear() ;
}

void KeyboardManager::iSendGlobalKeyEvent(EventHolder &e)
{
    GreAutolock ;

    for ( EventProceederHolder & proceeder : iGlobalKeyListeners )
    {
        if ( !proceeder.isInvalid() )
        {
            proceeder->onEvent( e ) ;
        }
    }
}

// ---------------------------------------------------------------------------------------------------

KeyboardManager::KeyboardListener::KeyboardListener ( KeyboardManager * creator )
: Gre::EventProceeder() , iManager(creator)
{
    if ( !creator )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "KeyboardManager::KeyboardListener can't be initialized with a null manager." << Gre::gendl ;
#endif
        throw GreExceptionWithText ("KeyboardManager::KeyboardListener can't be initialized with a null manager.") ;
    }
}

KeyboardManager::KeyboardListener::~KeyboardListener() noexcept ( false )
{

}

void KeyboardManager::KeyboardListener::onEvent(EventHolder &e)
{
    if ( iManager )
    {
        iManager->iSendGlobalKeyEvent(e) ;
    }
}

GreEndNamespace
