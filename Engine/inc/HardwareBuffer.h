//////////////////////////////////////////////////////////////////////
//
//  HardwareBuffer.h
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

#ifndef GRE_HardwareBuffer_h
#define GRE_HardwareBuffer_h

#include "Pools.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines the real Hardware Buffer interface.
/// A Hardware Buffer is a collection of data stored generally by the
/// renderer, to render objects more quickly in giving the subjacing
/// API this raw buffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareBuffer : public Resource
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareBuffer (const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareBuffer ();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Hardware Buffer in order to use it.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the Hardware Buffer after it has been used.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Update the buffer if dirty.
    /// This function should permit the HardwareBuffer to upload unuploaded
    /// data to the GPU, or whatever.
    //////////////////////////////////////////////////////////////////////
    virtual void update();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Buffer is invalid.
    //////////////////////////////////////////////////////////////////////
    virtual bool isDataInvalid() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds some data to this Buffer. Size is the total size in
    /// bytes, not the number of Vertex.
    //////////////////////////////////////////////////////////////////////
    virtual void addData(const char* vdata, size_t sz);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the data in this SoftwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this buffer needs to be updated.
    //////////////////////////////////////////////////////////////////////
    virtual bool isDirty() const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Used by the children classes, set this property to true
    /// if the buffer needs update.
    //////////////////////////////////////////////////////////////////////
    virtual void setDirty(bool dirty) const;
    
protected:
    
    /// @brief True if this HardwareBuffer needs a call to 'update()'.
    mutable bool iIsDirty;
};

/// @brief SpecializedCountedObjectHolder for HardwareBufferPrivate.
typedef SpecializedCountedObjectHolder<HardwareBuffer> HardwareBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareBufferPrivate
typedef SpecializedResourceHolderList<HardwareBuffer> HardwareBufferHolderList;

/// @brief SpecializedCountedObjectUser for HardwareBuffer.
typedef SpecializedCountedObjectUser<HardwareBuffer> HardwareBufferUser;

GreEndNamespace

#endif
