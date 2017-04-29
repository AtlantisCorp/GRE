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

// -----------------------------------------------------------------------------

PixelFormat PixelFormatFromString ( const std::string & format )
{
    if ( format == "RGB" ) return PixelFormat::RGB ;
    else if ( format == "BGR" ) return PixelFormat::BGR ;
    else if ( format == "RGBA" ) return PixelFormat::RGBA ;
    else if ( format == "BGRA" ) return PixelFormat::BGRA ;
    else if ( format == "Red" ) return PixelFormat::Red ;
    else if ( format == "RG" ) return PixelFormat::RG ;
    else if ( format == "StencilIndex" ) return PixelFormat::StencilIndex ;
    else if ( format == "DepthComponent" ) return PixelFormat::DepthComponent ;
    else if ( format == "DepthStencil" ) return PixelFormat::DepthStencil ;
    else if ( format == "Luminance" ) return PixelFormat::Luminance ;
    else if ( format == "LuminanceAlpha" ) return PixelFormat::LuminanceAlpha ;

    return PixelFormat::None ;
}

size_t PixelFormatGetCount ( const PixelFormat & format )
{
    if ( format == PixelFormat::RGB ) return 3 ;
    else if ( format == PixelFormat::BGR ) return 3 ;
    else if ( format == PixelFormat::RGBA ) return 4 ;
    else if ( format == PixelFormat::BGRA ) return 4 ;
    else if ( format == PixelFormat::Red ) return 1 ;
    else if ( format == PixelFormat::RG ) return 2 ;
    else if ( format == PixelFormat::StencilIndex ) return 1 ;
    else if ( format == PixelFormat::DepthComponent ) return 1 ;
    else if ( format == PixelFormat::DepthStencil ) return 2 ;
    else if ( format == PixelFormat::Luminance ) return 1 ;
    else if ( format == PixelFormat::LuminanceAlpha ) return 2 ;

    return 0 ;
}

InternalPixelFormat InternalPixelFormatFromString ( const std::string & format )
{
    if ( format == "DepthComponent" ) return InternalPixelFormat::DepthComponent ;
    else if ( format == "DepthStencil" ) return InternalPixelFormat::DepthStencil ;

    else if ( format == "Red" ) return InternalPixelFormat::Red ;
    else if ( format == "RG" ) return InternalPixelFormat::RG ;
    else if ( format == "RGB" ) return InternalPixelFormat::RGB ;
    else if ( format == "RGBA" ) return InternalPixelFormat::RGBA ;

    else if ( format == "RGB4" ) return InternalPixelFormat::RGB4 ;
    else if ( format == "RGB8" ) return InternalPixelFormat::RGB8 ;
    else if ( format == "RGB12" ) return InternalPixelFormat::RGB12 ;
    else if ( format == "RGB16" ) return InternalPixelFormat::RGB16 ;
    else if ( format == "RGB32" ) return InternalPixelFormat::RGB32 ;

    else if ( format == "RGBA2" ) return InternalPixelFormat::RGBA2 ;
    else if ( format == "RGBA4" ) return InternalPixelFormat::RGBA4 ;
    else if ( format == "RGBA8" ) return InternalPixelFormat::RGBA8 ;
    else if ( format == "RGBA12" ) return InternalPixelFormat::RGBA12 ;
    else if ( format == "RGBA16" ) return InternalPixelFormat::RGBA16 ;
    else if ( format == "RGBA32" ) return InternalPixelFormat::RGBA32 ;

    else if ( format == "CompressedRed" ) return InternalPixelFormat::CompressedRed ;
    else if ( format == "CompressedRG" ) return InternalPixelFormat::CompressedRG ;
    else if ( format == "CompressedRGB" ) return InternalPixelFormat::CompressedRGB ;
    else if ( format == "CompressedRGBA" ) return InternalPixelFormat::CompressedRGBA ;

    return InternalPixelFormat::None ;
}

PixelType PixelTypeFromString ( const std::string & type )
{
    if ( type == "UnsignedByte" ) return PixelType::UnsignedByte ;
    else if ( type == "Byte" ) return PixelType::Byte ;
    else if ( type == "UnsignedShort" ) return PixelType::UnsignedShort ;
    else if ( type == "Short" ) return PixelType::Short ;
    else if ( type == "UnsignedInt" ) return PixelType::UnsignedInt ;
    else if ( type == "Int" ) return PixelType::Int ;
    else if ( type == "Float" ) return PixelType::Float ;

    return PixelType::None ;
}

size_t PixelTypeGetSize ( const PixelType & type )
{
    if ( type == PixelType::UnsignedByte ) return sizeof ( uint8_t ) ;
    else if ( type == PixelType::Byte ) return sizeof ( int8_t ) ;
    else if ( type == PixelType::UnsignedShort ) return sizeof ( uint16_t ) ;
    else if ( type == PixelType::Short ) return sizeof ( uint16_t ) ;
    else if ( type == PixelType::UnsignedInt ) return sizeof ( uint32_t ) ;
    else if ( type == PixelType::Int ) return sizeof ( int32_t ) ;
    else if ( type == PixelType::Float ) return sizeof ( float ) ;

    return 0 ;
}

// -----------------------------------------------------------------------------

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
