//////////////////////////////////////////////////////////////////////
//
//  SoftwarePixelBuffer.h
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

#ifndef SoftwarePixelBuffer_h
#define SoftwarePixelBuffer_h

#include "Pools.h"
#include "HardwarePixelBuffer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A HardwarePixelBuffer but that can be stored only in the
/// CPU's RAM.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwarePixelBuffer : public HardwarePixelBuffer
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwarePixelBuffer(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwarePixelBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwarePixelFormat.
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelFormat(const HardwarePixelFormat& pixformat);
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the data buffer.
    //////////////////////////////////////////////////////////////////////
    virtual void setData(const char* data, size_t sz);
    
protected:
    
    /// @brief Holds the data, independently from its type.
    char* iPixBuffer;
    
    /// @brief Size of the data, in bytes.
    size_t iSize;
};

/// @brief SpecializedCountedObjectHolder for SoftwarePixelBufferPrivate.
typedef SpecializedCountedObjectHolder<SoftwarePixelBuffer> SoftwarePixelBufferHolder;

/// @brief SpecializedResourceHolderList for SoftwarePixelBufferPrivate.
typedef SpecializedResourceHolderList<SoftwarePixelBuffer> SoftwarePixelBufferHolderList;

/// @brief SpecializedCountedObjectUser for SoftwarePixelBuffer .
typedef SpecializedCountedObjectUser<SoftwarePixelBuffer> SoftwarePixelBufferUser ;

GreEndNamespace

#endif /* SoftwarePixelBuffer_h */
