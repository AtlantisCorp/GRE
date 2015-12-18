//
//  Emitter.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Emitter_h
#define GRE_Emitter_h

#include "Listener.h"

GRE_BEGIN_NAMESPACE

/// @brief An event emitter.
/// The Emitter object holds pointers to Listener object,
/// and send them events. The treatment of this event belongs
/// to the Listener object.
/// Proxies are used to hold the Listener objects.
class DLL_PUBLIC Emitter
{
public:
    
    POOLED(Pools::Event)
    
    Emitter ();
    virtual ~Emitter ();
    
    /// @brief Adds a listener giving him a new name.
    /// This function creates a new Generic Listener and returns it.
    Listener& addListener(const std::string& name);
    
    /// @brief Adds a listener to the emitter list, and returns a reference
    /// to the generic listener added.
    Listener& addListener(const Listener& listener);
    
    /// @brief Returns a shared_pointer to the Generic Listener pointer by its
    /// name.
    Listener getListener(const std::string& name);
    
    /// @brief Remove the given Listener from the list.
    void removeListener(const std::string& name);
    
    /// @brief Send an event to every listeners.
    void sendEvent(const Event& e);
    
protected:
    
    std::map<std::string, Listener> _mListeners;
};

//////////////////////////////////////////////////////////////////////
/// @brief A particular type of Emitter, which can listen to other
/// Emitters and transmit their events.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Transmitter : public Listener, public Emitter
{
public:
    
    POOLED(Pools::Event)
    
    Transmitter(const std::string& name);
    virtual ~Transmitter();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Transmits the event to the Listeners regitered, and then
    /// treat the event.
    //////////////////////////////////////////////////////////////////////
    virtual void onEvent(const Event& e);
};

GRE_END_NAMESPACE
#endif
