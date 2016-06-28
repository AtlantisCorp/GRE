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
enum class RenderFramebufferAttachement
{
    Color = 0,
    //      [...] Normally this value is up to Renderer::getCapacity(Capacity::MaxFrameBufferColorAttachement) .
    //      We support up to 32 Color Attachement. Default value for OpenGl is 4.
    Depth = 32,
    Stencil = 33
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
class DLL_PUBLIC RenderFramebufferPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebufferPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual Texture getTextureAttachement(const RenderFramebufferAttachement& attachement);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual const Texture getTextureAttachement(const RenderFramebufferAttachement& attachement) const;
    
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
    };
    
    /// @brief std::map<> for Attachement.
    typedef std::map<RenderFramebufferAttachement , Attachement> AttachementMap;
    
    /// @brief Holds the Attachement objects.
    AttachementMap iAttachements;
};

/// @brief SpecializedResourceHolder for RenderFramebuffer.
typedef SpecializedResourceHolder<RenderFramebufferPrivate> RenderFramebufferHolder;

/// @brief SpecializedResourceHolderList for RenderFramebuffer.
typedef SpecializedResourceHolderList<RenderFramebufferPrivate> RenderFramebufferHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for RenderFramebuffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderFramebuffer : public SpecializedResourceUser<RenderFramebufferPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebuffer(const RenderFramebufferPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebuffer(const RenderFramebufferHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebuffer(const RenderFramebuffer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual Texture getTextureAttachement(const RenderFramebufferAttachement& attachement);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Texture attached to the given attachement.
    /// This Texture can be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual const Texture getTextureAttachement(const RenderFramebufferAttachement& attachement) const;
    
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
    
    /// @brief A Null FrameBuffer.
    static RenderFramebuffer Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for RenderFramebuffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderFramebufferLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderFramebufferLoader();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderFramebufferLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::FrameBuff.
    //////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a FrameBuffer.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual Resource* load(Resource::Type type, const std::string& name) const;
};

/// @brief ResourceLoaderFactory for RenderFramebufferLoader.
typedef ResourceLoaderFactory<RenderFramebufferLoader> RenderFramebufferLoaderFactory;

GreEndNamespace

#endif
