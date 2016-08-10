//////////////////////////////////////////////////////////////////////
//
//  SoftwareVertexBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 11/07/2016.
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

#include "SoftwareVertexBuffer.h"

GreBeginNamespace

SoftwareVertexBufferPrivate::SoftwareVertexBufferPrivate(const std::string& name)
: Gre::HardwareVertexBufferPrivate(name), iVertexData(nullptr), iSize(0)
{
    
}

SoftwareVertexBufferPrivate::~SoftwareVertexBufferPrivate()
{
    clearData();
}

void SoftwareVertexBufferPrivate::bind() const
{
    
}

void SoftwareVertexBufferPrivate::unbind() const
{
    
}

void SoftwareVertexBufferPrivate::update() const
{
    
}

bool SoftwareVertexBufferPrivate::isDataInvalid() const
{
    return !( iVertexData && iSize );
}

void SoftwareVertexBufferPrivate::addData(const char *vdata, size_t sz)
{
    if ( iVertexData )
    {
        char* newdata = (char*) malloc(iSize + sz);
        memcpy(newdata, iVertexData, iSize);
        memcpy(newdata + iSize, vdata, sz);
        
        iSize = iSize + sz;
        iVertexData = newdata;
    }
    
    else
    {
        iVertexData = (char*) malloc(sz);
        memcpy(iVertexData, vdata, sz);
        
        iSize = sz;
    }
}

const char* SoftwareVertexBufferPrivate::getData() const
{
    return iVertexData;
}

void SoftwareVertexBufferPrivate::clearData()
{
    if ( iVertexData && iSize )
    {
        free(iVertexData);
        iVertexData = nullptr;
        iSize = 0;
    }
    
    else if ( iVertexData )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Freeing 'iVertexData' but 'iSize' is 0." << std::endl;
#endif
        free(iVertexData);
        iVertexData = nullptr;
        iSize = 0;
    }
}

size_t SoftwareVertexBufferPrivate::getSize() const
{
    return iSize;
}

size_t SoftwareVertexBufferPrivate::count() const
{
    if ( iVertexData && iSize )
    {
        return iSize / iComponents.getSize();
    }
    
    else
    {
        return 0;
    }
}

// ---------------------------------------------------------------------------------------------------

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBufferPrivate* pointer)
: ResourceUser(pointer)
, Gre::HardwareVertexBuffer(pointer)
, SpecializedResourceUser<Gre::SoftwareVertexBufferPrivate>(pointer)
{
    
}

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBufferHolder& holder)
: ResourceUser(holder)
, Gre::HardwareVertexBuffer(holder.get())
, SpecializedResourceUser<Gre::SoftwareVertexBufferPrivate>(holder)
{
    
}

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBuffer& user)
: ResourceUser(user)
, Gre::HardwareVertexBuffer(user)
, SpecializedResourceUser<Gre::SoftwareVertexBufferPrivate>(user)
{
    
}

SoftwareVertexBuffer::~SoftwareVertexBuffer()
{
    
}

SoftwareVertexBufferHolder SoftwareVertexBuffer::lock()
{
    return SpecializedResourceUser<SoftwareVertexBufferPrivate>::lock();
}

const SoftwareVertexBufferHolder SoftwareVertexBuffer::lock() const
{
    return SpecializedResourceUser<SoftwareVertexBufferPrivate>::lock();
}

SoftwareVertexBuffer SoftwareVertexBuffer::Null = SoftwareVertexBuffer(nullptr);

GreEndNamespace
