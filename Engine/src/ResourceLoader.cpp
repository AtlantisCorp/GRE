////////////////////////////////////////////////////
//  File    : ResourceLoader.cpp
//  Project : GRE
//
//  Created by Jacques Tronconi on 07/05/2016.
//  
//
////////////////////////////////////////////////////

#include "ResourceLoader.h"

GreBeginNamespace

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
