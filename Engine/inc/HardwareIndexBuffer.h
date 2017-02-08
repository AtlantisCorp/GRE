//////////////////////////////////////////////////////////////////////
//
//  HardwareIndexBuffer.h
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

#ifndef GRE_HardwareIndexBuffer_h
#define GRE_HardwareIndexBuffer_h

#include "HardwareBuffer.h"
#include "Face.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a HardwareBuffer used to hold Indexes.
///
/// As Indexes are stored by Materials, you have ways to add Indexes
/// to this HardwareBuffer.
///   - ::addData() adds raw indexes to the IndexBatch '0' which has
/// Material 'Materials/Default'.
///   - ::addIndexBatch() adds an IndexBatch which should contain a
/// custom Material.
///   - ::addDataToIndexBatch() adds raw indexes to the given IndexBatch.
///
/// You can't get a pointer to all the data, considering each IndexBatch
/// has a Material different and may have data of different type. To get
/// the Batches, use ::getIndexBatches(), ::getIndexBatch() or ::getDefaultBatch().
/// ::getData() returns the Indexes in the IndexBatch '0'.
///
/// To remove an IndexBatch, you can use ::removeIndexBatch() or
/// ::clearBatches().
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareIndexBuffer : public HardwareBuffer
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    /// @brief SpecializedCountedObjectHolder for HardwareIndexBufferPrivate.
    typedef SpecializedCountedObjectHolder<HardwareIndexBuffer> HardwareIndexBufferHolder;
    
    /// @brief SpecializedResourceHolderList for HardwareIndexBufferPrivate.
    typedef SpecializedResourceHolderList<HardwareIndexBuffer> HardwareIndexBufferHolderList;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBuffer(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds raw Indexes to the default IndexBatch.
    //////////////////////////////////////////////////////////////////////
    virtual void addData ( const char* data , size_t sz );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the IndexDescriptor for given IndexBatch.
    //////////////////////////////////////////////////////////////////////
    virtual void setIndexDescriptor ( const IndexDescriptor& desc , size_t index );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Copies data from given HardwareIndexBuffer to this one.
    //////////////////////////////////////////////////////////////////////
    virtual void setData ( const HardwareIndexBufferHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds an IndexBatch to the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addIndexBatch ( const IndexBatch& batch );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds raw Indexes to the given IndexBatch.
    ///
    /// @param data : Raw indexes to add.
    /// @param sz : Size of the indexes.
    /// @param index : Index of the IndexBatch in the HardwareIndexBuffer.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void addDataToIndexBatch ( const char* data , size_t sz , size_t index );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Indexes from IndexBatch '0'.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every IndexBatch.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexBatchVector& getIndexBatches () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the IndexBatch at given index.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexBatch& getIndexBatch ( const size_t& index ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns IndexBatch '0'.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexBatch& getDefaultBatch () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes IndexBatch at given index.
    //////////////////////////////////////////////////////////////////////
    virtual void removeIndexBatch ( const size_t& index );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every IndexBatch.
    //////////////////////////////////////////////////////////////////////
    virtual void clearBatches();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the data in the buffer.
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
    
    /// @brief Incremented Counter to count elements.
    uint32_t iElementsCount;
    
    /// @brief Incremented Counter to get overall size. (in bytes)
    uint32_t iElementsSize;
    
    /// @brief IndexBatch's Vector.
    IndexBatchVector iBatches;
    
    /// @brief True if data has changed since last update.
    mutable bool iDataChanged;
};

/// @brief SpecializedCountedObjectHolder for HardwareIndexBufferPrivate.
typedef SpecializedCountedObjectHolder<HardwareIndexBuffer> HardwareIndexBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareIndexBufferPrivate.
typedef SpecializedResourceHolderList<HardwareIndexBuffer> HardwareIndexBufferHolderList;

/// @brief SpecializedCountedObjectUser for HardwareIndexBuffer.
typedef SpecializedCountedObjectUser<HardwareIndexBuffer> HardwareIndexBufferUser;

GreEndNamespace

#endif
