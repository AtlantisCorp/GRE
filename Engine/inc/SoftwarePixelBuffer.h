//////////////////////////////////////////////////////////////////////
//
//  SoftwarePixelBuffer.h
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

#ifndef SoftwarePixelBuffer_h
#define SoftwarePixelBuffer_h

#include "Pools.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief PixelFormat possibilities.
enum class PixelFormat : int
{
    None , RGB , BGR , RGBA , BGRA , Red , RG ,
    StencilIndex , DepthComponent , DepthStencil ,
    Luminance , LuminanceAlpha
};

/// @brief Translates string to pixel format. Default is None.
PixelFormat PixelFormatFromString ( const std::string & format ) ;

/// @brief Returns the number of element in a given format.
size_t PixelFormatGetCount ( const PixelFormat & format ) ;

//////////////////////////////////////////////////////////////////////
/// @brief InternalPixelFormat possibilities.
enum class InternalPixelFormat : int
{
    DepthComponent, DepthStencil, Red, RG, RGB, RGBA,
    RGB4, RGB8, RGB12, RGB16, RGB32,
    RGBA2, RGBA4, RGBA8, RGBA12, RGBA16, RGBA32,

    CompressedRed , CompressedRG , CompressedRGB ,
    CompressedRGBA ,

    None
};

/// @brief Translates string to InternalPixelFormat.
InternalPixelFormat InternalPixelFormatFromString ( const std::string & format ) ;

//////////////////////////////////////////////////////////////////////
/// @brief PixelType Possibilities.
enum class PixelType : int
{
    UnsignedByte, Byte,
    UnsignedShort, Short,
    UnsignedInt, Int,
    Float,

    None
};

/// @brief Translates string to PixelType.
PixelType PixelTypeFromString ( const std::string & type ) ;

/// @brief Returns the size of a given pixel's type.
size_t PixelTypeGetSize ( const PixelType & type ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Represents a static buffer filled with pixels.
///
/// The pixels are represented following the given pixel format. This format
/// can be compressed or not. You should not change the pixels datas in
/// this buffer, as the data represention of the pixels is not always
/// none.
///
/// Pass this object to a texture to create a new texture from the
/// pixels in this buffer. The PixelFormat should be a valid format
/// handled by the driver, as welll as the InternalPixelFormat.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SoftwarePixelBuffer : public Resource
{
public:

    POOLED(Pools::HdwBuffer)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SoftwarePixelBuffer () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SoftwarePixelBuffer() noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iInternalPixelFormat'.
    //////////////////////////////////////////////////////////////////////
    virtual const InternalPixelFormat & getInternalPixelFormat () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iInternalPixelFormat'.
    //////////////////////////////////////////////////////////////////////
    virtual void setInternalPixelFormat ( const InternalPixelFormat& format ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSurface'.
    //////////////////////////////////////////////////////////////////////
    virtual const Surface & getSurface () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iSurface'.
    //////////////////////////////////////////////////////////////////////
    virtual void setSurface ( const Surface& surface ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPixelFormat'.
    //////////////////////////////////////////////////////////////////////
    virtual const PixelFormat & getPixelFormat () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelFormat ( const PixelFormat& format ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPixelType'.
    //////////////////////////////////////////////////////////////////////
    virtual const PixelType & getPixelType () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelType ( const PixelType& type ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPixBuffer'.
    //////////////////////////////////////////////////////////////////////
    virtual const void* getData () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPixBuffer'.
    //////////////////////////////////////////////////////////////////////
    virtual void* getData () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setData ( const void* data , size_t sz ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iDepth'.
    //////////////////////////////////////////////////////////////////////
    virtual int getDepth () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setDepth ( int depth ) ;

protected:

    /// @brief Holds the data, independently from its type.
    unsigned char* iPixBuffer;

    /// @brief Size of the data, in bytes.
    size_t iSize;

    /// @brief Surface represented by the pixel buffer.
    Surface iSurface ;

    /// @brief Current Pixel Format.
    PixelFormat iPixelFormat ;

    /// @brief Current Pixel Type.
    PixelType iPixelType ;

    /// @brief Desired machine Pixel Format.
    InternalPixelFormat iInternalPixelFormat ;

    /// @brief if the SoftwarePixelBuffer is a 3D image, it can have a
    /// depth component. This will be used by the texture to now depth of the image.
    int iDepth ;
};

/// @brief Holder for SoftwarePixelBufferPrivate.
typedef Holder<SoftwarePixelBuffer> SoftwarePixelBufferHolder;

/// @brief SpecializedResourceHolderList for SoftwarePixelBufferPrivate.
typedef SpecializedResourceHolderList<SoftwarePixelBuffer> SoftwarePixelBufferHolderList;

GreEndNamespace

#endif /* SoftwarePixelBuffer_h */
