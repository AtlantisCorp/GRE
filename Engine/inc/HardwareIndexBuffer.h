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
/// Storage is dependent of the Implementation. You can submit new Indexes
/// by using ::addData() or ::addIndexBatch(). The iDescriptor property
/// should be used to set the default property for the Indexes.
///
/// The Renderer can use this buffer to draw indexed meshes.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareIndexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareIndexBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new IndexBatch in the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addIndexBatch(const IndexBatch& ibatch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default IndexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexDescriptor& getDefaultDescriptor() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the default IndexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual void setDefaultDescriptor(const IndexDescriptor& desc);
    
protected:
    
    /// @brief Default IndexDescritor for the buffer.
    IndexDescriptor iDescriptor;
};

/// @brief SpecializedResourceHolder for HardwareIndexBufferPrivate.
typedef SpecializedResourceHolder<HardwareIndexBufferPrivate> HardwareIndexBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareIndexBufferPrivate.
typedef SpecializedResourceHolderList<HardwareIndexBufferPrivate> HardwareIndexBufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Proxy to the HardwareIndexBufferPrivate object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareIndexBuffer : public HardwareBuffer, public SpecializedResourceUser<HardwareIndexBufferPrivate>
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBuffer(const HardwareIndexBufferPrivate* resource);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBuffer(const HardwareIndexBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBuffer(const HardwareIndexBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const HardwareIndexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new IndexBatch in the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addIndexBatch(const IndexBatch& ibatch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default IndexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const IndexDescriptor& getDefaultDescriptor() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the default IndexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual void setDefaultDescriptor(const IndexDescriptor& desc);
    
    /// @brief A Null HardwareIndexBuffer.
    static HardwareIndexBuffer Null;
};

GreEndNamespace

#endif
