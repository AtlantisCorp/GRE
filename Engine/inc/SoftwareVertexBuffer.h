//////////////////////////////////////////////////////////////////////
//
//  SoftwareVertexBuffer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/07/2016.
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

#ifndef SoftwareVertexBuffer_h
#define SoftwareVertexBuffer_h

#include "Pools.h"
#include "HardwareVertexBuffer.h"
#include "BoundingBox.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Vertex Buffer holded in the Cpu's RAM.
///
/// To create Vertex Data, here is an example :
///   - Create your struct Vertex { .. }; with your Components (see
/// VertexComponentType for available Components).
///   - Create a VertexDescriptor describing how your Vertex is constitued.
///   - Add your Vertex data to the SoftwareBuffer using ::addData().
///   - Set the Descriptor using ::setVertexDescriptor().
///   - Use Mesh::setSoftwareVertexBuffer() to set your buffer.
///
/// If you want to disable a field, use the VertexComponentType::Null's
/// types.
///
/// The SoftwareVertexBuffer has a 'iBoundingBox' field, which is computed
/// when modifying the data. If you want to modify Vertex Data by mapping
/// a pointer, use '::makeBoundingBox()' to compute the BoundingBox with new
/// data.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareVertexBufferPrivate : public HardwareVertexBufferPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareVertexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareVertexBufferPrivate();
    
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
    /// @brief Returns the BoundingBox computed for this Software Buffer.
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox& getBoundingBox() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Computes the BoundingBox from this Software Buffer's data.
    /// Uses this function if you modify the HardwareBuffer from a pointer,
    /// after setting new data.
    //////////////////////////////////////////////////////////////////////
    virtual void makeBoundingBox();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets data from another HardwareVertexBuffer object, erasing
    /// prior data and updating new one.
    //////////////////////////////////////////////////////////////////////
    virtual void setData ( const HardwareVertexBufferHolder& holder );
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    ///
    /// Updates the 'iBoundingBox' property if 'iVertexData' has changed,
    /// calls 'makeBoundingBox()'.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
    
protected:
    
    /// @brief Holds the raw Vertex data.
    char* iVertexData;
    
    /// @brief Size of the buffer.
    size_t iSize;
    
    /// @brief BoundingBox computed for this VertexBuffer.
    BoundingBox iBoundingBox;
    
    /// @brief True if the BoundingBox must be computed.
    mutable bool iBoundingBoxInvalid;
};

/// @brief SpecializedResourceHolder for SoftwareVertexBufferPrivate.
typedef SpecializedResourceHolder<SoftwareVertexBufferPrivate> SoftwareVertexBufferHolder;

/// @brief SpecializedResourceHolderList for SoftwareVertexBufferPrivate.
typedef SpecializedResourceHolderList<SoftwareVertexBufferPrivate> SoftwareVertexBufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for SoftwareVertexBufferPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareVertexBuffer : public HardwareVertexBuffer
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareVertexBuffer(const SoftwareVertexBufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareVertexBuffer(const SoftwareVertexBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareVertexBuffer(const SoftwareVertexBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    SoftwareVertexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const SoftwareVertexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the BoundingBox computed for this Software Buffer.
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox& getBoundingBox() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Computes the BoundingBox from this Software Buffer's data.
    /// Uses this function if you modify the HardwareBuffer from a pointer,
    /// after setting new data.
    //////////////////////////////////////////////////////////////////////
    virtual void makeBoundingBox();
    
    /// @brief Null SoftwareVertexBuffer.
    static SoftwareVertexBuffer Null;
};

GreEndNamespace

#endif /* SoftwareVertexBuffer_h */
