//////////////////////////////////////////////////////////////////////
//
//  RenderingApiDescriptor.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 21/08/2016.
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

#include "RenderingApiDescriptor.h"

GreBeginNamespace

RenderingApiDescriptor::RenderingApiDescriptor()
{
    
}

RenderingApiDescriptor::~RenderingApiDescriptor()
{
    
}

void RenderingApiDescriptor::setApiName(const std::string &name)
{
    iApiName = name;
}

const std::string& RenderingApiDescriptor::getApiName() const
{
    return iApiName;
}

void RenderingApiDescriptor::setMinVersion(const std::string &minversion)
{
    iMinVersion = minversion;
}

const std::string& RenderingApiDescriptor::getMinVersion() const
{
    return iMinVersion;
}

void RenderingApiDescriptor::setMaxVersion(const std::string &maxversion)
{
    iMaxVersion = maxversion;
}

const std::string& RenderingApiDescriptor::getMaxVersion() const
{
    return iMaxVersion;
}

GreEndNamespace

#include <stdio.h>