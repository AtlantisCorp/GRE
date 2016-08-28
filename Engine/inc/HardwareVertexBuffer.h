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

#include "Pools.h"
#include "HardwareBuffer.h"
#include "VertexDescriptor.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a HardwareBuffer used to hold Vertexs.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareVertexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    /// @brief SpecializedResourceHolder for HardwareVertexBufferPrivate.
    typedef SpecializedResourceHolder<HardwareVertexBufferPrivate> HardwareVertexBufferHolder;
    
    /// @brief SpecializedResourceHolderList for HardwareVertexBufferPrivate.
    typedef SpecializedResourceHolderList<HardwareVertexBufferPrivate> HardwareVertexBufferHolderList;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareVertexBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const VertexDescriptor& getVertexDescriptor() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexDescriptor(const VertexDescriptor& vdesc);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets data from another HardwareVertexBuffer object, erasing
    /// prior data and updating new one.
    //////////////////////////////////////////////////////////////////////
    virtual void setData ( const HardwareVertexBufferHolder& holder ) = 0;
    
protected:
    
    /// @brief Vertex's Descriptor map.
    VertexDescriptor iComponents;
};

/// @brief SpecializedResourceHolder for HardwareVertexBufferPrivate.
typedef SpecializedResourceHolder<HardwareVertexBufferPrivate> HardwareVertexBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareVertexBufferPrivate.
typedef SpecializedResourceHolderList<HardwareVertexBufferPrivate> HardwareVertexBufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Proxy to the HardwareVertexBufferPrivate object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareVertexBuffer : public HardwareBuffer
{
public:
    
    POOLED(Pools::HdwBuffer)
    
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
    virtual ~HardwareVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const VertexDescriptor& getVertexDescriptor() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexDescriptor(const VertexDescriptor& vdesc);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets data from another HardwareVertexBuffer object, erasing
    /// prior data and updating new one.
    //////////////////////////////////////////////////////////////////////
    virtual void setData ( const HardwareVertexBufferHolder& holder );
    
    /// @brief A Null HardwareVertexBuffer.
    static HardwareVertexBuffer Null;
};

GreEndNamespace

#endif
