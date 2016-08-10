//////////////////////////////////////////////////////////////////////
//
//  HardwareIndexBuffer.cpp
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

#include "HardwareIndexBuffer.h"

GreBeginNamespace

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name)
{
    
}

HardwareIndexBufferPrivate::~HardwareIndexBufferPrivate()
{
    
}

void HardwareIndexBufferPrivate::addIndexBatch(const IndexBatch& ibatch)
{

}

const IndexDescriptor& HardwareIndexBufferPrivate::getDefaultDescriptor() const
{
    return iDescriptor;
}

void HardwareIndexBufferPrivate::setDefaultDescriptor(const IndexDescriptor& desc)
{
    iDescriptor = desc;
}

// ---------------------------------------------------------------------------------------------------

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferPrivate* resource)
: ResourceUser(resource)
, Gre::HardwareBuffer(resource)
, SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(resource)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferHolder& holder)
: ResourceUser(holder)
, Gre::HardwareBuffer(holder.get())
, SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(holder)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBuffer& user)
: ResourceUser(user)
, Gre::HardwareBuffer(user)
, SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(user)
{
    
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    
}

HardwareIndexBufferHolder HardwareIndexBuffer::lock()
{
    return SpecializedResourceUser<HardwareIndexBufferPrivate>::lock();
}

const HardwareIndexBufferHolder HardwareIndexBuffer::lock() const
{
    return SpecializedResourceUser<HardwareIndexBufferPrivate>::lock();
}

void HardwareIndexBuffer::addIndexBatch(const IndexBatch& ibatch)
{
    auto ptr = lock();
    if ( ptr )
        ptr->addIndexBatch(ibatch);
}

const IndexDescriptor& HardwareIndexBuffer::getDefaultDescriptor() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getDefaultDescriptor();
    return IndexDescriptor::Default;
}

void HardwareIndexBuffer::setDefaultDescriptor(const IndexDescriptor& desc)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setDefaultDescriptor(desc);
}

HardwareIndexBuffer HardwareIndexBuffer::Null = HardwareIndexBuffer(nullptr);

GreEndNamespace


