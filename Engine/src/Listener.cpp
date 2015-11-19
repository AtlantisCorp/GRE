//
//  Listener.cpp
//  GRE
//
//  Created by Jacques Tronconi on 19/11/2015.
//
//

#include "Listener.h"

Event::Event(const EventType& etype)
: _mType(etype)
{
    
}

Event::~Event()
{
    
}

const EventType& Event::getType() const
{
    return _mType;
}

KeyDownEvent::KeyDownEvent(const Key& keypressed)
: Event(EventType::KeyDown)
{
    key = keypressed;
}

KeyDownEvent::~KeyDownEvent()
{
    
}

ListenerPrivate::ListenerPrivate(const std::string& name)
: _name(name)
{
    
}

ListenerPrivate::~ListenerPrivate()
{
    
}

void ListenerPrivate::addAction(EventType etype, std::function<void (const Event &)> eaction)
{
    _actions[etype].push_back(eaction);
}

void ListenerPrivate::reset()
{
    _actions.clear();
}

const std::string& ListenerPrivate::getName() const
{
    return _name;
}

void ListenerPrivate::onEvent(const Event &e)
{
    auto actions = _actions[e.getType()];
    if(!actions.empty())
    {
        for(auto itr = actions.begin(); itr != actions.end(); itr++)
        {
            (*itr) (e);
        }
    }
}

Listener::Listener(const std::string& name)
: _mListener(new ListenerPrivate(name))
{
    
}

Listener::Listener(const Listener& rhs)
: _mListener(rhs._mListener)
{
    
}

Listener Listener::Null = Listener((ListenerPrivate*)nullptr);

Listener::Listener(ListenerPrivate* lpriv)
: _mListener(lpriv)
{
    
}

Listener::~Listener()
{
    
}

void Listener::addAction(EventType etype, std::function<void (const Event &)> eaction)
{
    _mListener->addAction(etype, eaction);
}

void Listener::reset()
{
    _mListener->reset();
}

const std::string& Listener::getName() const
{
    return _mListener->getName();
}

void Listener::onEvent(const Event &e)
{
    _mListener->onEvent(e);
}

Emitter::Emitter()
{
    
}

Emitter::~Emitter()
{
    
}

Listener& Emitter::addListener(const std::string &name)
{
    _mListeners.insert(std::pair<std::string, Listener>(name, Listener(name)));
    return _mListeners[name];
}

Listener Emitter::getListener(const std::string &name)
{
    auto itr = _mListeners.find(name);
    if(itr != _mListeners.end())
        return itr->second;
    return Listener::Null;
}

void Emitter::removeListener(const std::string &name)
{
    auto itr = _mListeners.find(name);
    if(itr != _mListeners.end())
        _mListeners.erase(itr);
}

void Emitter::sendEvent(const Event &e)
{
    for(auto itr = _mListeners.begin(); itr != _mListeners.end(); itr++)
    {
        itr->second.onEvent(e);
    }
}

