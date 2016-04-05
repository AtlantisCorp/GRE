//
//  HardwareBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "HardwareBuffer.h"

GreBeginNamespace

HardwareBufferPrivate::HardwareBufferPrivate()
: Resource("")
{
    
}

HardwareBufferPrivate::~HardwareBufferPrivate()
{
    
}

size_t HardwareBufferPrivate::getSize() const
{
    GreDebugFunctionNotImplemented();
    return 0;
}

size_t HardwareBufferPrivate::count() const
{
    GreDebugFunctionNotImplemented();
    return 0;
}

void HardwareBufferPrivate::bind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareBufferPrivate::unbind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareBufferPrivate::update() const
{
    GreDebugFunctionNotImplemented();
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
    GreDebugFunctionNotImplemented();
    return true;
}

HardwareBuffer::HardwareBuffer()
{
    
}

HardwareBuffer::~HardwareBuffer()
{
    
}

GreEndNamespace
