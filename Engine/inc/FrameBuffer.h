//////////////////////////////////////////////////////////////////////
//
//  FrameBuffer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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
#include "Viewport.h"
#include "Projection.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Buffers where the framebuffer can draw the fragments.
enum class RenderColorBuffer : int
{
    None , FrontLeft , FrontRight , BackLeft , BackRight , Front , Back ,
    Left , Right , FrontAndBack , Color0 , Color1 , Color2 , Color3 , Color4 ,
    Color5 , Color6 , Color7 , Color8
};

/// @brief Converts color buffer from string.
RenderColorBuffer RenderColorBufferFromString ( const std::string & str ) ;

/// @brief Attachement possible to this FrameBuffer.
/// You can attach Texture objects to this FrameBuffer, then attach
/// the Texture object to a Material to use this Texture in the Engine.
enum class RenderFramebufferAttachement : int
{
    Color0 = 0,   Color1 = 1,   Color2 = 2,   Color3 = 3,   Color4 = 4,   Color5 = 5,
    Color6 = 6,   Color7 = 7,   Color8 = 8,

    // [...] Normally this value is up to Renderer::getCapacity(Capacity::MaxFrameBufferColorAttachement) .
    // We support up to 9 Color Attachement. Default value for OpenGl is 4.
    // Notes 'Renderer::getCapacity()' function is not yet implemented. You can use
    // 'RenderFramebuffer::getMaximumColorAttachement()' to get the same result.

    Depth = 32,
    Stencil = 33,
    DepthStencil = 34 ,

    // Invalid value.
    Null
};

/// @brief Converts RenderFramebufferAttachement from a string.
RenderFramebufferAttachement RenderFramebufferAttachementFromString ( const std::string & str ) ;

/// @brief Converts RenderFramebufferAttachement to a string.
std::string RenderFramebufferAttachementToString ( const RenderFramebufferAttachement& layer ) ;

/// @brief Type of surface used for the Framebuffer in a particular
/// attachement.
enum class RenderFramebufferAttachementType
{
    /// @brief The attachement used is a Texture object (usually a TextureHolder).
    Texture ,

    /// @brief Attachement for Renderbuffers.
    Renderbuffer
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines basic proprties for a Renderbuffer.
struct Renderbuffer
{
    /// @brief Width and Height to initialize it.
    std::pair < int , int > size ;

    /// @brief Internal stored format.
    InternalPixelFormat format ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines a basic Attachement for a framebuffer.
struct FramebufferAttachment
{
    /// @brief The Attachement layer.
    RenderFramebufferAttachement layer;

    /// @brief The Attachement type.
    RenderFramebufferAttachementType type;

    /// @brief Valid if RenderFramebufferAttachementType is Texture.
    TextureHolder texture;

    /// @brief Valid if RenderFramebufferAttachementType is Renderbuffer.
    Renderbuffer renderbuffer ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    FramebufferAttachment();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void reset();
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
    /// @brief Returns true if the framebuffer is binded.
    //////////////////////////////////////////////////////////////////////
    virtual bool binded () const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the framebuffer is complete, i.e. it can be
    /// used to render on it, or read from it.
    //////////////////////////////////////////////////////////////////////
    virtual bool isComplete () const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the attachment.
    //////////////////////////////////////////////////////////////////////
    virtual FramebufferAttachment & getAttachment ( const RenderFramebufferAttachement & value ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the attachement for given key.
    //////////////////////////////////////////////////////////////////////
    virtual const FramebufferAttachment & getAttachment ( const RenderFramebufferAttachement & value ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the attachment for the framebuffer to given texture. As
    /// one can set an attachment during the rendering process, this method
    /// is constant.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttachment (const RenderFramebufferAttachement & value ,
                                const TextureHolder & texture ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the attachment for the framebuffer using a renderbuffer
    /// of given size. Notes if the size is different from other attachements
    /// size, the attachment will be discarded.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttachmentBuffer (const RenderFramebufferAttachement & value ,
                                      const std::pair < int , int > & size ,
                                      const InternalPixelFormat & format ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setWriteBuffer ( const RenderColorBuffer & buffer ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setReadBuffer ( const RenderColorBuffer & buffer ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setViewport ( const Viewport & viewport ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const Viewport & getViewport () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Viewport & getViewport () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default size.
    //////////////////////////////////////////////////////////////////////
    virtual const std::pair < int , int > & getDefaultSize () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clear the given attachment , resetting it to a default value.
    //////////////////////////////////////////////////////////////////////
    virtual void clear ( const RenderFramebufferAttachement & layer ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Resets the framebuffer. Also unbinds it if binded.
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the viewport's projection.
    //////////////////////////////////////////////////////////////////////
    virtual const Projection & getProjection () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Attaches the given attachment to the framebuffer. If the
    /// framebuffer was not binded , it should bind then unbind it.
    /// @return True on success, otherwise false.
    ///
    /// @notes If the attachment is a Renderbuffer , the framebuffer should
    /// set appropriate settings to create and manage it.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual bool bindAttachment ( const FramebufferAttachment & attachment ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the given attchament. If the attachment is a renderbuffer,
    /// destruction of this buffer should be done by the framebuffer implementation.
    //////////////////////////////////////////////////////////////////////
    virtual void unbindAttachment ( const FramebufferAttachment & attachment ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has sized its surface .
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowSizedEvent ( const WindowSizedEvent& e ) ;

protected:

    /// @brief Holds attachments to this framebuffer. The attachment can be a texture, or
    /// a renderbuffer. Notes this property is 'mutable' because one can attach a framebuffer
    /// during rendering, modifying the target buffers.
    mutable std::map < RenderFramebufferAttachement , FramebufferAttachment > iAttachments ;

    /// @brief Default size for this framebuffer. When setting an attachment, if the texture
    /// or renderbuffer has a different size, it will be discarded only if another attachment is
    /// already present. If none, the default size is set to the newly attached.
    mutable std::pair < int , int > iDefaultSize ;

    /// @brief Write buffer used.
    RenderColorBuffer iWriteBuffer ;

    /// @brief Read buffer used.
    RenderColorBuffer iReadBuffer ;

    /// @brief Viewport used by this framebuffer. When sets , the framebuffer will
    /// automatically push the current viewport and set this one when binding itself.
    Viewport iViewport ;
};

/// @brief Holder for RenderFramebuffer.
typedef Holder<RenderFramebuffer> RenderFramebufferHolder;

/// @brief SpecializedResourceHolderList for RenderFramebuffer.
typedef SpecializedResourceHolderList<RenderFramebuffer> RenderFramebufferHolderList;

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

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebuffer* loadNull () const = 0 ;
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
    /// @brief Returns the screen's framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder getNull () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a blank framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder loadBlank ( const std::string & name ) ;

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

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns first framebuffer encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder get ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns first framebuffer encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderFramebufferHolder get ( const std::string & name ) const ;

protected:

    /// @brief Internal creator for RenderFramebuffer.
    RenderFramebufferInternalCreator* iCreator ;
};

/// @brief
typedef Holder<RenderFramebufferManager> RenderFramebufferManagerHolder ;

GreEndNamespace

#endif
