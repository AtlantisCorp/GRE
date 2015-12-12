//
//  Listener.h
//  GRE
//
//  Created by Jacques Tronconi on 18/11/2015.
//
//

#ifndef GRE_Listener_h
#define GRE_Listener_h

#include "Pools.h"
#include "Key.h"
#include "Event.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines physically the Listener object.
/// This object holds data about the listener.
/// The Listener is just an executor which executes functions given
/// by the user when a particular event reach its ::onEvent() method.
class ListenerPrivate
{
public:
    
    POOLED(Pools::Event)
    
    ListenerPrivate (const std::string& name);
    virtual ~ListenerPrivate();
    
    /// @brief Add ann action to the Listener.
    /// An Action is a function which do something when an event of the correct
    /// type reach it. You can use it thanks to lambda functions (C++11 feature only).
    void addAction (EventType etype, std::function<void (const Event&)> eaction);
    
    /// @brief Removes every functions in the Listener.
    /// @note The Listener continues to listen to objects it has been registered to.
    void reset ();
    
    /// @brief Returns the name of the Listener.
    const std::string& getName() const;
    
    /// @brief Calls every methods registered in the Listener.
    /// When overwriting this method, if you still want to add actions, use
    /// ::onEvent(e) to call this class function which correctly calls every
    /// methods.
    virtual void onEvent (const Event& e);
    
protected:
    
    std::string _name;///< @brief Name of the Listener.
    std::map<EventType, std::vector<std::function<void (const Event&)> > > _actions; ///< @brief Actions related to the listener.
};

class DLL_PUBLIC Emitter;

/// @brief Proxy to a ListenerPrivate object.
/// Use this object as if it was the real Listener. It holds a shared_ptr
/// to the ListenerPrivate class object, so you don't have to mind about
/// allocation and deallocation.
class Listener
{
public:
    
    POOLED(Pools::Event)
    
    Listener (const std::string& name);
    Listener (const Listener& rhs);
    explicit Listener (ListenerPrivate* lpriv = nullptr);
    
    virtual ~Listener();
    
    /// @brief Add an action to the listener.
    /// @see ListenerPrivate::addAction()
    void addAction (EventType etype, std::function<void (const Event&)> eaction);
    
    /// @brief Reset the Listener.
    /// @see ListenerPrivate::reset()
    void reset ();
    
    /// @brief Returns the name of the Listener.
    /// @see ListenerPrivate::getName()
    const std::string& getName() const;
    
    /// @brief Make the given emitter to be listened by this listener.
    void listen(Emitter& emitter);
    
    /// @brief Handles event.
    /// @see ListenerPrivate::onEvent()
    void onEvent (const Event& e);
    
    static Listener Null;///< @brief A Null Listener Proxy.
    
protected:
    
    std::shared_ptr<ListenerPrivate> _mListener;
};

GRE_END_NAMESPACE

#endif
