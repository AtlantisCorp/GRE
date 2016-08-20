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
#include "ResourceManager.h"

GreBeginNamespace

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name)
, iElementsCount(0)
, iElementsSize(0)
, iDataChanged(false)
{
    Material defaultMat = ResourceManager::Get().findResource<Material>("Materials/Default");
    
    if ( !defaultMat.isInvalid() )
    {
        IndexDescriptor defaultDescriptor;
        defaultDescriptor.setType(IndexType::UnsignedShort);
        defaultDescriptor.setMaterial(defaultMat);
        
        IndexBatch defaultBatch;
        defaultBatch.setDescriptor(defaultDescriptor);
        addIndexBatch(defaultBatch);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't load Material 'Default' from MaterialManager." << std::endl;
#endif
        throw GreConstructorException("HardwareIndexBufferPrivate", "Resource 'Materials/Default' not found.");
    }
}

HardwareIndexBufferPrivate::~HardwareIndexBufferPrivate()
{
    
}

void HardwareIndexBufferPrivate::addData(const char *data, size_t sz)
{
    addDataToIndexBatch(data, sz, 0);
}

void HardwareIndexBufferPrivate::addIndexBatch(const Gre::IndexBatch &batch)
{
    iBatches.push_back(batch);
    iElementsSize += batch.getSize();
    iElementsCount += batch.getSize() / IndexTypeGetSize(batch.getDescriptor().getType());
    iDataChanged = true;
}

void HardwareIndexBufferPrivate::addDataToIndexBatch(const char *data, size_t sz, size_t index)
{
    if ( sz && data )
    {
        if ( index < iBatches.size() )
        {
            IndexBatch& batch = iBatches.at(index);
            batch.addData(data, sz);
            iElementsSize += sz;
            iElementsCount += sz / IndexTypeGetSize(batch.getDescriptor().getType());
            iDataChanged = true;
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "Invalid index given ('" << index << "')." << std::endl;
        }
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Invalid arguments given." << std::endl;
    }
#endif
}

const char* HardwareIndexBufferPrivate::getData() const
{
    const IndexBatch& batch = iBatches.at(0);
    return batch.getData();
}

const IndexBatchVector& HardwareIndexBufferPrivate::getIndexBatches() const
{
    return iBatches;
}

const IndexBatch& HardwareIndexBufferPrivate::getIndexBatch(const size_t &index) const
{
    if ( index < iBatches.size() )
    {
        return iBatches.at(index);
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Invalid index given ('" << index << "')." << std::endl;
#endif
    
    throw GreIndexException("HardwareIndexBufferPrivate", index, iBatches.size());
}

const IndexBatch& HardwareIndexBufferPrivate::getDefaultBatch() const
{
    return iBatches.at(0);
}

void HardwareIndexBufferPrivate::removeIndexBatch(const size_t &index)
{
    if ( index < iBatches.size() )
    {
        IndexBatch& batch = iBatches.at(index);
        iElementsSize -= batch.getSize();
        iElementsCount -= batch.getSize() / IndexTypeGetSize( batch.getDescriptor().getType() );
        iBatches.erase(iBatches.begin() + index);
        iDataChanged = true;
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Invalid index given ('" << index << "')." << std::endl;
    }
#endif
}

void HardwareIndexBufferPrivate::clearBatches()
{
    iElementsCount = 0;
    iElementsSize = 0;
    iBatches.clear();
    iDataChanged = true;
}

void HardwareIndexBufferPrivate::clearData()
{
    clearBatches();
}

size_t HardwareIndexBufferPrivate::getSize() const
{
    return iElementsSize;
}

size_t HardwareIndexBufferPrivate::count() const
{
    return iElementsCount;
}

// ---------------------------------------------------------------------------------------------------

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferPrivate* resource)
: ResourceUser(resource)
, Gre::HardwareBuffer(resource)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferHolder& holder)
: ResourceUser(holder)
, Gre::HardwareBuffer(holder.get())
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBuffer& user)
: ResourceUser(user)
, Gre::HardwareBuffer(user)
{
    
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    
}

HardwareIndexBufferHolder HardwareIndexBuffer::lock()
{
    return GreUserLockCast(HardwareIndexBufferHolder, HardwareIndexBufferPrivate, HardwareBuffer);
}

const HardwareIndexBufferHolder HardwareIndexBuffer::lock() const
{
    return GreUserConstLockCast(HardwareIndexBufferHolder, HardwareIndexBufferPrivate, HardwareBuffer);
}

void HardwareIndexBuffer::addIndexBatch(const Gre::IndexBatch &batch)
{
    auto ptr = lock();
    if ( ptr )
        ptr->addIndexBatch(batch);
    throw GreInvalidUserException("HardwareIndexBuffer");
}

void HardwareIndexBuffer::addDataToIndexBatch(const char *data, size_t sz, size_t index)
{
    auto ptr = lock();
    if ( ptr )
        ptr->addDataToIndexBatch(data, sz, index);
    throw GreInvalidUserException("HardwareIndexBuffer");
}

const IndexBatchVector& HardwareIndexBuffer::getIndexBatches() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getIndexBatches();
    throw GreInvalidUserException("HardwareIndexBuffer");
}

const IndexBatch& HardwareIndexBuffer::getIndexBatch(const size_t &index) const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getIndexBatch(index);
    throw GreInvalidUserException("HardwareIndexBuffer");
}

const IndexBatch& HardwareIndexBuffer::getDefaultBatch() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getDefaultBatch();
    throw GreInvalidUserException("HardwareIndexBuffer");
}

void HardwareIndexBuffer::removeIndexBatch(const size_t &index)
{
    auto ptr = lock();
    if ( ptr )
        ptr->removeIndexBatch(index);
    throw GreInvalidUserException("HardwareIndexBuffer");
}

void HardwareIndexBuffer::clearBatches()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearBatches();
    throw GreInvalidUserException("HardwareIndexBuffer");
}

HardwareIndexBuffer HardwareIndexBuffer::Null = HardwareIndexBuffer(nullptr);

GreEndNamespace


