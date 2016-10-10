//////////////////////////////////////////////////////////////////////
//
//  HardwarePixel.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/07/2016.
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

#include "HardwarePixel.h"

GreBeginNamespace

HardwarePixelFormatDescriptor::HardwarePixelFormatDescriptor()
: iFormat(HardwarePixelFormat::Null), iSize(0), iName("null")
{
    
}

// ---------------------------------------------------------------------------------------------------

HardwarePixelFormatDescriptorManager::HardwarePixelFormatDescriptorManager()
{
    // Register Core HardwarePixelFormat .
    
    HardwarePixelFormatDescriptor s1;
    s1.iFormat = HardwarePixelFormat::RGBAShort;
    s1.iSize = sizeof ( short );
    s1.iName = "GL_SHORT GL_RGBA";
    registerDescriptor(s1);
    
    HardwarePixelFormatDescriptor s2;
    s2.iFormat = HardwarePixelFormat::RGBAInt;
    s2.iSize = sizeof ( int );
    s2.iName = "GL_INT GL_RGBA";
    registerDescriptor(s2);
    
    HardwarePixelFormatDescriptor s3;
    s3.iFormat = HardwarePixelFormat::RGBAFloat;
    s3.iSize = sizeof ( float );
    s3.iName = "GL_FLOAT GL_RGBA";
    registerDescriptor(s3);
}

HardwarePixelFormatDescriptorManager::~HardwarePixelFormatDescriptorManager()
{
    
}

void HardwarePixelFormatDescriptorManager::registerDescriptor(const Gre::HardwarePixelFormatDescriptor &descriptor)
{
    iDescriptors.push_back(descriptor);
}

const HardwarePixelFormatDescriptor HardwarePixelFormatDescriptorManager::getDescriptor(const Gre::HardwarePixelFormat &format) const
{
    for(auto it = iDescriptors.begin(); it != iDescriptors.end(); it++)
    {
        if( it->iFormat == format )
        {
            return *it;
        }
    }
    
    return HardwarePixelFormatDescriptor();
}

const HardwarePixelFormatDescriptor HardwarePixelFormatDescriptorManager::getDescriptorByName(const std::string &name) const
{
    for(auto it = iDescriptors.begin(); it != iDescriptors.end(); it++)
    {
        if( it->iName == name )
        {
            return *it;
        }
    }
    
    return HardwarePixelFormatDescriptor();
}

HardwarePixelFormatDescriptorList HardwarePixelFormatDescriptorManager::getDescriptorsBySize(size_t size) const
{
    HardwarePixelFormatDescriptorList ret;
    
    for(auto it = iDescriptors.begin(); it != iDescriptors.end(); it++)
    {
        if( it->iSize == size )
        {
            ret.push_back( *it );
        }
    }
    
    return ret;
}

HardwarePixelFormatDescriptorList HardwarePixelFormatDescriptorManager::getDescriptors() const
{
    return iDescriptors;
}

HardwarePixelFormatDescriptorManager HardwarePixelFormatDescriptorManager::Instance = HardwarePixelFormatDescriptorManager();

GreEndNamespace
