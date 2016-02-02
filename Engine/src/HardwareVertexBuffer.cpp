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
    _mColorActivated  = false;
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
    return _mVertexs.size();
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

void HardwareVertexBufferPrivate::activateColor(bool activate)
{
    _mColorActivated = activate;
}

bool HardwareVertexBufferPrivate::isColorActivated() const
{
    return _mColorActivated;
}

void HardwareVertexBufferPrivate::activateTexCoord(bool activate)
{
    _mTexCoordActivated = activate;
}

bool HardwareVertexBufferPrivate::isTexCoordActivated() const
{
    return _mTexCoordActivated;
}

// ---------------------------------------------------------------------------------------------------

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

void HardwareVertexBuffer::unbind() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->unbind();
}

void HardwareVertexBuffer::update() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->update();
}

size_t HardwareVertexBuffer::getSize() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->getSize();
    return 0;
}

size_t HardwareVertexBuffer::count() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->count();
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

void HardwareVertexBuffer::activateColor(bool activate)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->activateColor(activate);
}

bool HardwareVertexBuffer::isColorActivated() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isColorActivated();
    return false;
}

void HardwareVertexBuffer::activateTexCoord(bool activate)
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        ptr->activateTexCoord(activate);
}

bool HardwareVertexBuffer::isTexCoordActivated() const
{
    auto ptr = _mBuffer.lock();
    if(ptr)
        return ptr->isTexCoordActivated();
    return false;
}

HardwareVertexBuffer& HardwareVertexBuffer::operator=(const HardwareVertexBuffer &rhs)
{
    _mBuffer = rhs._mBuffer;
    return *this;
}

HardwareVertexBuffer HardwareVertexBuffer::Null = HardwareVertexBuffer();

GRE_END_NAMESPACE
