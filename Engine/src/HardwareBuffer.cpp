//
//  HardwareBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "HardwareBuffer.h"

GRE_BEGIN_NAMESPACE

HardwareBufferPrivate::HardwareBufferPrivate()
: Resource("")
{
    
}

HardwareBufferPrivate::~HardwareBufferPrivate()
{
    
}

size_t HardwareBufferPrivate::getSize() const
{
    return 0;
}

void HardwareBufferPrivate::bind() const
{
    
}

void HardwareBufferPrivate::unbind() const
{
    
}

void HardwareBufferPrivate::update() const
{
    
}

bool HardwareBufferPrivate::isDirty() const
{
    return _mIsDirty;
}

void HardwareBufferPrivate::setDirty(bool dirty) const
{
    _mIsDirty = dirty;
}

bool HardwareBufferPrivate::isInvalid() const
{
    return true;
}

HardwareBuffer::HardwareBuffer()
{
    
}

HardwareBuffer::~HardwareBuffer()
{
    
}

GRE_END_NAMESPACE
