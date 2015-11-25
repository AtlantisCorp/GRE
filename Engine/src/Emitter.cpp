//
//  Emitter.cpp
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#include "Emitter.h"

GRE_BEGIN_NAMESPACE

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

Listener& Emitter::addListener(const Listener &listener)
{
    _mListeners.insert(std::pair<std::string, Listener>(listener.getName(), listener));
    return _mListeners[listener.getName()];
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

GRE_END_NAMESPACE
