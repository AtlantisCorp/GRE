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
class DLL_PUBLIC HardwareVertexBuffer : public HardwareBuffer
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    /// @brief SpecializedCountedObjectHolder for HardwareVertexBufferPrivate.
    typedef SpecializedCountedObjectHolder<HardwareVertexBuffer> HardwareVertexBufferHolder;
    
    /// @brief SpecializedResourceHolderList for HardwareVertexBufferPrivate.
    typedef SpecializedResourceHolderList<HardwareVertexBuffer> HardwareVertexBufferHolderList;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBuffer(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const VertexDescriptor& getVertexDescriptor() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the VertexDescriptor.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexDescriptor(const VertexDescriptor& vdesc);
    
protected:
    
    /// @brief Vertex's Descriptor map.
    VertexDescriptor iComponents;
};

/// @brief SpecializedCountedObjectHolder for HardwareVertexBufferPrivate.
typedef SpecializedCountedObjectHolder<HardwareVertexBuffer> HardwareVertexBufferHolder;

/// @brief SpecializedResourceHolderList for HardwareVertexBufferPrivate.
typedef SpecializedResourceHolderList<HardwareVertexBuffer> HardwareVertexBufferHolderList;

/// @brief SpecializedCountedObjectUser.
typedef SpecializedCountedObjectUser<HardwareVertexBuffer> HardwareVertexBufferUser;

GreEndNamespace

#endif
