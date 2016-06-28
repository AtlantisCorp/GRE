//////////////////////////////////////////////////////////////////////
//
//  HardwareVertexBuffer.cpp
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

#include "HardwareVertexBuffer.h"

GreBeginNamespace

HardwareVertexBufferPrivate::HardwareVertexBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name)
{
    iAllowDuplicates = false;
    iColorActivated = false;
    iTexCoordActivated = false;
    iVertexs.reserve(100);
}

HardwareVertexBufferPrivate::~HardwareVertexBufferPrivate()
{
    
}

size_t HardwareVertexBufferPrivate::getSize() const
{
    return sizeof(Vertex) * count();
}

size_t HardwareVertexBufferPrivate::count() const
{
    return iVertexs.size();
}

void HardwareVertexBufferPrivate::add(const Vertex &vertex)
{
    if(areDuplicatesAllowed()) {
        iVertexs.push_back(vertex);
        setDirty(true);
    }
    else if(!contains(vertex)) {
        iVertexs.push_back(vertex);
        setDirty(true);
    }
}

void HardwareVertexBufferPrivate::add(const VertexBatch &batch)
{
    for(auto vertex : batch)
        add(vertex);
}

bool HardwareVertexBufferPrivate::contains(const Vertex& vertex) const
{
    return std::find(iVertexs.begin(), iVertexs.end(), vertex) != iVertexs.end();
}

void HardwareVertexBufferPrivate::setDuplicatesAllowed(bool allowed)
{
    iAllowDuplicates = allowed;
}

bool HardwareVertexBufferPrivate::areDuplicatesAllowed() const
{
    return iAllowDuplicates;
}

void HardwareVertexBufferPrivate::activateColor(bool activate)
{
    iColorActivated = activate;
}

bool HardwareVertexBufferPrivate::isColorActivated() const
{
    return iColorActivated;
}

void HardwareVertexBufferPrivate::activateTexCoord(bool activate)
{
    iTexCoordActivated = activate;
}

bool HardwareVertexBufferPrivate::isTexCoordActivated() const
{
    return iTexCoordActivated;
}

// ---------------------------------------------------------------------------------------------------

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBufferPrivate* pointer)
: HardwareBuffer(pointer), SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(pointer)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBufferHolder& holder)
: HardwareBuffer(holder.get()), SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(holder)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBuffer& user)
: HardwareBuffer(user), SpecializedResourceUser<Gre::HardwareVertexBufferPrivate>(user)
{
    
}

HardwareVertexBuffer::~HardwareVertexBuffer()
{
    
}

HardwareVertexBufferHolder HardwareVertexBuffer::lock()
{
    return SpecializedResourceUser<HardwareVertexBufferPrivate>::lock();
}

const HardwareVertexBufferHolder HardwareVertexBuffer::lock() const
{
    return SpecializedResourceUser<HardwareVertexBufferPrivate>::lock();
}

void HardwareVertexBuffer::add(const Vertex& vertex)
{
    auto ptr = lock();
    if(ptr)
        ptr->add(vertex);
}

void HardwareVertexBuffer::add(const VertexBatch &batch)
{
    auto ptr = lock();
    if(ptr)
        ptr->add(batch);
}

bool HardwareVertexBuffer::contains(const Vertex &vertex) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->contains(vertex);
    return false;
}

void HardwareVertexBuffer::setDuplicatesAllowed(bool allowed)
{
    auto ptr = lock();
    if(ptr)
        ptr->setDuplicatesAllowed(allowed);
}

bool HardwareVertexBuffer::areDuplicatesAllowed() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->areDuplicatesAllowed();
    return false;
}

void HardwareVertexBuffer::activateColor(bool activate)
{
    auto ptr = lock();
    if(ptr)
        ptr->activateColor(activate);
}

bool HardwareVertexBuffer::isColorActivated() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isColorActivated();
    return false;
}

void HardwareVertexBuffer::activateTexCoord(bool activate)
{
    auto ptr = lock();
    if(ptr)
        ptr->activateTexCoord(activate);
}

bool HardwareVertexBuffer::isTexCoordActivated() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isTexCoordActivated();
    return false;
}

HardwareVertexBuffer HardwareVertexBuffer::Null = HardwareVertexBuffer(nullptr);

GreEndNamespace
