//
//  HardwareVertexBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "HardwareVertexBuffer.h"

GRE_BEGIN_NAMESPACE

HardwareVertexBufferPrivate::HardwareVertexBufferPrivate()
{
    _mAllowDuplicates = false;
}

HardwareVertexBufferPrivate::~HardwareVertexBufferPrivate()
{
    
}

void HardwareVertexBufferPrivate::add(const Vertex &vertex)
{
    if(areDuplicatesAllowed()) {
        _mVertexs.push_back(vertex);
        setDirty(true);
    }
    else if(!contains(vertex)) {
        _mVertexs.push_back(vertex);
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
    return std::find(_mVertexs.begin(), _mVertexs.end(), vertex) != _mVertexs.end();
}

void HardwareVertexBufferPrivate::setDuplicatesAllowed(bool allowed)
{
    _mAllowDuplicates = allowed;
}

bool HardwareVertexBufferPrivate::areDuplicatesAllowed() const
{
    return _mAllowDuplicates;
}

HardwareVertexBuffer::HardwareVertexBuffer()
: _mBuffer()
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const HardwareVertexBuffer& buffer)
: _mBuffer(buffer._mBuffer)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(HardwareVertexBuffer&& buffer)
: _mBuffer(std::move(buffer._mBuffer))
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(std::weak_ptr<HardwareVertexBufferPrivate> bufferptr)
: _mBuffer(bufferptr)
{
    
}

HardwareVertexBuffer::HardwareVertexBuffer(const ResourceUser& ruser)
: _mBuffer(std::dynamic_pointer_cast<HardwareVertexBufferPrivate>(ruser.lock()))
{
    
}

HardwareVertexBuffer::~HardwareVertexBuffer()
{
    
}

void HardwareVertexBuffer::bind() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->bind();
}

size_t HardwareVertexBuffer::getSize() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getSize();
    return 0;
}

bool HardwareVertexBuffer::isDirty() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isDirty();
    return false;
}

bool HardwareVertexBuffer::isExpired() const
{
    return _mBuffer.expired();
}

void HardwareVertexBuffer::add(const Vertex& vertex)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->add(vertex);
}

void HardwareVertexBuffer::add(const VertexBatch &batch)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->add(batch);
}

bool HardwareVertexBuffer::contains(const Vertex &vertex) const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->contains(vertex);
    return false;
}

void HardwareVertexBuffer::setDuplicatesAllowed(bool allowed)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->setDuplicatesAllowed(allowed);
}

bool HardwareVertexBuffer::areDuplicatesAllowed() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->areDuplicatesAllowed();
    return false;
}

bool HardwareVertexBuffer::isInvalid() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isInvalid();
    return true;
}

HardwareVertexBuffer& HardwareVertexBuffer::operator=(const HardwareVertexBuffer &rhs)
{
    _mBuffer = rhs._mBuffer;
    return *this;
}

HardwareVertexBuffer HardwareVertexBuffer::Null = HardwareVertexBuffer();

GRE_END_NAMESPACE
