//
//  Variant.cpp
//  GRE
//
//  Created by Jacques Tronconi on 25/01/2016.
//
//

#include "Variant.h"

GreBeginNamespace

Variant::Variant(Variant::Policy policy, void* object)
: _mPolicy(policy), _mObject(object)
{
    
}

Variant::Variant(int object)
: _mPolicy(Variant::Policy::Integer), _mObject(new int(object))
{
    
}

Variant::Variant(const Variant& rhs)
: _mPolicy(Variant::Policy::None), _mObject(nullptr)
{
    if(rhs._mPolicy != Policy::None)
    {
        _mObject = CopyFromPolicy(rhs._mPolicy, rhs._mObject);
    }
}

Variant::Variant(const Version& object)
: _mPolicy(Variant::Policy::Version), _mObject(new Version(object))
{
    
}

Variant::Variant(bool object)
: _mPolicy(Variant::Policy::Boolean), _mObject(new bool(object))
{
    
}

Variant::~Variant()
{
    clear();
}

void* Variant::CopyFromPolicy(Variant::Policy policy, const void *object)
{
    if(policy == Policy::Integer)
    {
        const int* tmp = (const int*) object;
        return new int(*tmp);
    }
    
    if(policy == Policy::Version)
    {
        const Version* tmp = (const Version*) object;
        return new Version(*tmp);
    }
    
    if(policy == Policy::Boolean)
    {
        const bool* tmp = (const bool*) object;
        return new bool(*tmp);
    }
    
    return nullptr;
}

void Variant::DeleteFromPolicy(Variant::Policy policy, void *object)
{
    if(policy == Policy::Integer)
    {
        int* tmp = (int*) object;
        delete tmp;
        return;
    }
    
    if(policy == Policy::Version)
    {
        Version* tmp = (Version*) object;
        delete tmp;
        return;
    }
    
    if(policy == Policy::Boolean)
    {
        bool* tmp = (bool*) object;
        delete tmp;
        return;
    }
}

void Variant::reset(Variant::Policy policy, const void *object)
{
    if(_mObject)
    {
        DeleteFromPolicy(_mPolicy, _mObject);
    }
    
    _mObject = CopyFromPolicy(policy, object);
    _mPolicy = policy;
}

void Variant::reset(int object)
{
    reset(Policy::Integer, &object);
}

void Variant::reset(const Version& object)
{
    reset(Policy::Version, &object);
}

void Variant::reset(bool object)
{
    reset(Policy::Boolean, &object);
}

void Variant::clear()
{
    if(_mObject)
    {
        DeleteFromPolicy(_mPolicy, _mObject);
        _mObject = nullptr;
        _mPolicy = Policy::None;
    }
}

int& Variant::toInteger()
{
    if(_mPolicy == Policy::Integer)
    {
        return * ( (int*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Integer object from none Integer real object.");
}

const int& Variant::toInteger() const
{
    if(_mPolicy == Policy::Integer)
    {
        return * ( (const int*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Integer object from none Integer real object.");
}

Version& Variant::toVersion()
{
    if(_mPolicy == Policy::Version)
    {
        return * ( (Version*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Version object from none Version real object.");
}

const Version& Variant::toVersion() const
{
    if(_mPolicy == Policy::Version)
    {
        return * ( (const Version*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Version object from none Version real object.");
}

bool& Variant::toBool()
{
    if(_mPolicy == Policy::Boolean)
    {
        return * ( (bool*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Boolean object from none Boolean real object.");
}

const bool& Variant::toBool() const
{
    if(_mPolicy == Policy::Boolean)
    {
        return * ( (const bool*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Boolean object from none Boolean real object.");
}

bool Variant::isNull() const
{
    return _mPolicy == Policy::None || _mObject == nullptr;
}

Variant Variant::Null = Variant();

GreEndNamespace
