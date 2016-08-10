//////////////////////////////////////////////////////////////////////
//
//  SoftwareIndexBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 16/07/2016.
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

#include "SoftwareIndexBuffer.h"

GreBeginNamespace

SoftwareIndexBufferPrivate::SoftwareIndexBufferPrivate(const std::string& name)
: Gre::HardwareIndexBufferPrivate(name), iIndexes()
{
    IndexBatch idefbatch;
    idefbatch.setDescriptor(IndexDescriptor::Default);
    iIndexes.push_back(idefbatch);
}

SoftwareIndexBufferPrivate::~SoftwareIndexBufferPrivate() noexcept(false)
{
    
}

const IndexBatchVector& SoftwareIndexBufferPrivate::getIndexBatchVector() const
{
    return iIndexes;
}

void SoftwareIndexBufferPrivate::setIndexBatchVector(const IndexBatchVector &ivector)
{
    iIndexes = ivector;
}

void SoftwareIndexBufferPrivate::addIndexBatch(const Gre::IndexBatch &ibatch)
{
    iIndexes.push_back(ibatch);
}

void SoftwareIndexBufferPrivate::addData(const char *vdata, size_t sz)
{
    auto idefbatch = iIndexes.at(0);
    idefbatch.addData(vdata, sz);
}

const char* SoftwareIndexBufferPrivate::getData() const
{
    return iIndexes.at(0).getData();
}

void SoftwareIndexBufferPrivate::clearData()
{
    for ( int i = 0; i < iIndexes.size(); ++i )
    {
        iIndexes.at(i).clear();
    }
}

size_t SoftwareIndexBufferPrivate::getSize() const
{
    size_t totsz = 0;
    
    for ( int i = 0; i < iIndexes.size(); ++i )
    {
        totsz += iIndexes[i].getSize();
    }
    
    return totsz;
}

size_t SoftwareIndexBufferPrivate::count() const
{
    size_t totcount = 0;
    
    for ( int i = 0; i < iIndexes.size(); ++i )
    {
        totcount += iIndexes[i].getSize() / IndexTypeGetSize(iIndexes[i].getDescriptor().getType());
    }
    
    return totcount;
}

// ---------------------------------------------------------------------------------------------------

SoftwareIndexBuffer::SoftwareIndexBuffer(const SoftwareIndexBufferPrivate* pointer)
: Gre::ResourceUser(pointer),
  Gre::HardwareIndexBuffer(pointer),
  SpecializedResourceUser<Gre::SoftwareIndexBufferPrivate>(pointer)
{
    
}

SoftwareIndexBuffer::SoftwareIndexBuffer(const SoftwareIndexBufferHolder& holder)
: Gre::ResourceUser(holder),
  Gre::HardwareIndexBuffer(holder.get()),
  SpecializedResourceUser<Gre::SoftwareIndexBufferPrivate>(holder)
{
    
}

SoftwareIndexBuffer::SoftwareIndexBuffer(const SoftwareIndexBuffer& user)
: Gre::ResourceUser(user),
  Gre::HardwareIndexBuffer(user),
  SpecializedResourceUser<Gre::SoftwareIndexBufferPrivate>(user)
{
    
}

SoftwareIndexBuffer::~SoftwareIndexBuffer()
{
    
}

SoftwareIndexBufferHolder SoftwareIndexBuffer::lock()
{
    return SpecializedResourceUser<SoftwareIndexBufferPrivate>::lock();
}

const SoftwareIndexBufferHolder SoftwareIndexBuffer::lock() const
{
    return SpecializedResourceUser<SoftwareIndexBufferPrivate>::lock();
}

const IndexBatchVector SoftwareIndexBuffer::getIndexBatchVector() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getIndexBatchVector();
    return IndexBatchVector();
}

void SoftwareIndexBuffer::setIndexBatchVector(const IndexBatchVector &ivector)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setIndexBatchVector(ivector);
}

SoftwareIndexBuffer SoftwareIndexBuffer::Null = SoftwareIndexBuffer(nullptr);

GreEndNamespace
