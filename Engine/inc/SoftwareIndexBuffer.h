//////////////////////////////////////////////////////////////////////
//
//  SoftwareIndexBuffer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 16/07/2016.
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

#ifndef SoftwareIndexBuffer_h
#define SoftwareIndexBuffer_h

#include "HardwareIndexBuffer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Software Index Buffer 's resource.
///
/// As HardwareIndexBufferPrivate already stores every indexes, we just
/// need to ensure that 'iDataChanged' is set to false on every updates.
///
/// SoftwareIndexBuffer can be used to stores indexes only on the CPU
/// memory.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareIndexBufferPrivate : public HardwareIndexBufferPrivate
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareIndexBufferPrivate() noexcept(false);
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// In SoftwareIndexBuffer, this function is used to set 'iDataChanged'
    /// to false, as we don't need to update any data.
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
};

/// @brief SpecializedResourceHolder for SoftwareIndexBufferPrivate.
typedef SpecializedResourceHolder<SoftwareIndexBufferPrivate> SoftwareIndexBufferHolder;

/// @brief SpecializedResourceHolderList for SoftwareIndexBufferPrivate.
typedef SpecializedResourceHolderList<SoftwareIndexBufferPrivate> SoftwareIndexBufferHolderList;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwareIndexBuffer : public HardwareIndexBuffer
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBuffer(const SoftwareIndexBuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwareIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    SoftwareIndexBufferHolder lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const SoftwareIndexBufferHolder lock() const;
    
    /// @brief Null SoftwareIndexBuffer.
    static SoftwareIndexBuffer Null;
};

/// @brief std::list for SoftwareIndexBuffer.
typedef std::list<SoftwareIndexBuffer> SoftwareIndexBufferList;

GreEndNamespace

#endif /* SoftwareIndexBuffer_h */
