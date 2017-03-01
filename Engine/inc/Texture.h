//////////////////////////////////////////////////////////////////////
//
//  Texture.h
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

#ifndef GRE_Texture_h
#define GRE_Texture_h

#include "Pools.h"
#include "Resource.h"
#include "SpecializedResourceManager.h"

#include "SoftwarePixelBuffer.h"
#include "HardwareSampler.h"
#include "HardwareProgram.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Resumes the Texture's possible targets (types).
enum class TextureType
{
    Texture1D, Texture2D, Texture3D,
    CubeMap, Null
};

//////////////////////////////////////////////////////////////////////
/// @brief Lists the parameters available for the Texture at creation.
enum class TextureParameter : int
{
    /// @brief Specifies the mode used to read from depth-stencil format textures.
    DepthStencilMode ,
    
    /// @brief Specifies the index of the lowest defined mipmap level. This is an
    /// integer value. The initial value is 0.
    MipmapBaseLevel ,
    
    /// @brief Specifies a Color value that define the border values that should
    /// be used for border texels.
    BorderColor ,
    
    /// @brief Specifies the comparison operator used when TextureCompareMode is set
    /// to TextureCompareMode::Ref .
    CompareFunc ,
    
    /// @brief Specifies the texture comparison mode for currently bound depth textures.
    CompareMode ,
    
    /// @brief Specifies a fixed bias value that is to be added to the level-of-detail
    /// parameter for the texture before texture sampling.
    LODBias ,
    
    /// @brief Supplies a function for minifying the texture.
    MinFilter ,
    
    /// @brief Supplies a function for magnifying the texture.
    MagFilter ,
    
    /// @brief Sets the wrap parameter for texture coordinate s.
    WrapS ,
    
    /// @brief Sets the wrap parameter for texture coordinate t.
    WrapT ,
    
    /// @brief Sets the wrap parameter for texture coordinate r.
    WrapR
};

//////////////////////////////////////////////////////////////////////
/// @brief TextureWrap values.
enum class TextureWrap : int
{
    ClampToEdge, ClampToBorder,
    MirroredRepeat, Repeat,
    MirrorClampToEdge
};

//////////////////////////////////////////////////////////////////////
/// @brief TextureMagFilter values.
enum class TextureMagFilter : int
{
    Nearest, Linear
};

//////////////////////////////////////////////////////////////////////
/// @brief TextureMinFilter values.
enum class TextureMinFilter : int
{
    Nearest, Linear,
    MipmapLinear, MipmapNearest
};

//////////////////////////////////////////////////////////////////////
/// @brief TextureCompareMode values.
enum class TextureCompareMode : int
{
    Ref, None
};

//////////////////////////////////////////////////////////////////////
/// @brief TextureCompareFunc values.
enum class TextureCompareFunc : int
{
    LessEqual, GreatEqual, Less, Greater,
    Equal, NotEqual, Always, Never
};

//////////////////////////////////////////////////////////////////////
/// @brief Modes for DepthStencilMode.
enum class TextureDepthStencilMode : int
{
    Depth , Stencil
};

/// @brief Translates the TextureType value to a String.
std::string TextureTypeToString(const TextureType& type);

//////////////////////////////////////////////////////////////////////
/// @brief Represents a Texture Resource object.
///
/// A Texture object should contains the data related to Texture's
/// objects, as the Images pixels.
///
/// ### Loading a Texture
///
/// You have to use the TextureManager, registered to the ResourceManager
/// by the currently installed Renderer. The TextureManager use one of the
/// TextureFileLoader to loads a pixel format data and then registers the
/// texture using the underlying Driver ( OpenGl, DirectX ) .
///
/// ### Creating a TextureFileLoader
///
/// The TextureFileLoader creates only a SoftwarePixelBuffer wich serves
/// the TextureManager to load the Texture on the GPU. You should fill it
/// with the correct PixelFormat and the data from the image file.
///
/// ### Manipulating Texture
///
/// As Textures objects are creating onto the GPU, you can't directly
/// modify them. You can get their data by copy, modify it, and then reupload
/// them into the Texture GPU buffer.
///
/// ### Creating Manual Texture
///
/// You can also create a Manual Texture by using the second load function,
/// indicating a SoftwarePixelBuffer ( wich can contains only PixelFormat
/// but with null data ).
///
/// ### Implementation Notes
///
/// The Texture should keep a trace of the SoftwarePixelBuffer used to
/// construct it only if the 'keepCache' flag is true. Also, the Texture
/// should be able to upload new data to the buffer, and also change its
/// size dynamically.
///
/// The Texture should be rendered by the following pass :
///   + Activate Texture Unit Desired ( @see Material )
///   + _bind () , Set Shader Constant .
///
/// The Shader may or may not have the correct constant to have this
/// texture. If the Texture Unit is for example 48 , the texture will be
/// bound to sampler iTexture48 .
///
/// @note
/// As the TextureType::CubeMap may have up to six textures, the Texture
/// object stores a list of SoftwarePixelBuffer.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Texture : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Texture ( const std::string& name , const TextureType & type , const SoftwarePixelBufferHolderList& buffers ,
              bool keepCache = true ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Texture () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the first PixelBuffer used by this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelBuffer ( const SoftwarePixelBufferUser& pixelbuffer ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first PixelBuffer for this Texture, if has one.
    //////////////////////////////////////////////////////////////////////
    virtual const SoftwarePixelBufferUser getPixelBuffer () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface representing this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual const Surface& getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture's type.
    //////////////////////////////////////////////////////////////////////
    virtual const TextureType & getType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes current parameter for the Texture object.
    //////////////////////////////////////////////////////////////////////
    virtual void setParameterValue ( const TextureParameter & param , const Variant & value ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the value of given parameter.
    //////////////////////////////////////////////////////////////////////
    virtual const Variant & getParameterValue ( const TextureParameter & param ) const ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should bind the Texture to the correct target.
    //////////////////////////////////////////////////////////////////////
    virtual void _bind () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should unbind the Texture from its target.
    //////////////////////////////////////////////////////////////////////
    virtual void _unbind () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the GPU texture buffer.
    //////////////////////////////////////////////////////////////////////
    virtual void _setBuffer ( ) const = 0 ;
    
protected:
    
    /// @brief Holds the Texture's type.
    TextureType iType;
    
    /// @brief The SoftwarePixelBuffer list, if has one.
    SoftwarePixelBufferHolderList iPixelBuffers;
    
    /// @brief Surface for this Texture.
    Surface iSurface;
    
    /// @brief True if binded, false otherwise.
    mutable bool iBinded;
    
    /// @brief Parameters map.
    std::map < TextureParameter , Variant > iParameters ;
};

/// @brief SpecializedCountedObjectHolder for TexturePrivate.
typedef SpecializedCountedObjectHolder<Texture> TextureHolder;

/// @brief SpecializedResourceHolderList for TexturePrivate.
typedef SpecializedResourceHolderList<Texture> TextureHolderList;

/// @brief SpecializedResourceUser for TexturePrivate.
typedef SpecializedCountedObjectUser<Texture> TextureUser ;

/// @brief std::vector for Texture.
typedef std::list<TextureUser> TextureUserList;

//////////////////////////////////////////////////////////////////////
/// @brief Should loads an Image to a SoftwarePixelBuffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TextureFileLoader : public ResourceLoader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TextureFileLoader () ;
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~TextureFileLoader () noexcept ( false ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a file to the given SoftwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual SoftwarePixelBufferHolder load ( const std::string & filepath ) const = 0 ;
};

/// @brief ResourceLoaderFactory for TextureLoader.
typedef ResourceLoaderFactory<TextureFileLoader> TextureFileLoaderFactory;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TextureInternalCreator
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TextureInternalCreator () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TextureInternalCreator () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Texture* create (const std::string& name , const TextureType& type ,
                             const SoftwarePixelBufferHolderList& buflist ) const = 0 ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages Texture objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TextureManager : public SpecializedResourceManager<Texture, TextureFileLoader>
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TextureManager( const std::string & name = "DefaultTextureManager" );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TextureManager() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a file.
    //////////////////////////////////////////////////////////////////////
    virtual TextureUser load (const std::string & name , const TextureType & type ,
                              const std::string & filepath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a SoftwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual TextureUser load (const std::string & name , const TextureType & type ,
                              const SoftwarePixelBufferHolder& buffer ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setInternalCreator ( TextureInternalCreator* creator ) ;
    
protected:
    
    /// @brief An internal creator wich should be set by the Renderer.
    TextureInternalCreator* iCreator ;
};

/// @brief SpecializedCountedObjectHolder for TextureManager .
typedef SpecializedCountedObjectHolder < TextureManager > TextureManagerHolder ;

GreEndNamespace

#endif
