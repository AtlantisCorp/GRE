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
/// @brief Defines a HardwareBuffer used to hold Indexes (faces).
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareIndexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferPrivate(const std::string& name, PrimitiveType ptype, StorageType stype = StorageType::UnsignedInt);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareIndexBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add an index (a groupe of indice, or a face) to this buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const IndexedFace& index);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add a batch of face to this buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const IndexedFaceBatch& batch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the material used to render this buffer.
    //////////////////////////////////////////////////////////////////////
    void setMaterial(const Material& material);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Material used.
    //////////////////////////////////////////////////////////////////////
    Material& getMaterial();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Material used.
    //////////////////////////////////////////////////////////////////////
    const Material& getMaterial() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Primitive type.
    //////////////////////////////////////////////////////////////////////
    PrimitiveType getPrimitiveType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Storage Type.
    //////////////////////////////////////////////////////////////////////
    StorageType getStorageType() const;
    
protected:
    
    MaterialIndexedFace iFaces;        ///< @brief The indexes, for one material.
    PrimitiveType       iPrimitiveType;///< @brief Primitive Type to be drawn.
    StorageType         iStorageType;  ///< @brief Storage used to store the indices.
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
    
    POOLED(Pools::HwdBuffer)
    
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
    ~HardwareIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const HardwareIndexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add an index (a groupe of indice, or a face) to this buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const IndexedFace& index);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add a batch of face to this buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const IndexedFaceBatch& batch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the material used to render this buffer.
    //////////////////////////////////////////////////////////////////////
    void setMaterial(const Material& material);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Material used.
    //////////////////////////////////////////////////////////////////////
    Material& getMaterial();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Material used.
    //////////////////////////////////////////////////////////////////////
    const Material& getMaterial() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Primitive type.
    //////////////////////////////////////////////////////////////////////
    PrimitiveType getPrimitiveType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Storage Type.
    //////////////////////////////////////////////////////////////////////
    StorageType getStorageType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements.
    //////////////////////////////////////////////////////////////////////
    size_t getElementCount() const;
    
    /// @brief A Null HardwareIndexBuffer.
    static HardwareIndexBuffer Null;
};

GreEndNamespace

#endif
