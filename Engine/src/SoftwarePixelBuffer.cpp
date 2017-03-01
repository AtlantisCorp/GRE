//////////////////////////////////////////////////////////////////////
//
//  SoftwarePixelBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 02/07/2016.
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

#include "SoftwarePixelBuffer.h"

GreBeginNamespace

SoftwarePixelBuffer::SoftwarePixelBuffer ( )
: Gre::Resource ( "" )
{
    iPixBuffer = nullptr ;
    iSize = 0 ;
}

SoftwarePixelBuffer::~SoftwarePixelBuffer() noexcept ( false )
{
    if ( iPixBuffer )
        free ( iPixBuffer ) ;
}

const InternalPixelFormat & SoftwarePixelBuffer::getInternalPixelFormat() const
{
    GreAutolock ; return iInternalPixelFormat ;
}

void SoftwarePixelBuffer::setInternalPixelFormat(const Gre::InternalPixelFormat &format)
{
    GreAutolock ; iInternalPixelFormat = format ;
}

const Surface & SoftwarePixelBuffer::getSurface() const
{
    GreAutolock ; return iSurface ;
}

void SoftwarePixelBuffer::setSurface(const Gre::Surface &surface)
{
    GreAutolock ; iSurface = surface ;
}

const PixelFormat & SoftwarePixelBuffer::getPixelFormat() const
{
    GreAutolock ; return iPixelFormat ;
}

void SoftwarePixelBuffer::setPixelFormat(const Gre::PixelFormat &format)
{
    GreAutolock ; iPixelFormat = format ;
}

const PixelType & SoftwarePixelBuffer::getPixelType() const
{
    GreAutolock ; return iPixelType ;
}

void SoftwarePixelBuffer::setPixelType(const Gre::PixelType &type)
{
    GreAutolock ; iPixelType = type ;
}

const void* SoftwarePixelBuffer::getData () const
{
    GreAutolock ; return (const void*) iPixBuffer ;
}

void* SoftwarePixelBuffer::getData ()
{
    GreAutolock ; return (void*) iPixBuffer ;
}

void SoftwarePixelBuffer::setData(const void *data, size_t sz)
{
    GreAutolock ;
    
    if ( iPixBuffer ) free ( iPixBuffer ) ;
    
    iPixBuffer = (unsigned char*) malloc ( sz ) ;
    iSize = sz ;
    
    // If one wants to set the data size before setting any data , we should allow creation
    // of a new buffer and grant access to the resulting buffer in order for the user to directly
    // modify it as needed.
    
    if ( data ) {
        memcpy ( iPixBuffer , data , sz ) ;
    }
    else {
        memset ( iPixBuffer , 0  , sz ) ;
    }
}

int SoftwarePixelBuffer::getDepth () const
{
    GreAutolock ; return iDepth ;
}

void SoftwarePixelBuffer::setDepth(int depth)
{
    GreAutolock ; iDepth = depth ;
}

// ---------------------------------------------------------------------------------------------------

GreEndNamespace
