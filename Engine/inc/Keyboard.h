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
#include "ResourceLoader.h"
#include "SpecializedResourceManager.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Basic Resource to handle more easily Key Events.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Keyboard : public Resource
{
public:
    
    POOLED(Pools::Event)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Keyboard(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~Keyboard() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given key is down.
    //////////////////////////////////////////////////////////////////////
    bool isKeyDown(Key k) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    void unload () ;
    
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

/// @brief SpecializedCountedObjectHolder for KeyboardPrivate.
typedef SpecializedCountedObjectHolder<Keyboard> KeyboardHolder;

/// @brief SpecializedResourceHolderList for KeyboardPrivate.
typedef SpecializedResourceHolderList<Keyboard> KeyboardHolderList;

/// @brief SpecializedCountedObjectUser for Keyboard
typedef SpecializedCountedObjectUser<Keyboard> KeyboardUser;

//////////////////////////////////////////////////////////////////////
/// @brief A simple ResourceLoader to permit to the ResourceManager
/// to load a Keyboard Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC KeyboardLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Event)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    KeyboardLoader();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~KeyboardLoader() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Keyboard.
    //////////////////////////////////////////////////////////////////////
    virtual KeyboardHolder load ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    //////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
};

//////////////////////////////////////////////////////////////////////
/// @brief Keyboard basic manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC KeyboardManager : public SpecializedResourceManager < Keyboard , KeyboardLoader >
{
public:
    
    POOLED ( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    KeyboardManager ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    KeyboardManager ( const std::string & name ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~KeyboardManager ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Keyboard with given name .
    //////////////////////////////////////////////////////////////////////
    virtual KeyboardUser load ( const std::string & kbdname ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener which listens to any of the Keyboard loaded
    /// by the Keyboard Manager.
    //////////////////////////////////////////////////////////////////////
    virtual void addGlobalKeyListener ( const SpecializedCountedObjectUser<EventProceeder> & listener ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every listener.
    //////////////////////////////////////////////////////////////////////
    virtual void clearGlobalKeyListeners () ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sends a key event to 'iGlobalKeyListeners' .
    //////////////////////////////////////////////////////////////////////
    virtual void iSendGlobalKeyEvent ( EventHolder & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Listens to every Keyboard loaded.
    //////////////////////////////////////////////////////////////////////
    class KeyboardListener : public EventProceeder
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        KeyboardListener ( KeyboardManager * creator ) ;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        ~KeyboardListener () noexcept ( false ) ;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        void onEvent ( EventHolder & e ) ;
        
    protected:
        
        /// @brief Creator of this listener.
        KeyboardManager * iManager ;
    };
    
    typedef SpecializedCountedObjectHolder<KeyboardListener> KeyboardListenerHolder ;
    
protected:
    
    typedef std::list < SpecializedCountedObjectUser<EventProceeder> > EventProceederList ;
    
    /// @brief Listener for every Keyboards loaded.
    KeyboardListenerHolder iKeyboardListener ;
    
    /// @brief Holds Listeners for iKeyboardListener .
    EventProceederList iGlobalKeyListeners ;
};

GreEndNamespace

#endif
