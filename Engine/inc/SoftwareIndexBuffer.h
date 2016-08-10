//////////////////////////////////////////////////////////////////////
//
//  SoftwareIndexBuffer.h
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

#ifndef SoftwareIndexBuffer_h
#define SoftwareIndexBuffer_h

#include "HardwareIndexBuffer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Software Index Buffer 's resource.
///
/// By default, every indexes added with ::addData() is added in the
/// default IndexBatch , i.e. iIndexes[0] . The Default Index Batch
/// is initialized with IndexDescriptor::Default.
///
/// Using ::setIndexBatchVector() changes the Default Index Batch.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareIndexBufferPrivate : public HardwareIndexBufferPrivate
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareIndexBufferPrivate() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Vector of indexes.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexBatchVector& getIndexBatchVector() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the IndexBatchVector.
    //////////////////////////////////////////////////////////////////////
    virtual void setIndexBatchVector(const IndexBatchVector& ivector);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new IndexBatch in the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addIndexBatch(const IndexBatch& ibatch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds some data to this Buffer. Size is the total size in
    /// bytes, not the number of Vertex. Data is added to the default
    /// IndexBatch.
    //////////////////////////////////////////////////////////////////////
    virtual void addData(const char* vdata, size_t sz);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the data in the default index batch.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the data in the buffer.
    /// This function does not clear the Indexes's Descriptors, only the
    /// raw Indexes data are erased.
    //////////////////////////////////////////////////////////////////////
    virtual void clearData();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
protected:
    
    /// @brief Stores the indexes.
    IndexBatchVector iIndexes;
};

/// @brief SpecializedResourceHolder for SoftwareIndexBufferPrivate.
typedef SpecializedResourceHolder<SoftwareIndexBufferPrivate> SoftwareIndexBufferHolder;

/// @brief SpecializedResourceHolderList for SoftwareIndexBufferPrivate.
typedef SpecializedResourceHolderList<SoftwareIndexBufferPrivate> SoftwareIndexBufferHolderList;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareIndexBuffer : public HardwareIndexBuffer , public SpecializedResourceUser<SoftwareIndexBufferPrivate>
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const SoftwareIndexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Vector of indexes.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexBatchVector getIndexBatchVector() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the IndexBatchVector.
    //////////////////////////////////////////////////////////////////////
    virtual void setIndexBatchVector(const IndexBatchVector& ivector);
    
    /// @brief Null SoftwareIndexBuffer.
    static SoftwareIndexBuffer Null;
};

/// @brief std::list for SoftwareIndexBuffer.
typedef std::list<SoftwareIndexBuffer> SoftwareIndexBufferList;

GreEndNamespace

#endif /* SoftwareIndexBuffer_h */
