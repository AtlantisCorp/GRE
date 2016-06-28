//////////////////////////////////////////////////////////////////////
//
//  HardwareIndexBuffer.cpp
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

#include "HardwareIndexBuffer.h"

GreBeginNamespace

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate(const std::string& name)
: Gre::HardwareBufferPrivate(name), iPrimitiveType(PrimitiveType::Triangles), iStorageType(StorageType::UnsignedInt)
{
    iPrimitiveType = PrimitiveType::Triangles;
    iStorageType = StorageType::UnsignedInt;
    iFaces.indexedFaces.reserve(100);
}

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate(const std::string& name, PrimitiveType ptype, StorageType stype)
: Gre::HardwareIndexBufferPrivate(name)
{
    iPrimitiveType = ptype;
    iStorageType = stype;
    iFaces.indexedFaces.reserve(100);
}

HardwareIndexBufferPrivate::~HardwareIndexBufferPrivate()
{
    
}

size_t HardwareIndexBufferPrivate::getSize() const
{
    return sizeof(unsigned) * count();
}

size_t HardwareIndexBufferPrivate::count() const
{
    unsigned ret = 0;
    
    for(auto indexedFace : iFaces.indexedFaces)
        ret += indexedFace.indices.size();
    
    return ret;
}

void HardwareIndexBufferPrivate::add(const IndexedFace &index)
{
    iFaces.indexedFaces.push_back(index);
    setDirty(true);
}

void HardwareIndexBufferPrivate::add(const IndexedFaceBatch &batch)
{
    for(auto face : batch)
        add(face);
}

void HardwareIndexBufferPrivate::setMaterial(const Material &material)
{
    iFaces.material = material;
}

Material& HardwareIndexBufferPrivate::getMaterial()
{
    return iFaces.material;
}

const Material& HardwareIndexBufferPrivate::getMaterial() const
{
    return iFaces.material;
}

PrimitiveType HardwareIndexBufferPrivate::getPrimitiveType() const
{
    return iPrimitiveType;
}

StorageType HardwareIndexBufferPrivate::getStorageType() const
{
    return iStorageType;
}

// ---------------------------------------------------------------------------------------------------

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferPrivate* resource)
: Gre::HardwareBuffer(resource), SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(resource)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBufferHolder& holder)
: Gre::HardwareBuffer(holder.get()), SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(holder)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBuffer& user)
: Gre::HardwareBuffer(user), SpecializedResourceUser<Gre::HardwareIndexBufferPrivate>(user)
{
    
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    
}

HardwareIndexBufferHolder HardwareIndexBuffer::lock()
{
    return SpecializedResourceUser<HardwareIndexBufferPrivate>::lock();
}

const HardwareIndexBufferHolder HardwareIndexBuffer::lock() const
{
    return SpecializedResourceUser<HardwareIndexBufferPrivate>::lock();
}

void HardwareIndexBuffer::add(const IndexedFace &index)
{
    auto ptr = lock();
    if(ptr)
        ptr->add(index);
}

void HardwareIndexBuffer::add(const IndexedFaceBatch &index)
{
    auto ptr = lock();
    if(ptr)
        ptr->add(index);
}

void HardwareIndexBuffer::setMaterial(const Material &material)
{
    auto ptr = lock();
    if(ptr)
        ptr->setMaterial(material);
}

Material& HardwareIndexBuffer::getMaterial()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getMaterial();
    return Material::Null;
}

const Material& HardwareIndexBuffer::getMaterial() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getMaterial();
    return Material::Null;
}

PrimitiveType HardwareIndexBuffer::getPrimitiveType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getPrimitiveType();
    return PrimitiveType::Triangles;
}

StorageType HardwareIndexBuffer::getStorageType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getStorageType();
    return StorageType::UnsignedInt;
}

HardwareIndexBuffer HardwareIndexBuffer::Null = HardwareIndexBuffer(nullptr);

GreEndNamespace


