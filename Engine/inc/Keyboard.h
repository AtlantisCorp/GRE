//
//  Keyboard.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_Keyboard_h
#define GRE_Keyboard_h

#include "Listener.h"
#include "Emitter.h"

GRE_BEGIN_NAMESPACE

/// @brief A ListenerPrivate object that listen to keys and then distribute it to other
/// listeners.
class DLL_PUBLIC KeyboardPrivate : public ListenerPrivate,
                                   public Emitter
{
public:
    
    POOLED(Pools::Event)
    
    KeyboardPrivate(const std::string& name);
    ~KeyboardPrivate();
    
    /// @brief Calls every methods registered in the Listener.
    /// Also distribute the Key events to registered listeners.
    void onEvent (const Event& e);
    
    /// @brief Returns true if given key is down.
    bool isKeyDown(Key k) const;
    
private:
    
    std::map<Key, bool> _keyDown;
};

/// @brief This proxy is here to ensure that you can listen to the KeyboardPrivate object.
class DLL_PUBLIC Keyboard : public Listener
{
public:
    
    POOLED(Pools::Event)
    
    Keyboard(const std::string& name);
    Keyboard (const Keyboard& rhs);
    explicit Keyboard (ListenerPrivate* lpriv = nullptr);
    
    ~Keyboard();
    
    operator Emitter& ();
    operator const Emitter& () const;
    
    /// @brief Returns true if given key is down.
    bool isKeyDown(Key k) const;
};

GRE_END_NAMESPACE

#endif
