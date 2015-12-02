//
//  HardwareIndexBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "HardwareIndexBuffer.h"

GRE_BEGIN_NAMESPACE

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate()
: _mPrimitiveType(PrimitiveType::Triangles), _mStorageType(StorageType::UnsignedInt)
{
    
}

HardwareIndexBufferPrivate::HardwareIndexBufferPrivate(PrimitiveType ptype, StorageType stype)
: _mPrimitiveType(ptype), _mStorageType(stype)
{
    
}

HardwareIndexBufferPrivate::~HardwareIndexBufferPrivate()
{
    
}

void HardwareIndexBufferPrivate::add(const IndexedFace &index)
{
    _mFaces.indexedFaces.push_back(index);
    setDirty(true);
}

void HardwareIndexBufferPrivate::add(const IndexedFaceBatch &batch)
{
    for(auto face : batch)
        add(face);
}

void HardwareIndexBufferPrivate::setMaterial(const Material &material)
{
    _mFaces.material = material;
}

Material& HardwareIndexBufferPrivate::getMaterial()
{
    return _mFaces.material;
}

const Material& HardwareIndexBufferPrivate::getMaterial() const
{
    return _mFaces.material;
}

PrimitiveType HardwareIndexBufferPrivate::getPrimitiveType() const
{
    return _mPrimitiveType;
}

StorageType HardwareIndexBufferPrivate::getStorageType() const
{
    return _mStorageType;
}

size_t HardwareIndexBufferPrivate::getElementCount() const
{
    return 0;
}

HardwareIndexBuffer::HardwareIndexBuffer()
: _mBuffer()
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const HardwareIndexBuffer& buffer)
: _mBuffer(buffer._mBuffer)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(HardwareIndexBuffer&& buffer)
: _mBuffer(std::move(buffer._mBuffer))
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(std::weak_ptr<HardwareIndexBufferPrivate> bufferptr)
: _mBuffer(bufferptr)
{
    
}

HardwareIndexBuffer::HardwareIndexBuffer(const ResourceUser& ruser)
: _mBuffer(std::dynamic_pointer_cast<HardwareIndexBufferPrivate>(ruser.lock()))
{
    
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    
}

void HardwareIndexBuffer::bind() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->bind();
}

void HardwareIndexBuffer::unbind() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->unbind();
}

void HardwareIndexBuffer::update() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->update();
}

size_t HardwareIndexBuffer::getSize() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getSize();
    return 0;
}

bool HardwareIndexBuffer::isDirty() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isDirty();
    return false;
}

bool HardwareIndexBuffer::isExpired() const
{
    return _mBuffer.expired();
}

void HardwareIndexBuffer::add(const IndexedFace &index)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->add(index);
}

void HardwareIndexBuffer::add(const IndexedFaceBatch &index)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->add(index);
}

void HardwareIndexBuffer::setMaterial(const Material &material)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->setMaterial(material);
}

bool HardwareIndexBuffer::isInvalid() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isInvalid();
    return true;
}

HardwareIndexBuffer& HardwareIndexBuffer::operator=(const HardwareIndexBuffer &rhs)
{
    _mBuffer = rhs._mBuffer;
    return *this;
}

Material& HardwareIndexBuffer::getMaterial()
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getMaterial();
    return Material::Null;
}

const Material& HardwareIndexBuffer::getMaterial() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getMaterial();
    return Material::Null;
}

PrimitiveType HardwareIndexBuffer::getPrimitiveType() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getPrimitiveType();
    return PrimitiveType::Triangles;
}

StorageType HardwareIndexBuffer::getStorageType() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getStorageType();
    return StorageType::UnsignedInt;
}

size_t HardwareIndexBuffer::getElementCount() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getElementCount();
    return 0;
}

HardwareIndexBuffer HardwareIndexBuffer::Null = HardwareIndexBuffer();

GRE_END_NAMESPACE


