//////////////////////////////////////////////////////////////////////
//
//  HardwareVertexBuffer.cpp
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

#include "HardwareVertexBuffer.h"

GreBeginNamespace

HardwareVertexBufferPrivate::HardwareVertexBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name)
{
    
}

HardwareVertexBufferPrivate::~HardwareVertexBufferPrivate()
{
    
}

const VertexDescriptor& HardwareVertexBufferPrivate::getVertexDescriptor() const
{
    return iComponents;
}

void HardwareVertexBufferPrivate::setVertexDescriptor(const Gre::VertexDescriptor &vdesc)
{
    iComponents = vdesc;
}

// ---------------------------------------------------------------------------------------------------

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBufferPrivate* pointer)
: ResourceUser(pointer)
, HardwareBuffer(pointer)
, SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(pointer)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBufferHolder& holder)
: ResourceUser(holder)
, HardwareBuffer(holder.get())
, SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(holder)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBuffer& user)
: ResourceUser(user)
, HardwareBuffer(user)
, SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(user)
{
    
}

HardwareVertexBuffer::~HardwareVertexBuffer()
{
    
}

HardwareVertexBufferHolder HardwareVertexBuffer::lock()
{
    return SpecializedResourceUser<HardwareVertexBufferPrivate>::lock();
}

const HardwareVertexBufferHolder HardwareVertexBuffer::lock() const
{
    return SpecializedResourceUser<HardwareVertexBufferPrivate>::lock();
}

const VertexDescriptor& HardwareVertexBuffer::getVertexDescriptor() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getVertexDescriptor();
    return VertexDescriptor::Default;
}

void HardwareVertexBuffer::setVertexDescriptor(const Gre::VertexDescriptor &vdesc)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setVertexDescriptor(vdesc);
}

HardwareVertexBuffer HardwareVertexBuffer::Null = HardwareVertexBuffer(nullptr);

GreEndNamespace
