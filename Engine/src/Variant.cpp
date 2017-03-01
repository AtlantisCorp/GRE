//////////////////////////////////////////////////////////////////////
//
//  Variant.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 25/01/2016.
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

Variant::Variant(float object)
: _mPolicy(Variant::Policy::Float), _mObject(new float(object))
{
    
}

Variant::Variant(const Variant& rhs)
: _mPolicy(Variant::Policy::None), _mObject(nullptr)
{
    if(rhs._mPolicy != Policy::None)
    {
        _mObject = CopyFromPolicy(rhs._mPolicy, rhs._mObject);
		_mPolicy = rhs._mPolicy ;
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

Variant::Variant ( const std::string & obj )
: _mPolicy(Variant::Policy::String), _mObject(new std::string(obj))
{
    
}

Variant::Variant ( const Color& color )
: _mPolicy(Variant::Policy::Color), _mObject(new Color(color))
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
    
    if ( policy == Policy::Float )
    {
        const float* tmp = (const float*) object ;
        return new float(*tmp);
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
    
    if ( policy == Policy::String )
    {
        const std::string* tmp = (const std::string*) object;
        return new std::string(*tmp);
    }
    
    if ( policy == Policy::Color )
    {
        const Color * tmp = (const Color*) object ;
        return new Color (*tmp) ;
    }
    
    return nullptr;
}

void Variant::DeleteFromPolicy(Variant::Policy policy, void *object)
{
    if(policy == Policy::Integer)
    {
        int* tmp = (int*) object;
        //delete tmp;
        return;
    }
    
    if ( policy == Policy::Float )
    {
        float* tmp = (float*) object ;
        delete tmp ; return ;
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
    
    if ( policy == Policy::String )
    {
        std::string* tmp = (std::string*) object;
        delete tmp;
        return ;
    }
    
    if ( policy == Policy::Color )
    {
        Color* tmp = (Color*) object ;
        delete tmp ; return ;
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
void Variant::reset(float object)
{
    reset(Policy::Float, &object);
}

void Variant::reset(const Version& object)
{
    reset(Policy::Version, &object);
}

void Variant::reset(bool object)
{
    reset(Policy::Boolean, &object);
}

void Variant::reset(const std::string &object)
{
    reset(Policy::String, &object);
}

void Variant::reset(const Color &color)
{
    reset(Policy::Color, &color);
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

float& Variant::toFloat()
{
    if ( _mPolicy == Policy::Float )
        return * ( (float*) _mObject ) ;
    throw VariantBadCast("Trying to get Float object from none Float real object.");
}

const float& Variant::toFloat() const
{
    if ( _mPolicy == Policy::Float )
        return * ( (float*) _mObject ) ;
    throw VariantBadCast("Trying to get Float object from none Float real object.");
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

std::string& Variant::toString()
{
    if ( _mPolicy == Policy::String )
    {
        return * ( (std::string*) _mObject ) ;
    }
    
	GreDebug("[WARN] Invalid Variant conversion : 'std::string' instead of '") << (int) _mPolicy << "'." << gendl ; 
    throw VariantBadCast("Trying to get String object from none String real object.");
}

const std::string& Variant::toString() const
{
    if ( _mPolicy == Policy::String )
    {
        return * ( (const std::string*) _mObject ) ;
    }
    
	GreDebug("[WARN] Invalid Variant conversion : 'std::string' instead of '") << (int) _mPolicy << "'." << gendl ;
    throw VariantBadCast("Trying to get String object from none String real object.");
}

Color& Variant::toColor()
{
    if ( _mPolicy == Policy::Color )
    {
        return * ( (Color*) _mObject ) ;
    }
    
    throw VariantBadCast("Trying to get Color object from none Color real object.");
}

const Color& Variant::toColor() const 
{
    if ( _mPolicy == Policy::Color )
    {
        return * ( (Color*) _mObject ) ;
    }
    
    throw VariantBadCast("Trying to get Color object from none Color real object.");
}

bool Variant::isNull() const
{
    return _mPolicy == Policy::None || _mObject == nullptr;
}

Variant Variant::Null = Variant();

GreEndNamespace
