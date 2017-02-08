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

/// @brief Resumes the Texture's possible Types.
enum class TextureType
{
    OneDimension,
    TwoDimension,
    ThreeDimension,
    CubeMap,
    Null
};

/// @brief Translates the TextureType value to a String.
std::string TextureTypeToString(const TextureType& type);

/// @brief Translates the TextureType value to HdwProgVarType.
HdwProgVarType TextureTypeToHdwProgType(const TextureType& type);

//////////////////////////////////////////////////////////////////////
/// @brief Represents a Texture Resource object.
///
/// A Texture object should contains the data related to Texture's
/// objects, as the Images pixels.
///
/// Before being registered to the Renderer, it may holds SoftwarePixelBuffer's
/// objects to holds the pixels. The SoftwarePixelBuffer is then transfered
/// to the GPU by the Renderer.
///
/// Please notes you should load a Texture file to a pre-created Texture
/// object from the Renderer.
///
/// Example ( Loading file 'textures/wood.png' ) :
///
/// TextureHolder texture = myrenderer.loadTexture("MyWoodTexture", "textures/wood.png");
///
/// Other example ( Same file ) :
///
/// TextureHolder texture = myrenderer.createTexture("MyWoodTexture");
/// Texture textureuser = myrenderer.getTextureManager().loadTexture(texture, "textures/wood.png");
///
/// Other example ( Same file ) :
///
/// Texture textureuser = myrenderer.getTextureManager().loadTexture("MyWoodTexture", "textures/wood.png");
///
/// Using the TextureManager from the Renderer can be useful to have a Texture
/// user. Using the Renderer to load the Texture lets you with a TextureHolder,
/// but the Texture is still registered in the Renderer's Texture Manager.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Texture : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Texture(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Texture() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Texture using given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void bindWithTextureUnit ( int texunit ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Texture using activated Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the Texture from activated Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activate given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void activateTextureUnit ( int texunit ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the first PixelBuffer used by this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void setPixelBuffer(const SoftwarePixelBufferUser& pixelbuffer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first PixelBuffer for this Texture, if has one.
    //////////////////////////////////////////////////////////////////////
    virtual const SoftwarePixelBufferUser getPixelBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first PixelBuffer for this Texture, if has one.
    //////////////////////////////////////////////////////////////////////
    virtual SoftwarePixelBufferUser getPixelBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Original file, if one was provided.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string& getOriginalFile() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface representing this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual const Surface& getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface representing this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual Surface& getSurface();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Surface representing this Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void setSurface(const Surface& surface);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture's type.
    //////////////////////////////////////////////////////////////////////
    virtual const TextureType getType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the Texture's type.
    //////////////////////////////////////////////////////////////////////
    virtual void setType(TextureType textype);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if it holds an HardwareSampler activated.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasHardwareSamplerActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the activated HardwareSampler.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareSamplerUser getHardwareSampler();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Configure the given HardwareProgram to have this Texture
    /// binded to the given TextureUnit .
    //////////////////////////////////////////////////////////////////////
    virtual void configureProgram ( const HardwareProgramHolder & program , int textureunit ) const ;
    
protected:
    
    /// @brief Holds the Texture's type.
    TextureType iType;
    
    /// @brief True if binded, false otherwise.
    mutable bool iBinded;
    
    /// @brief True if something changed and not updated, false otherwise.
    mutable bool iDirty;
    
    /// @brief If has one, the original filepath.
    std::string iOriginFile;
    
    /// @brief The SoftwarePixelBuffer list, if has one.
    SoftwarePixelBufferHolderList iPixelBuffers;
    
    /// @brief Surface for this Texture.
    Surface iSurface;
    
    /// @brief HardwareSampler activated for this Texture.
    HardwareSamplerHolder iSampler;
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
/// @brief ResourceLoader base class to load TexturePrivate objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TextureLoader : public ResourceLoader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TextureLoader();
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~TextureLoader();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a file.
    /// The resulting Texture only has SoftwarePixelBuffer, and cannot be
    /// used to render objects.
    ////////////////////////////////////////////////////////////////////////
    virtual TextureHolder load(const std::string& name, const std::string& filepath) const = 0;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a file to a pre-created Texture Object.
    ////////////////////////////////////////////////////////////////////////
    virtual TextureHolder load(TextureHolder& to, const std::string& filepath) const = 0;
};

/// @brief ResourceLoaderFactory for TextureLoader.
typedef ResourceLoaderFactory<TextureLoader> TextureLoaderFactory;

//////////////////////////////////////////////////////////////////////
/// @brief Manages Texture objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TextureManager : public SpecializedResourceManager<Texture, TextureLoader>
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TextureManager();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TextureManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a file.
    //////////////////////////////////////////////////////////////////////
    virtual TextureUser load(const std::string& name, const std::string& filepath) = 0 ;
};

/// @brief SpecializedCountedObjectHolder for TextureManager .
typedef SpecializedCountedObjectHolder < TextureManager > TextureManagerHolder ;

GreEndNamespace

#endif
