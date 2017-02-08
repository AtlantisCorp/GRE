//////////////////////////////////////////////////////////////////////
//
//  HardwareBuffer.cpp
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

#include "HardwareBuffer.h"

GreBeginNamespace

HardwareBuffer::HardwareBuffer(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{
    
}

HardwareBuffer::~HardwareBuffer()
{
    
}

size_t HardwareBuffer::getSize() const
{
    return 0;
}

size_t HardwareBuffer::count() const
{
    return 0;
}

void HardwareBuffer::bind() const
{
    
}

void HardwareBuffer::unbind() const
{

}

void HardwareBuffer::update()
{

}

bool HardwareBuffer::isDirty() const
{
    return iIsDirty;
}

void HardwareBuffer::setDirty(bool dirty) const
{
    iIsDirty = dirty;
}

bool HardwareBuffer::isDataInvalid() const
{
    return true;
}

void HardwareBuffer::addData(const char *vdata, size_t sz)
{
    
}

const char* HardwareBuffer::getData() const
{
    return nullptr;
}

void HardwareBuffer::clearData()
{
    
}

GreEndNamespace
