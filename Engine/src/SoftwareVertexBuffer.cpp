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
: HardwareVertexBufferPrivate(name)
, iVertexData(nullptr)
, iSize(0)
, iBoundingBoxInvalid(false)
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

void SoftwareVertexBufferPrivate::update()
{
    if ( iBoundingBoxInvalid )
    {
        makeBoundingBox();
    }
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
        iBoundingBoxInvalid = true;
    }
    
    else
    {
        iVertexData = (char*) malloc(sz);
        memcpy(iVertexData, vdata, sz);
        
        iSize = sz;
        iBoundingBoxInvalid = true;
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
        iBoundingBox.clear();
        iBoundingBoxInvalid = false;
    }
    
    else if ( iVertexData )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Freeing 'iVertexData' but 'iSize' is 0." << std::endl;
#endif
        free(iVertexData);
        iVertexData = nullptr;
        iSize = 0;
        iBoundingBox.clear();
        iBoundingBoxInvalid = false;
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

const BoundingBox& SoftwareVertexBufferPrivate::getBoundingBox() const
{
    return iBoundingBox;
}

void SoftwareVertexBufferPrivate::makeBoundingBox()
{
    if ( iBoundingBoxInvalid || iBoundingBox.isInvalid() )
    {
        const VertexDescriptor& desc = getVertexDescriptor();
        iBoundingBox.clear();
        
        if ( desc.getSize() && iVertexData && iSize )
        {
            int posloc = desc.getComponentLocation(VertexComponentType::Position);
            
            if ( posloc >= 0 )
            {
                // We have to iterates through each Vertex to get the position data.
                // 1. Get position component pointer.
                // 2. While another position is available, add the vertex stride to get
                // next Position.
                
                const char* end = iVertexData + iSize;
                const char* data = iVertexData + posloc;
                size_t stride = desc.getStride(VertexComponentType::Position);
                
                do
                {
                    const Vector3& vec = * ( (const Vector3*) data );
                    iBoundingBox.add(vec);
                    data = data + stride;
                    
                } while ( data < end );
            }
            
#ifdef GreIsDebugMode
            else
            {
                GreDebugPretty() << "No 'VertexComponentType::Position' in VertexDescriptor." << std::endl;
            }
#endif
            
        }
        
        iBoundingBoxInvalid = false;
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No need to update BoundingBox as 'iBoundingBoxInvalid' is false." << std::endl;
    }
#endif
}

void SoftwareVertexBufferPrivate::setData(const HardwareVertexBufferHolder &holder)
{
    clearData();
    
    if ( !holder.isInvalid() )
    {
        setVertexDescriptor(holder->getVertexDescriptor());
        addData(holder->getData(), holder->getSize());
    }
}

void SoftwareVertexBufferPrivate::onUpdateEvent(const Gre::UpdateEvent &e)
{
    HardwareVertexBufferPrivate::onUpdateEvent(e);
    
    if ( iBoundingBoxInvalid )
    {
        makeBoundingBox();
    }
}

// ---------------------------------------------------------------------------------------------------

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBufferPrivate* pointer)
: ResourceUser(pointer)
, HardwareVertexBuffer(pointer)
{
    
}

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBufferHolder& holder)
: ResourceUser(holder)
, HardwareVertexBuffer(holder.get())
{
    
}

SoftwareVertexBuffer::SoftwareVertexBuffer(const SoftwareVertexBuffer& user)
: ResourceUser(user)
, HardwareVertexBuffer(user)
{
    
}

SoftwareVertexBuffer::~SoftwareVertexBuffer()
{
    
}

SoftwareVertexBufferHolder SoftwareVertexBuffer::lock()
{
    return GreUserLockCast(SoftwareVertexBufferHolder, SoftwareVertexBufferPrivate, HardwareVertexBuffer);
}

const SoftwareVertexBufferHolder SoftwareVertexBuffer::lock() const
{
    return GreUserConstLockCast(SoftwareVertexBufferHolder, SoftwareVertexBufferPrivate, HardwareVertexBuffer);
}

const BoundingBox& SoftwareVertexBuffer::getBoundingBox() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getBoundingBox();
    throw GreInvalidUserException("SoftwareVertexBuffer");
}

void SoftwareVertexBuffer::makeBoundingBox()
{
    auto ptr = lock();
    if ( ptr )
        ptr->makeBoundingBox();
}

SoftwareVertexBuffer SoftwareVertexBuffer::Null = SoftwareVertexBuffer(nullptr);

GreEndNamespace
