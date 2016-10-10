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
    iShouldTransmit = true;
    iCounter = nullptr;
    iCounterInitialized = false;
}

Resource::~Resource() noexcept(false)
{
    {
        GreResourceAutolock ;
        
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
    GreResourceAutolock ;
    iActions[etype].push_back(eaction);
}

void Resource::resetActions()
{
    GreResourceAutolock ;
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
    
    // As those functions are not certainly locked , we must ensure multithread availability and lock
    // the recursive mutex.
    
    GreResourceAutolock ;
    
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
    
    if( shouldTransmitEvents() )
    {
        sendEvent(e);
    }
}

void Resource::onNextEvent(const Gre::EventType &etype, EventCallback callback)
{
    GreResourceAutolock ;
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
    GreResourceAutolock ;
    ResourceUser pureListener = ResourceManager::Get().loadEmptyResource(name);
    return addListener(pureListener);
}

ResourceUser& Resource::addListener(const ResourceUser& listener)
{
    GreResourceAutolock ;
    iListeners.insert(std::pair<std::string, ResourceUser>(listener.getName(), listener));
    return iListeners.at(listener.getName());
}

ResourceUser Resource::getListener(const std::string &name)
{
    GreResourceAutolock ;
    return iListeners.at(name);
}

void Resource::removeListener(const std::string &name)
{
    GreResourceAutolock ;
    auto it = iListeners.find(name);
    if(it != iListeners.end())
    {
        iListeners.erase(it);
    }
}

void Resource::sendEvent(const Event &e)
{
    GreResourceAutolock ;
    
    for(auto itr = iListeners.begin(); itr != iListeners.end(); itr++)
    {
        itr->second.onEvent(e);
    }
}

void Resource::sendEvent(Event* e , bool destroy)
{
    if ( e )
    {
        GreResourceAutolock ;
        
        // Send this Event to every Listeners we have.
        
        for ( auto it = iListeners.begin() ; it != iListeners.end() ; it++ )
        {
            // As '::onEvent()' use only the reference function '::sendEvent()' , we don't have to pay attention
            // on deleting or not the event.
            
            it->second.onEvent( *e );
        }
        
        // If we have to , also delete this event.
        
        if ( destroy )
        {
            delete e ;
        }
    }
}

void Resource::setShouldTransmitEvents(bool p)
{
    GreResourceAutolock ;
    iShouldTransmit = p;
}

bool Resource::shouldTransmitEvents() const
{
    return iShouldTransmit;
}

void Resource::acquire()
{
    GreResourceAutolock ;
    
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
    // Notes ( 26.09.2016 ) : We can't use GreResourceAutolock in this function , because it would cause the
    // 'iMutex' property to be destroyed two times when 'deleting this;' is called.
    // As 'Resource::~Resource()' use GreResourceAutolock , we don't need to check for Mutex locking in this
    // function ( 'delete this' ).
    
    lockGuard();
    
    if(iCounterInitialized)
    {
        iCounter->unhold();
        
        if(iCounter->getHolderCount() == 0)
        {
            unlockGuard();
            
            delete this;
            return;
        }
    }
    
    unlockGuard();
}

int Resource::getCounterValue() const
{
    GreResourceAutolock ;
    return iCounter->getHolderCount();
}

ReferenceCounter* Resource::getReferenceCounter()
{
    GreResourceAutolock ;
    return iCounter;
}

Variant& Resource::getCustomData(const std::string &entry)
{
    GreResourceAutolock ;
    return iCustomData[entry];
}

const Variant& Resource::getCustomData(const std::string &entry) const
{
    GreResourceAutolock ;
    return iCustomData.at(entry);
}

void Resource::setCustomData(const std::string &entry, const Gre::Variant &data)
{
    GreResourceAutolock ;
    iCustomData[entry] = data;
}

const void* Resource::getProperty(const std::string &name) const
{
    GreResourceAutolock ;
    
    if ( name == "iName" )
    {
        return &iName;
    }
    
    if ( name == "iType" )
    {
        return &iType;
    }
    
    if ( name == "iActions" )
    {
        return &iActions;
    }
    
    return nullptr;
}

void Resource::lockGuard() const
{
    iMutex.lock();
}

void Resource::unlockGuard() const
{
    iMutex.unlock();
}

void Resource::clearListeners()
{
    iListeners.clear();
}

void Resource::clear()
{
    iName.clear();
    iType = Resource::Type::Null;
    iActions.clear();
    iNextCallbacks.clear();
    iListeners.clear();
    iShouldTransmit = true;
    iCustomData.clear();
}

// ---------------------------------------------------------------------------------------------------

ResourceAutolock::ResourceAutolock ( std::recursive_mutex& mutex )
: iMutexReference( &mutex )
{
    iMutexReference->lock();
}

ResourceAutolock::~ResourceAutolock()
{
    iMutexReference->unlock();
}

GreEndNamespace
