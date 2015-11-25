//
//  Listener.cpp
//  GRE
//
//  Created by Jacques Tronconi on 19/11/2015.
//
//

#include "Listener.h"

GRE_BEGIN_NAMESPACE

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

GRE_END_NAMESPACE

