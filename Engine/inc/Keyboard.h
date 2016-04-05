//
//  Keyboard.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

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
    std::map<Key, bool> _keyDown;
};

//////////////////////////////////////////////////////////////////////
/// @brief ResourceUser to manipulate the KeyboardPrivate class.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Keyboard : public ResourceUser
{
public:
    
#ifndef GreExtraMacros
    
    POOLED(Pools::Event)
    
    Keyboard();
    Keyboard(const Keyboard& rhs);
    Keyboard(Keyboard&& rhs);
    explicit Keyboard(const ResourceUser& rhs);
    Keyboard& operator = (const Keyboard& rhs);
    bool operator == (const Keyboard& rhs) const;
    bool operator != (const Keyboard& rhs) const;
    
    ~Keyboard();
    
#else
    
    GreResourceUserMakeConstructorsPooled(Keyboard, Event);
    
#endif
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given key is down.
    //////////////////////////////////////////////////////////////////////
    bool isKeyDown(Key k) const;
    
private:
    
    /// @brief Holds a pointer to the Keyboard.
    std::weak_ptr<KeyboardPrivate> _kbd;
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
