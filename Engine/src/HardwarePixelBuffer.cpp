//////////////////////////////////////////////////////////////////////
//
//  HardwarePixelBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/07/2016.
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

#include "HardwarePixelBuffer.h"

GreBeginNamespace

HardwarePixelBufferPrivate::HardwarePixelBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name), iPixFormat(HardwarePixelFormat::Null)
{
    
}

HardwarePixelBufferPrivate::~HardwarePixelBufferPrivate()
{
    
}

void HardwarePixelBufferPrivate::setPixelFormat(const Gre::HardwarePixelFormat &pixformat)
{
    iPixFormat = pixformat;
    setDirty(true);
}

HardwarePixelFormat HardwarePixelBufferPrivate::getPixelFormat() const
{
    return iPixFormat;
}

void* HardwarePixelBufferPrivate::getData()
{
    return nullptr;
}

const void* HardwarePixelBufferPrivate::getData() const
{
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------

HardwarePixelBuffer::HardwarePixelBuffer(const HardwarePixelBufferPrivate* pointer)
: Gre::HardwareBuffer(pointer), SpecializedResourceUser<Gre::HardwarePixelBufferPrivate>(pointer)
{
    
}

HardwarePixelBuffer::HardwarePixelBuffer(const HardwarePixelBufferHolder& holder)
: Gre::HardwareBuffer(holder.get()), SpecializedResourceUser<Gre::HardwarePixelBufferPrivate>(holder)
{
    
}

HardwarePixelBuffer::HardwarePixelBuffer(const HardwarePixelBuffer& user)
: Gre::HardwareBuffer(user), SpecializedResourceUser<Gre::HardwarePixelBufferPrivate>(user)
{
    
}

HardwarePixelBuffer::~HardwarePixelBuffer() noexcept(false)
{
    
}

HardwarePixelBufferHolder HardwarePixelBuffer::lock()
{
    return SpecializedResourceUser<HardwarePixelBufferPrivate>::lock();
}

const HardwarePixelBufferHolder HardwarePixelBuffer::lock() const
{
    return SpecializedResourceUser<HardwarePixelBufferPrivate>::lock();
}

void HardwarePixelBuffer::setPixelFormat(const Gre::HardwarePixelFormat &pixformat)
{
    auto ptr = lock();
    if( ptr )
        ptr->setPixelFormat(pixformat);
}

HardwarePixelFormat HardwarePixelBuffer::getPixelFormat() const
{
    auto ptr = lock();
    if( ptr )
        return ptr->getPixelFormat();
    return HardwarePixelFormat::Null;
}

void* HardwarePixelBuffer::getData()
{
    auto ptr = lock();
    if( ptr )
        return ptr->getData();
    return nullptr;
}

const void* HardwarePixelBuffer::getData() const
{
    auto ptr = lock();
    if( ptr )
        return ptr->getData();
    return nullptr;
}

GreEndNamespace
