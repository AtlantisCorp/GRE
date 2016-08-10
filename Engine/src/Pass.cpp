//////////////////////////////////////////////////////////////////////
//
//  Pass.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/01/2016.
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

#include "Pass.h"
#include "Renderer.h"

GreBeginNamespace

PassPrivate::PassPrivate(const std::string& name, const PassNumber& passNumber)
: Resource(name), iNumber(passNumber), iIsActivated(true), iLinkedProgram(HardwareProgram::Null)
{
    
}

PassPrivate::~PassPrivate()
{
    
}

void PassPrivate::setActivated(bool activate)
{
    iIsActivated = activate;
}

bool PassPrivate::isActivated() const
{
    return iIsActivated;
}

void PassPrivate::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    iLinkedProgram = hwdProgram;
}

HardwareProgram PassPrivate::getHardwareProgram() const
{
    return iLinkedProgram;
}

PassNumber PassPrivate::getPassNumber() const
{
    return iNumber;
}

// ---------------------------------------------------------------------------------------------------

Pass::Pass(const PassPrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser<Gre::PassPrivate>(pointer)
{
    
}

Pass::Pass(const PassHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser<Gre::PassPrivate>(holder)
{
    
}

Pass::Pass(const Pass& user)
: ResourceUser(user)
, SpecializedResourceUser<Gre::PassPrivate>(user)
{
    
}

Pass::~Pass()
{
    
}

void Pass::setActivated(bool activate)
{
    auto ptr = lock();
    
    if(ptr)
        ptr->setActivated(activate);
}

bool Pass::isActivated() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->isActivated();
    return false;
}

void Pass::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    auto ptr = lock();
    
    if(ptr)
        ptr->setHardwareProgram(hwdProgram);
}

HardwareProgram Pass::getHardwareProgram() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->getHardwareProgram();
    return HardwareProgram::Null;
}

PassNumber Pass::getPassNumber() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->getPassNumber();
    return (PassNumber) 0;
}

Pass Pass::Null = Pass(nullptr);

GreEndNamespace
