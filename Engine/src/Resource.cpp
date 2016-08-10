//////////////////////////////////////////////////////////////////////
//
//  Resource.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/05/2016.
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

#include "Resource.h"
#include "ResourceManager.h"

GreBeginNamespace

Resource::Resource(const std::string& name)
{
    iName = name;
    iType = Resource::Type::Null;
    iShouldTransmit = false;
    iCounter = nullptr;
    iCounterInitialized = false;
}

Resource::~Resource() noexcept(false)
{
    if(iCounterInitialized)
    {
        iCounter->unuse();
        
        if(iCounter->getUserCount() == 0)
        {
            delete iCounter;
            iCounter = nullptr;
            iCounterInitialized = false;
        }
    }
}

const std::string& Resource::getName() const
{
    return iName;
}

const Resource::Type& Resource::getType() const
{
    return iType;
}

void Resource::addAction(Gre::EventType etype, std::function<void (const Event &)> eaction)
{
    iActions[etype].push_back(eaction);
}

void Resource::resetActions()
{
    iActions.clear();
}

void Resource::onEvent(const Event &e)
{
    if(e.getType() == EventType::Update) {
        onUpdateEvent(e.to<UpdateEvent>());
    }
    
    else if(e.getType() == EventType::KeyUp) {
        onKeyUpEvent(e.to<KeyUpEvent>());
    }
    
    else if(e.getType() == EventType::KeyDown) {
        onKeyDownEvent(e.to<KeyDownEvent>());
    }
    
    auto actions = iActions[e.getType()];
    if(!actions.empty())
    {
        for(auto itr = actions.begin(); itr != actions.end(); itr++)
        {
            (*itr) (e);
        }
    }
    
    // Proceeds to the iNextCallbacks functions.
    
    if ( !iNextCallbacks[e.getType()].empty() )
    {
        for ( auto callback : iNextCallbacks[e.getType()] )
        {
            callback(e);
        }
        
        iNextCallbacks[e.getType()].clear();
    }
    
    // Send events to listener if it has to.
    
    if(shouldTransmitEvents())
    {
        sendEvent(e);
    }
}

void Resource::onNextEvent(const Gre::EventType &etype, EventCallback callback)
{
    iNextCallbacks[etype].push_back(callback);
}

void Resource::onUpdateEvent(const Gre::UpdateEvent &e)
{
    
}

void Resource::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    
}

void Resource::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    
}

ResourceUser& Resource::addListener(const std::string& name)
{
    ResourceUser pureListener = ResourceManager::Get().createPureListener(name);
    return addListener(pureListener);
}

ResourceUser& Resource::addListener(const ResourceUser& listener)
{
    iListeners.insert(std::pair<std::string, ResourceUser>(listener.getName(), listener));
    return iListeners.at(listener.getName());
}

ResourceUser Resource::getListener(const std::string &name)
{
    return iListeners.at(name);
}

void Resource::removeListener(const std::string &name)
{
    auto it = iListeners.find(name);
    if(it != iListeners.end())
    {
        iListeners.erase(it);
    }
}

void Resource::sendEvent(const Event &e)
{
    for(auto itr = iListeners.begin(); itr != iListeners.end(); itr++)
    {
        itr->second.onEvent(e);
    }
}

void Resource::setShouldTransmitEvents(bool p)
{
    iShouldTransmit = p;
}

bool Resource::shouldTransmitEvents() const
{
    return iShouldTransmit;
}

void Resource::acquire()
{
    if(!iCounterInitialized)
    {
        iCounter = new ReferenceCounter;
        iCounter->use();
        iCounterInitialized = true;
    }
    
    iCounter->hold();
}

void Resource::release()
{
    if(iCounterInitialized)
    {
        iCounter->unhold();
        
        if(iCounter->getHolderCount() == 0)
        {
            delete this;
        }
    }
}

int Resource::getCounterValue() const
{
    return iCounter->getHolderCount();
}

ReferenceCounter* Resource::getReferenceCounter()
{
    return iCounter;
}

Variant& Resource::getCustomData(const std::string &entry)
{
    return iCustomData[entry];
}

const Variant& Resource::getCustomData(const std::string &entry) const
{
    return iCustomData[entry];
}

void Resource::setCustomData(const std::string &entry, const Gre::Variant &data)
{
    iCustomData[entry] = data;
}

GreEndNamespace
