//////////////////////////////////////////////////////////////////////
//
//  FrameBuffer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 25/01/2016.
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

#ifndef GRE_FrameBuffer_h
#define GRE_FrameBuffer_h

#include "Resource.h"
#include "Texture.h"

GreBeginNamespace

/// @brief Attachement possible to this FrameBuffer.
/// You can attach Texture objects to this FrameBuffer, then attach
/// the Texture object to a Material to use this Texture in the Engine.
enum class RenderFramebufferAttachement : int
{
    Color0 = 0,   Color1 = 1,   Color2 = 2,   Color3 = 3,   Color4 = 4,   Color5 = 5,
    Color6 = 6,   Color7 = 7,   Color8 = 8,   Color9 = 9,   Color10 = 10, Color11 = 11,
    Color12 = 12, Color13 = 13, Color14 = 14, Color15 = 15, Color16 = 16, Color17 = 17,
    Color18 = 18, Color19 = 19, Color20 = 20, Color21 = 21, Color22 = 22, Color23 = 23,
    Color24 = 24, Color25 = 25, Color26 = 26, Color27 = 27, Color28 = 28, Color29 = 29,
    Color30 = 30, Color31 = 31,
    
    // [...] Normally this value is up to Renderer::getCapacity(Capacity::MaxFrameBufferColorAttachement) .
    // We support up to 32 Color Attachement. Default value for OpenGl is 4.
    // Notes 'Renderer::getCapacity()' function is not yet implemented. You can use
    // 'RenderFramebuffer::getMaximumColorAttachement()' to get the same result.
  
    Depth = 32,
    Stencil = 33, 
    
    // Invalid value.
    Null
};

/// @brief Type of surface used for the Framebuffer in a particular
/// attachement.
enum class RenderFramebufferAttachementType
{
    /// @brief The attachement used is a Texture object (usually a TextureHolder).
    Texture = 0
};

//////////////////////////////////////////////////////////////////////
/// @brief A RenderFramebuffer.
///
/// Represents a buffer for a frame rendering. Usually, it is hold by a
/// RenderTarget. Also, that can be used by a Pass to blend muliples
/// Pass.
/// You can also render to a Framebuffer to create an image of the Render,
/// without using a Window.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderFramebuffer : public Resource
{
public:
    
    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebuffer(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual TextureUser getTextureAttachement(const RenderFramebufferAttachement& attachement);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual const TextureUser getTextureAttachement(const RenderFramebufferAttachement& attachement) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface associated with a given Attachement.
    //////////////////////////////////////////////////////////////////////
    virtual Surface getAttachementSurface(const RenderFramebufferAttachement& attachement) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attachs the given texture object to the given Attachement.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttachement(const RenderFramebufferAttachement& attachement, TextureHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Maximum Width a Framebuffer Attachement can
    /// have.
    //////////////////////////////////////////////////////////////////////
    virtual int getMaximumWidth() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Maximum Height a Framebuffer Attachement can
    /// have.
    //////////////////////////////////////////////////////////////////////
    virtual int getMaximumHeight() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Maximum number of Color Attachement a Framebuffer
    /// can have.
    //////////////////////////////////////////////////////////////////////
    virtual int getMaximumColorAttachementCount() const;
    
protected:
    
    /// @brief Defines a basic Attachement.
    struct Attachement
    {
        /// @brief The Attachement layer.
        RenderFramebufferAttachement attachement;
        
        /// @brief The Attachement type.
        RenderFramebufferAttachementType attachType;
        
        /// @brief The Surface associated with the Attachement.
        Surface surface;
        
        /// @brief Depending on attachType , can holds a Texture.
        TextureHolder texture;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        Attachement();
      
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        void reset();
      
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        bool isValid() const;
    };
    
    /// @brief std::map<> for Attachement.
    typedef std::map<RenderFramebufferAttachement , Attachement> AttachementMap;
    
    /// @brief Holds the Attachement objects.
    AttachementMap iAttachements;
};

/// @brief SpecializedCountedObjectHolder for RenderFramebuffer.
typedef SpecializedCountedObjectHolder<RenderFramebuffer> RenderFramebufferHolder;

/// @brief SpecializedResourceHolderList for RenderFramebuffer.
typedef SpecializedResourceHolderList<RenderFramebuffer> RenderFramebufferHolderList;

/// @brief SpecializedCountedObjectUser for RenderFramebuffer.
typedef SpecializedCountedObjectUser<RenderFramebuffer> RenderFramebufferUser;

//////////////////////////////////////////////////////////////////////
/// @brief Creates a RenderFramebuffer Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderFramebufferInternalCreator
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebufferInternalCreator () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebufferInternalCreator () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebuffer* load ( const std::string & name , const ResourceLoaderOptions& options ) const = 0 ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages the RenderFramebuffer loaded.
///
/// Note a RenderFramebufferConstructor object is needed to be installed
/// by the Renderer in order to make the possibility to create framebuffers.
/// Also, in certain implementations (OpenGl), a RenderContext must be
/// binded when creating framebuffers.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderFramebufferManager : public ResourceManagerBase < RenderFramebuffer >
{
public:
    
    POOLED ( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebufferManager ( const std::string & name = "DefaultRenderFramebufferManager" ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebufferManager () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new framebuffer given options.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder load ( const std::string & name , const ResourceLoaderOptions & options ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iCreator'.
    //////////////////////////////////////////////////////////////////////
    virtual void setInternalCreator ( RenderFramebufferInternalCreator* creator ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iCreator'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderFramebufferInternalCreator* getInternalCreator () const ;
    
protected:
    
    /// @brief Internal creator for RenderFramebuffer.
    RenderFramebufferInternalCreator* iCreator ;
};

/// @brief
typedef SpecializedCountedObjectHolder<RenderFramebufferManager> RenderFramebufferManagerHolder ;

GreEndNamespace

#endif
