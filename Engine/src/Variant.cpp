//
//  Variant.cpp
//  GRE
//
//  Created by Jacques Tronconi on 25/01/2016.
//
//

#include "Variant.h"

GRE_BEGIN_NAMESPACE

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

Variant::~Variant()
{
    clear();
}

void* Variant::CopyFromPolicy(Variant::Policy policy, void *object)
{
    if(policy == Policy::Integer)
    {
        int* tmp = (int*) object;
        return new int(*tmp);
    }
    
    return nullptr;
}

void Variant::DeleteFromPolicy(Variant::Policy policy, void *object)
{
    if(policy == Policy::Integer)
    {
        int* tmp = (int*) object;
        delete tmp;
    }
}

void Variant::reset(Variant::Policy policy, void *object)
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
        return * ( (int*) _mObject );
    }
    
    throw VariantBadCast("Trying to get Integer object from none Integer real object.");
}

bool Variant::isNull() const
{
    return _mPolicy == Policy::None || _mObject == nullptr;
}

Variant Variant::Null = Variant();

GRE_END_NAMESPACE
