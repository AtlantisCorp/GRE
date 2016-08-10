//////////////////////////////////////////////////////////////////////
//
//  HardwarePixelBuffer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 02/07/2016.
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

#ifndef HardwarePixelBuffer_h
#define HardwarePixelBuffer_h

#include "Pools.h"
#include "HardwareBuffer.h"
#include "HardwarePixel.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Buffer Object used to store Pixel data.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwarePixelBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwarePixelBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwarePixelFormat.
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelFormat(const HardwarePixelFormat& pixformat);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwarePixelFormat used by this buffer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwarePixelFormat getPixelFormat() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should return a pointer to the data stored by the
    /// HardwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual char* getData();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should return a pointer to the data stored by the
    /// HardwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
    
protected:
    
    /// @brief Holds the HardwarePixelFormat used to store data.
    HardwarePixelFormat iPixFormat;
};

/// @brief SpecializedResourceHolder for HardwarePixelBufferPrivate.
typedef SpecializedResourceHolder<HardwarePixelBufferPrivate> HardwarePixelBufferHolder;

/// @brief SpecializedResourceHolderList for HardwarePixelBufferPrivate.
typedef SpecializedResourceHolderList<HardwarePixelBufferPrivate> HardwarePixelBufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for HardwarePixelBufferPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwarePixelBuffer : public HardwareBuffer, public SpecializedResourceUser<HardwarePixelBufferPrivate>
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelBuffer(const HardwarePixelBufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelBuffer(const HardwarePixelBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelBuffer(const HardwarePixelBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwarePixelBuffer() noexcept(false) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    HardwarePixelBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const HardwarePixelBufferHolder lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwarePixelFormat.
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelFormat(const HardwarePixelFormat& pixformat);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwarePixelFormat used by this buffer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwarePixelFormat getPixelFormat() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should return a pointer to the data stored by the
    /// HardwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual char* getData();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should return a pointer to the data stored by the
    /// HardwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
};

GreEndNamespace

#endif /* HardwarePixelBuffer_h */
