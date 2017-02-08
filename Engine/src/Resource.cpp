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

GreBeginNamespace

ResourceIdentifier ResourceIdentifier::Current = ResourceIdentifier ( 0 ) ;

ResourceIdentifier ResourceIdentifier::New()
{
    ResourceIdentifier retvalue ( ResourceIdentifier::Current ) ;
    ResourceIdentifier::Current ++ ;
    return retvalue ;
}

ResourceIdentifier ResourceIdentifier::Last()
{
    unsigned long int value = ResourceIdentifier::Current.iValue ;
    
    if ( value == 0 )
        return value ;
    
    return ResourceIdentifier ( value - 1 ) ;
}

ResourceIdentifier::ResourceIdentifier ( unsigned long int value )
: iValue(value)
{
    
}

ResourceIdentifier::ResourceIdentifier ( const ResourceIdentifier & rhs )
: iValue(rhs.iValue)
{
    
}

ResourceIdentifier::operator unsigned long()
{
    return iValue ;
}

ResourceIdentifier::operator const unsigned long() const
{
    return iValue ;
}

ResourceIdentifier & ResourceIdentifier::operator++()
{
    iValue ++ ;
    return *this ;
}

ResourceIdentifier ResourceIdentifier::operator++(int)
{
    ResourceIdentifier tmp ( *this ) ;
    operator++();
    return tmp ;
}

ResourceIdentifier & ResourceIdentifier::operator--()
{
    iValue -- ;
    return *this ;
}

ResourceIdentifier ResourceIdentifier::operator--(int)
{
    ResourceIdentifier tmp ( *this ) ;
    operator--();
    return tmp ;
}

// ---------------------------------------------------------------------------------------------------

Resource::Resource ()
: Gre::EventProceeder()
, iIdentifier(ResourceIdentifier::New())
, iName("Default") , iProperties() , iLoadStatus(true)
{
    
}

Resource::Resource ( const std::string & name )
: Gre::EventProceeder ()
, iIdentifier(ResourceIdentifier::New())
, iName(name) , iProperties() , iLoadStatus(true)
{
    
}

Resource::Resource ( const ResourceIdentifier & identifier , const std::string & name )
: Gre::EventProceeder()
, iIdentifier(identifier)
, iName(name) , iProperties() , iLoadStatus(true)
{
    
}

Resource::~Resource() noexcept ( false )
{
    
}

const ResourceIdentifier & Resource::getIdentifier() const
{
    GreAutolock ;
    return iIdentifier ;
}

const std::string & Resource::getName() const
{
    GreAutolock ;
    return iName ;
}

void Resource::setName(const std::string &name)
{
    GreAutolock ;
    iName = name ;
}

const ResourceProperty & Resource::getProperty(const std::string &name) const
{
    GreAutolock ;
    
    for ( const ResourceProperty & property : iProperties )
    {
        if ( property.name == name )
            return property ;
    }
    
    return Resource::BadProperty ;
}

const void * Resource::getPropertyData(const std::string &name) const
{
    GreAutolock ;
    
    for ( const ResourceProperty & property : iProperties )
    {
        if ( property.name == name )
            return property.data ;
    }
    
    return nullptr ;
}

bool Resource::isLoaded() const
{
    GreAutolock ;
    return iLoadStatus ;
}

void Resource::unload()
{
    GreAutolock ;
    
    // Always send the 'ResourceUnloadedEvent' before destroying the Resource, thus the EventProceeder
    // listeners can access for the last time their objects in relation with this one.
    
    EventHolder e = EventHolder ( new ResourceUnloadedEvent(this) ) ;
    sendEvent(e) ;
    
    iIdentifier = ResourceIdentifier::New();
    iName = "Default" ;
    iProperties.clear() ;
    iLoadStatus = false ;
    
    EventProceeder::clear() ;
}

ResourceProperty Resource::BadProperty = { "BadProperty" , "" , nullptr } ;

// ---------------------------------------------------------------------------------------------------

IndependentResource::IndependentResource ()
: Gre::Resource()
{
    
}

IndependentResource::IndependentResource ( const std::string & name )
: Gre::Resource ( ResourceIdentifier::New() , name )
{
    
}

IndependentResource::IndependentResource ( const ResourceIdentifier & identifier , const std::string & name )
: Gre::Resource(identifier , name)
{
    
}

IndependentResource::~IndependentResource() noexcept ( false )
{
    
}

bool IndependentResource::reloadResource ( const std::string & arguments )
{
    unload(); return loadResource(arguments);
}

GreEndNamespace
