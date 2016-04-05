//
//  Resource.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Resource.h"
#include "ResourceManager.h"

GreBeginNamespace

Resource::Resource (const std::string& name) :
_name (name), _type(Resource::Type::Null)
{
    VariantDescriptor nulldescriptor;
    nulldescriptor.variant = Variant();
    _mVariantDatas[-1] = nulldescriptor;
    _shouldTransmit = false;
    
#ifdef GreIsDebugMode
    GreDebugPretty() << name << " : Constructed." << std::endl;
#endif
}

Resource::~Resource ()
{
#ifdef GreIsDebugMode
    GreDebugPretty() << _name << " : Destroyed." << std::endl;
#endif
}

const std::string& Resource::getName() const
{
    return _name;
}

const void* Resource::_getData() const
{
    return nullptr;
}

const void* Resource::getCustomData(const std::string &dataname) const
{
    return nullptr;
}

void Resource::storeVariantData(int index, const Variant &data)
{
    if(index >= 0)
        _mVariantDatas[index].variant = data;
}

Variant& Resource::getVariantData(int index)
{
    if(index >= 0)
        return _mVariantDatas[index].variant;
    else
        return _mVariantDatas[-1].variant;
}

const Variant& Resource::getVariantData(int index) const
{
    if(index >= 0)
        return _mVariantDatas.at(index).variant;
    else
        return _mVariantDatas.at(-1).variant;
}

Resource::Type Resource::getType() const
{
    return _type;
}

void Resource::addAction(EventType etype, std::function<void (const Event&)> eaction)
{
    _actions[etype].push_back(eaction);
}

void Resource::resetActions()
{
    _actions.clear();
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
    
    auto actions = _actions[e.getType()];
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
    _mListeners.insert(std::pair<std::string, ResourceUser>(listener.getName(), listener));
    return _mListeners.at(listener.getName());
}

ResourceUser Resource::getListener(const std::string &name)
{
    return _mListeners.at(name);
}

void Resource::removeListener(const std::string &name)
{
    auto it = _mListeners.find(name);
    if(it != _mListeners.end())
    {
        _mListeners.erase(it);
    }
}

void Resource::sendEvent(const Event &e)
{
    for(auto itr = _mListeners.begin(); itr != _mListeners.end(); itr++)
    {
        itr->second.onEvent(e);
    }
}

void Resource::setShouldTransmitEvents(bool p)
{
    _shouldTransmit = p;
}

bool Resource::shouldTransmitEvents() const
{
    return _shouldTransmit;
}

// ---------------------------------------------------------------------------------------------------

ResourceUser ResourceUser::Null = ResourceUser(std::weak_ptr<Resource>());

ResourceUser::ResourceUser (bool persistent)
: _resource(), _isPersistent(persistent)
{

}

ResourceUser::ResourceUser (ResourceUser&& movref)
: _resource(std::move(movref._resource)), _isPersistent(movref._isPersistent)
{
    if(isPersistent())
    {
        _persistentResource = _resource.lock();
        _resource = _persistentResource;
    }
}


ResourceUser::ResourceUser (const ResourceUser& rhs)
: _resource(rhs._resource), _isPersistent(rhs._isPersistent)
{
    if(isPersistent())
    {
        _persistentResource = std::dynamic_pointer_cast<Resource>(_resource.lock());
        _resource = _persistentResource;
    }
}

ResourceUser::ResourceUser (std::weak_ptr<Resource> r, bool persistent) :
_resource ( r ), _isPersistent(persistent)
{
    if(isPersistent())
    {
        _persistentResource = std::dynamic_pointer_cast<Resource>(_resource.lock());
        _resource = _persistentResource;
    }
}

ResourceUser::~ResourceUser ()
{
    
}

bool ResourceUser::expired () const
{
    return _resource.expired();
}

const std::string& ResourceUser::getName() const
{
    auto ptr = _resource.lock();
    return ptr->getName();
}


std::shared_ptr<Resource> ResourceUser::lock()
{
    return _resource.lock();
}
const std::shared_ptr<Resource> ResourceUser::lock() const
{
    return _resource.lock();
}

const void* ResourceUser::getCustomData(const std::string &dataname) const
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->getCustomData(dataname);
    return nullptr;
}

void ResourceUser::storeVariantData(int index, const Gre::Variant &data)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->storeVariantData(index, data);
}

Variant& ResourceUser::getVariantData(int index)
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->getVariantData(index);
    return Variant::Null;
}

const Variant& ResourceUser::getVariantData(int index) const
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->getVariantData(index);
    return Variant::Null;
}

void ResourceUser::addAction(EventType etype, std::function<void (const Event&)> eaction)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->addAction(etype, eaction);
}

void ResourceUser::resetActions()
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->resetActions();
}

void ResourceUser::onEvent(const Event &e)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->onEvent(e);
}

void ResourceUser::listen(ResourceUser &emitter)
{
    emitter.addListener(*this);
}

ResourceUser& ResourceUser::addListener(const std::string &name)
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->addListener(name);
    return ResourceUser::Null;
}

ResourceUser& ResourceUser::addListener(const ResourceUser &listener)
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->addListener(listener);
    return ResourceUser::Null;
}

ResourceUser ResourceUser::getListener(const std::string &name)
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->getListener(name);
    return ResourceUser::Null;
}

void ResourceUser::removeListener(const std::string &name)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->removeListener(name);
}

void ResourceUser::sendEvent(const Event &e)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->sendEvent(e);
}

void ResourceUser::setShouldTransmitEvents(bool p)
{
    auto ptr = _resource.lock();
    if(ptr)
        ptr->setShouldTransmitEvents(p);
}

bool ResourceUser::shouldTransmitEvents() const
{
    auto ptr = _resource.lock();
    if(ptr)
        return ptr->shouldTransmitEvents();
    return false;
}

bool ResourceUser::isPersistent() const
{
    return _isPersistent;
}

// ---------------------------------------------------------------------------------------------------

ResourceLoader::ResourceLoader()
{
}
ResourceLoader::ResourceLoader(const ResourceLoader&)
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::isTypeSupported (Resource::Type type) const
{
    return false;
}

ResourceLoader* ResourceLoader::clone() const
{
    return nullptr;
}

GreEndNamespace