//////////////////////////////////////////////////////////////////////
//
//  ResourceHolder.cpp
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

#include "ResourceHolder.h"

GreBeginNamespace

#ifdef GreExtraResourceHolder
bool CheckHolderMagic ( Resource* pointer )
{
    return *( reinterpret_cast<int*>(pointer) - 1 ) == 0xBAD007;
}
#endif

ResourceHolder::ResourceHolder(Resource* resource)
{
    iResource = resource;
    iCounter = nullptr;
    
//  This constructor is very bad because, at many times, one could use a null 'resource' object
//  to initialize this one with a bad adress. If 'GreExtraResourceHolder' is defined, we make a
//  simple check for the 'iHolderMagic' property in Resource. This property is added by the POOL
//  functions.
    
#ifdef GreExtraResourceHolder
    if ( !CheckHolderMagic(resource) )
    {
        iResource = nullptr;
        iCounter = nullptr;
    }
#endif
    
    if( iResource != nullptr )
    {
        iResource->acquire();
        iCounter = iResource->getReferenceCounter();
    }
}

ResourceHolder::ResourceHolder(Resource* resource, ReferenceCounter* counter)
{
    iResource = resource;
    iCounter = counter;
    
//  Prevents acquiring a Resource with invalid pointer. This function is used, for example, by
//  'ResourceUser' when locking although it's null.
    
#ifdef GreExtraResourceHolder
    if ( !CheckHolderMagic(resource) )
    {
        iResource = nullptr;
        iCounter = nullptr;
    }
#endif
    
    if( iCounter != nullptr )
    {
        if( iCounter->getHolderCount() <= 0 )
        {
            iResource = nullptr;
            iCounter = nullptr;
        }
    }
    
    if( iResource )
    {
        iResource->acquire();
        iCounter = iResource->getReferenceCounter();
    }
}

ResourceHolder::ResourceHolder(const ResourceHolder& holder)
{
    iResource = holder.iResource;
    iCounter = holder.iCounter;
    
//  Prevents acquiring a Resource with invalid pointer. This function is used, for example, by
//  'ResourceUser' when locking although it's null.
    
#ifdef GreExtraResourceHolder
    if ( !CheckHolderMagic(holder.iResource) )
    {
        iResource = nullptr;
        iCounter = nullptr;
    }
#endif
    
    if( iCounter != nullptr )
    {
        if( iCounter->getHolderCount() == 0 )
        {
            iResource = nullptr;
            iCounter = nullptr;
        }
    }
    
    if( iCounter )
    {
        iResource->acquire();
        iCounter = iResource->getReferenceCounter();
    }
}

ResourceHolder::~ResourceHolder()
{
    if(iResource)
    {
        iResource->release();
        iResource = nullptr;
        iCounter = nullptr;
    }
}

Resource* ResourceHolder::get()
{
    return iResource;
}

const Resource* ResourceHolder::get() const
{
    return iResource;
}

bool ResourceHolder::isNull() const
{
    return iResource == nullptr;
}

bool ResourceHolder::isInvalid() const
{
    return iResource == nullptr;
}

ResourceHolder::operator bool() const
{
    return iResource != nullptr;
}

Resource* ResourceHolder::operator->()
{
    return iResource;
}

const Resource* ResourceHolder::operator->() const
{
    return iResource;
}

void ResourceHolder::reset()
{
	if(iResource)
    {
        iResource->release();
        iResource = nullptr;
        iCounter = nullptr;
    }
}

ResourceHolder& ResourceHolder::operator= (const ResourceHolder& rhs)
{
    reset();
    iResource = rhs.iResource;
    iCounter = rhs.iCounter;
    
    if ( iResource && iCounter )
    {
        iCounter->hold();
    }
    
    return *this;
}

GreEndNamespace
