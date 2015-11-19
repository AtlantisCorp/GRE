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

enum class Key
{
    V = 0x9
};

enum class EventType
{
    KeyDown
};

class Event
{
public:
    
    POOLED(Pools::Event)
    
    Event (const EventType& etype);
    virtual ~Event();
    
    const EventType& getType() const;
    
    template<typename T>
    const T& to() const { return *reinterpret_cast<T*>(const_cast<Event*>(this)); }
    
protected:
    
    EventType _mType;
};

class KeyDownEvent : public Event
{
public:
    
    POOLED(Pools::Event)
    
    KeyDownEvent (const Key& pressedKey);
    virtual ~KeyDownEvent ();
    
    Key key;
};

class ListenerPrivate
{
public:
    
    POOLED(Pools::Event)
    
    ListenerPrivate (const std::string& name);
    virtual ~ListenerPrivate();
    
    void addAction (EventType etype, std::function<void (const Event&)> eaction);
    void reset ();
    
    const std::string& getName() const;
    
    virtual void onEvent (const Event& e);
    
protected:
    
    std::string _name;
    std::map<EventType, std::vector<std::function<void (const Event&)> > > _actions;
};

class Listener
{
public:
    
    POOLED(Pools::Event)
    
    Listener (const std::string& name);
    Listener (const Listener& rhs);
    explicit Listener (ListenerPrivate* lpriv = nullptr);
    
    virtual ~Listener();
    
    void addAction (EventType etype, std::function<void (const Event&)> eaction);
    void reset ();
    
    const std::string& getName() const;
    
    void onEvent (const Event& e);
    
    static Listener Null;
    
protected:
    
    std::shared_ptr<ListenerPrivate> _mListener;
};

class Emitter
{
public:
    
    POOLED(Pools::Event)
    
    Emitter ();
    virtual ~Emitter ();
    
    Listener& addListener(const std::string& name);
    Listener getListener(const std::string& name);
    void removeListener(const std::string& name);
    
    void sendEvent(const Event& e);
    
protected:
    
    std::map<std::string, Listener> _mListeners;
};

#endif
