//////////////////////////////////////////////////////////////////////
//
//  SoftwarePixelBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 02/07/2016.
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

#include "SoftwarePixelBuffer.h"

GreBeginNamespace

SoftwarePixelBufferPrivate::SoftwarePixelBufferPrivate(const std::string& name)
: HardwarePixelBufferPrivate(name), iPixBuffer(nullptr)
{
    
}

SoftwarePixelBufferPrivate::~SoftwarePixelBufferPrivate()
{
    if ( iPixBuffer )
    {
        free(iPixBuffer);
    }
}

void SoftwarePixelBufferPrivate::setPixelFormat(const Gre::HardwarePixelFormat &pixformat)
{
    iPixFormat = pixformat;
    // No need to set the 'dirty' flag to true, as every data is stored here.
}

char* SoftwarePixelBufferPrivate::getData()
{
    return iPixBuffer;
}

const char* SoftwarePixelBufferPrivate::getData() const
{
    return iPixBuffer;
}

size_t SoftwarePixelBufferPrivate::getSize() const
{
    return iSize;
}

size_t SoftwarePixelBufferPrivate::count() const
{
    HardwarePixelFormatDescriptor desc = HardwarePixelFormatDescriptorManager::Instance.getDescriptor(iPixFormat);
    if( desc.iSize != 0 )
    {
        return iSize / desc.iSize;
    }
    
    return 0;
}

void SoftwarePixelBufferPrivate::setData(const char *data, size_t sz)
{
    if( iPixBuffer )
    {
        free(iPixBuffer);
    }
    
    iSize = sz;
    iPixBuffer = (char*) malloc(sz);
    
    // We must check for data validity , because one can create an empty buffer.
    
    if ( data )
    {
        memcpy(iPixBuffer, data, sz);
    }
}

// ---------------------------------------------------------------------------------------------------

SoftwarePixelBuffer::SoftwarePixelBuffer(const SoftwarePixelBufferPrivate* pointer)
: ResourceUser(pointer)
, HardwarePixelBuffer(pointer)
, SpecializedResourceUser<SoftwarePixelBufferPrivate>(pointer)
{
    
}

SoftwarePixelBuffer::SoftwarePixelBuffer(const SoftwarePixelBufferHolder& holder)
: ResourceUser(holder)
, HardwarePixelBuffer(holder.get())
, SpecializedResourceUser<SoftwarePixelBufferPrivate>(holder)
{
    
}

SoftwarePixelBuffer::SoftwarePixelBuffer(const SoftwarePixelBuffer& user)
: ResourceUser(user)
, HardwarePixelBuffer(user)
, SpecializedResourceUser<SoftwarePixelBufferPrivate>(user)
{
    
}

SoftwarePixelBuffer::~SoftwarePixelBuffer()
{
    
}

SoftwarePixelBufferHolder SoftwarePixelBuffer::lock()
{
    return SpecializedResourceUser<SoftwarePixelBufferPrivate>::lock();
}

const SoftwarePixelBufferHolder SoftwarePixelBuffer::lock() const
{
    return SpecializedResourceUser<SoftwarePixelBufferPrivate>::lock();
}

void SoftwarePixelBuffer::setPixelFormat(const Gre::HardwarePixelFormat &pixformat)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setPixelFormat(pixformat);
}

char* SoftwarePixelBuffer::getData()
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getData();
    return nullptr;
}

const char* SoftwarePixelBuffer::getData() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getData();
    return nullptr;
}

size_t SoftwarePixelBuffer::getSize() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getSize();
    return 0;
}

size_t SoftwarePixelBuffer::count() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->count();
    return 0;
}

void SoftwarePixelBuffer::setData(const char *data, size_t sz)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setData(data, sz);
}

SoftwarePixelBuffer SoftwarePixelBuffer::Null = SoftwarePixelBuffer(nullptr);

GreEndNamespace
