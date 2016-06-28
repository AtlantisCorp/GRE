//////////////////////////////////////////////////////////////////////
//
//  HardwareVertexBuffer.h
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

#ifndef GRE_HardwareVertexBuffer_h
#define GRE_HardwareVertexBuffer_h

#include "HardwareBuffer.h"
#include "Vertex.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a HardwareBuffer used to hold Vertexs.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareVertexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareVertexBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add a Vertex to this buffer.
    /// If allowDuplicate is true, then no checking will be done
    /// about the unicity of the given vertex.
    //////////////////////////////////////////////////////////////////////
    void add(const Vertex& vertex);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Batch of Vertex to the buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const VertexBatch& batch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this buffer contains at least one copy of given
    /// vertex.
    //////////////////////////////////////////////////////////////////////
    bool contains(const Vertex& vertex) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set this property to true to allow duplicates.
    //////////////////////////////////////////////////////////////////////
    void setDuplicatesAllowed(bool allowed);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if duplicates are allowed.
    //////////////////////////////////////////////////////////////////////
    bool areDuplicatesAllowed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activates or de-activate the color buffer.
    //////////////////////////////////////////////////////////////////////
    void activateColor(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if color buffer is activated.
    //////////////////////////////////////////////////////////////////////
    bool isColorActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activates or Deactivates the Texture Coordinates in this
    /// Vertex Buffer.
    /// If Activated, Texture Coordinates will be sent to Shader under
    /// the location 'TexCoord'.
    //////////////////////////////////////////////////////////////////////
    void activateTexCoord(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Texture Coordinate has been activated.
    //////////////////////////////////////////////////////////////////////
    bool isTexCoordActivated() const;
    
protected:
    
    /// @brief The Vertexs holded by this buffer.
    std::vector<Vertex> iVertexs;
    
    /// @brief True if duplicates are allowed.
    bool iAllowDuplicates;
    
    /// @brief True if colors are enabled.
    bool iColorActivated;
    
    /// @brief True if Texture Coordinates are activated.
    bool iTexCoordActivated;
};

/// @brief SpecializedResourceHolder for HardwareVertexBufferPrivate.
typedef SpecializedResourceHolder<HardwareVertexBufferPrivate> HardwareVertexBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareVertexBufferPrivate.
typedef SpecializedResourceHolderList<HardwareVertexBufferPrivate> HardwareVertexBufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Proxy to the HardwareVertexBufferPrivate object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareVertexBuffer : public HardwareBuffer, public SpecializedResourceUser<HardwareVertexBufferPrivate>
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBuffer(const HardwareVertexBufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBuffer(const HardwareVertexBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBuffer(const HardwareVertexBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~HardwareVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a vertex to the buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const Vertex& vertex);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Batch of Vertex to the buffer.
    //////////////////////////////////////////////////////////////////////
    void add(const VertexBatch& batch);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this buffer contains at least one copy of given
    /// vertex.
    //////////////////////////////////////////////////////////////////////
    bool contains(const Vertex& vertex) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set this property to true to allow duplicates.
    //////////////////////////////////////////////////////////////////////
    void setDuplicatesAllowed(bool allowed);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if duplicates are allowed.
    //////////////////////////////////////////////////////////////////////
    bool areDuplicatesAllowed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activates or de-activate the color buffer.
    //////////////////////////////////////////////////////////////////////
    void activateColor(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if color buffer is activated.
    //////////////////////////////////////////////////////////////////////
    bool isColorActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activates or Deactivates the Texture Coordinates in this
    /// Vertex Buffer.
    /// If Activated, Texture Coordinates will be sent to Shader under
    /// the location 'TexCoord'.
    //////////////////////////////////////////////////////////////////////
    void activateTexCoord(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Texture Coordinate has been activated.
    //////////////////////////////////////////////////////////////////////
    bool isTexCoordActivated() const;
    
    /// @brief A Null HardwareVertexBuffer.
    static HardwareVertexBuffer Null;
};

GreEndNamespace

#endif
