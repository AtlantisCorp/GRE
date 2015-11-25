//
//  Resource.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Resource.h"

GRE_BEGIN_NAMESPACE

Resource::Resource (const std::string& name) :
_name (name)
{
#ifdef DEBUG
    std::cout << "[Resource:" << name << "] Constructed." << std::endl;
#endif
}

Resource::~Resource ()
{
#ifdef DEBUG
    std::cout << "[Resource:" << _name << "] Destroyed." << std::endl;
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

ResourceUser ResourceUser::Null = ResourceUser(std::weak_ptr<Resource>());

ResourceUser::ResourceUser (std::weak_ptr<Resource> r) :
_resource ( std::move(r) )
{
#ifdef DEBUG
//    std::cout << "[ResourceUser:" << ( _resource.lock() ? _resource.lock()->getName() : "null" ) << "] Constructed." << std::endl;
#endif
}

ResourceUser::~ResourceUser ()
{
#ifdef DEBUG
//    std::cout << "[ResourceUser:" << ( _resource.lock() ? _resource.lock()->getName() : "null" ) << "] Destroyed." << std::endl;
#endif
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

GRE_END_NAMESPACE