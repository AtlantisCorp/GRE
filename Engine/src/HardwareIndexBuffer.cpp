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

HardwareIndexBuffer::HardwareIndexBuffer(const std::string& name)
: Gre::HardwareBuffer(name)
, iElementsCount(0)
, iElementsSize(0)
, iDataChanged(false)
{
    MaterialUser defaultMat = ResourceManager::Get().getMaterialManager()->get("Default");
    
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
        GreDebugPretty() << "Can't load Material 'Default' from MaterialManager." << Gre::gendl;
#endif
        throw GreConstructorException("HardwareIndexBuffer", "Resource 'Materials/Default' not found.");
    }
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    
}

void HardwareIndexBuffer::addData(const char *data, size_t sz)
{
    addDataToIndexBatch(data, sz, 0);
}

void HardwareIndexBuffer::setIndexDescriptor(const Gre::IndexDescriptor &desc, size_t index)
{
    if ( index < iBatches.size() )
    {
        IndexBatch& batch = iBatches.at(index);
        batch.setDescriptor(desc);
    }
}

void HardwareIndexBuffer::setData(const HardwareIndexBufferHolder &holder)
{
    clearBatches();
    
    if ( !holder.isInvalid() )
    {
        // Add every Batches to this Hardware Buffer.
        
        for ( const IndexBatch& batch : holder->getIndexBatches() )
        {
            addIndexBatch(batch);
        }
    }
}

void HardwareIndexBuffer::addIndexBatch(const Gre::IndexBatch &batch)
{
    iBatches.push_back(batch);
    iElementsSize += batch.getSize();
    iElementsCount += batch.getSize() / IndexTypeGetSize(batch.getDescriptor().getType());
    iDataChanged = true;
}

void HardwareIndexBuffer::addDataToIndexBatch(const char *data, size_t sz, size_t index)
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
            GreDebugPretty() << "Invalid index given ('" << index << "')." << Gre::gendl;
        }
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Invalid arguments given." << Gre::gendl;
    }
#endif
}

const char* HardwareIndexBuffer::getData() const
{
    const IndexBatch& batch = iBatches.at(0);
    return batch.getData();
}

const IndexBatchVector& HardwareIndexBuffer::getIndexBatches() const
{
    return iBatches;
}

const IndexBatch& HardwareIndexBuffer::getIndexBatch(const size_t &index) const
{
    if ( index < iBatches.size() )
    {
        return iBatches.at(index);
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Invalid index given ('" << index << "')." << Gre::gendl;
#endif
    
    throw GreIndexException("HardwareIndexBuffer", index, iBatches.size());
}

const IndexBatch& HardwareIndexBuffer::getDefaultBatch() const
{
    return iBatches.at(0);
}

void HardwareIndexBuffer::removeIndexBatch(const size_t &index)
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
        GreDebugPretty() << "Invalid index given ('" << index << "')." << Gre::gendl;
    }
#endif
}

void HardwareIndexBuffer::clearBatches()
{
    iElementsCount = 0;
    iElementsSize = 0;
    iBatches.clear();
    iDataChanged = true;
}

void HardwareIndexBuffer::clearData()
{
    clearBatches();
}

size_t HardwareIndexBuffer::getSize() const
{
    return iElementsSize;
}

size_t HardwareIndexBuffer::count() const
{
    return iElementsCount;
}

GreEndNamespace


