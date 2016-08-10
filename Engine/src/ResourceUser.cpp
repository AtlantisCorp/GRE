//////////////////////////////////////////////////////////////////////
//
//  ResourceUser.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 02/05/2016.
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

#include "ResourceUser.h"

GreBeginNamespace

ResourceUser::ResourceUser(const Resource* resource)
{
    iResource = const_cast<Resource*>(resource);
    iCounter = nullptr;
    
    if(iResource)
    {
        iCounter = iResource->getReferenceCounter();
        iCounter->use();
    }
}

ResourceUser::ResourceUser(const ResourceHolder& holder)
{
    iResource = const_cast<Resource*>(holder.get());
    iCounter = nullptr;
    
    if(iResource)
    {
        iCounter = iResource->getReferenceCounter();
        iCounter->use();
    }
}

ResourceUser::ResourceUser(const ResourceUser& user)
{
    iResource = user.iResource;
    iCounter = nullptr;
    
    if(iResource)
    {
        iCounter = iResource->getReferenceCounter();
        iCounter->use();
    }
}

ResourceUser::~ResourceUser() noexcept(false)
{
    if(iCounter)
    {
        iCounter->unuse();
        
        if(iCounter->getUserCount() == 0)
        {
            delete iCounter;
            iCounter = nullptr;
        }
    }
}

ResourceHolder ResourceUser::lock()
{
    return ResourceHolder(iResource, iCounter);
}

const ResourceHolder ResourceUser::lock() const
{
    return ResourceHolder(iResource, iCounter);
}

bool ResourceUser::isInvalid() const
{
    if ( iCounter )
    {
        return iCounter->getHolderCount() == 0;
    }
    
    else
    {
        return true;
    }
}

bool ResourceUser::isExpired() const
{
    if(iCounter)
    {
        return iCounter->getHolderCount() == 0;
    }
    
    else
    {
        return true;
    }
}

ResourceUser::operator bool() const
{
	return isExpired();
}

void ResourceUser::reset()
{
    if(iCounter)
    {
        iCounter->unuse();
        
        if(iCounter->getUserCount() == 0)
        {
            delete iCounter;
            iCounter = nullptr;
        }
    }
    
    iResource = nullptr;
}

std::string ResourceUser::getName() const
{
    auto ptr = lock();
    if(ptr) {
        return ptr->getName();
    }
    return "";
}

Resource::Type ResourceUser::getType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getType();
    return Resource::Type::Null;
}

void ResourceUser::addAction(Gre::EventType etype, std::function<void (const Event &)> eaction)
{
    auto ptr = lock();
    if(ptr)
        ptr->addAction(etype, eaction);
}

void ResourceUser::resetActions()
{
    auto ptr = lock();
    if(ptr)
        ptr->resetActions();
}

void ResourceUser::onEvent(const Gre::Event &e)
{
    auto ptr = lock();
    if(ptr)
        ptr->onEvent(e);
}

ResourceUser& ResourceUser::addListener(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        return ptr->addListener(name);
    return ResourceUser::Null;
}

ResourceUser& ResourceUser::addListener(const ResourceUser &listener)
{
    auto ptr = lock();
    if(ptr)
        return ptr->addListener(listener);
    return ResourceUser::Null;
}

ResourceUser ResourceUser::getListener(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        return ptr->getListener(name);
    return ResourceUser::Null;
}

void ResourceUser::removeListener(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        ptr->removeListener(name);
}

void ResourceUser::sendEvent(const Event &e)
{
    auto ptr = lock();
    if(ptr)
        ptr->sendEvent(e);
}

void ResourceUser::setShouldTransmitEvents(bool p)
{
    auto ptr = lock();
    if(ptr)
        ptr->setShouldTransmitEvents(p);
}

bool ResourceUser::shouldTransmitEvents() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->shouldTransmitEvents();
    return false;
}

void ResourceUser::acquire()
{
    auto ptr = lock();
    if(ptr)
        ptr->acquire();
}

void ResourceUser::release()
{
    auto ptr = lock();
    if(ptr)
        ptr->release();
}

int ResourceUser::getCounterValue() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getCounterValue();
    return 0;
}

ReferenceCounter* ResourceUser::getReferenceCounter()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getReferenceCounter();
    return nullptr;
}

Variant& ResourceUser::getCustomData(const std::string &entry)
{
    auto ptr = lock();
    if(ptr)
        return ptr->getCustomData(entry);
    return Variant::Null;
}

const Variant& ResourceUser::getCustomData(const std::string &entry) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getCustomData(entry);
    return Variant::Null;
}

void ResourceUser::setCustomData(const std::string &entry, const Gre::Variant &data)
{
    auto ptr = lock();
    if(ptr)
        ptr->setCustomData(entry, data);
}

ResourceUser ResourceUser::Null = ResourceUser(nullptr);

GreEndNamespace
