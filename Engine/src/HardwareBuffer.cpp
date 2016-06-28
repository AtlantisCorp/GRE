//////////////////////////////////////////////////////////////////////
//
//  HardwareBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/11/2015.
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

#include "HardwareBuffer.h"

GreBeginNamespace

HardwareBufferPrivate::HardwareBufferPrivate(const std::string& name)
: Resource(name)
{
    
}

HardwareBufferPrivate::~HardwareBufferPrivate()
{
    
}

size_t HardwareBufferPrivate::getSize() const
{
    GreDebugFunctionNotImplemented();
    return 0;
}

size_t HardwareBufferPrivate::count() const
{
    GreDebugFunctionNotImplemented();
    return 0;
}

void HardwareBufferPrivate::bind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareBufferPrivate::unbind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareBufferPrivate::update() const
{
    GreDebugFunctionNotImplemented();
}

bool HardwareBufferPrivate::isDirty() const
{
    return iIsDirty;
}

void HardwareBufferPrivate::setDirty(bool dirty) const
{
    iIsDirty = dirty;
}

bool HardwareBufferPrivate::isInvalid() const
{
    GreDebugFunctionNotImplemented();
    return true;
}

// ---------------------------------------------------------------------------------------------------

HardwareBuffer::HardwareBuffer(const HardwareBufferPrivate* resource)
: SpecializedResourceUser<Gre::HardwareBufferPrivate>(resource)
{
    
}

HardwareBuffer::HardwareBuffer(const HardwareBufferHolder& holder)
: SpecializedResourceUser<Gre::HardwareBufferPrivate>(holder)
{
    
}

HardwareBuffer::HardwareBuffer(const HardwareBuffer& user)
: SpecializedResourceUser<Gre::HardwareBufferPrivate>(user)
{
    
}

HardwareBuffer::~HardwareBuffer()
{
    
}

void HardwareBuffer::bind() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->bind();
}

void HardwareBuffer::unbind() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->unbind();
}

void HardwareBuffer::update() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->update();
}

size_t HardwareBuffer::getSize() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSize();
    return 0;
}

size_t HardwareBuffer::count() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->count();
    return 0;
}

bool HardwareBuffer::isDirty() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isDirty();
    return false;
}

GreEndNamespace
