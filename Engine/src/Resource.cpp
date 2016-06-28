////////////////////////////////////////////////////
//  File    : Resource.cpp
//  Project : GRE
//
//  Created by Jacques Tronconi on 06/05/2016.
//  
//
////////////////////////////////////////////////////

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
    
    if(shouldTransmitEvents())
    {
        sendEvent(e);
    }
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
