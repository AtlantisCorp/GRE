//////////////////////////////////////////////////////////////////////
//
//  RenderTarget.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 15/02/2016.
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

#ifndef GRE_RenderTarget_h
#define GRE_RenderTarget_h

#include "Pools.h"
#include "Resource.h"
#include "RenderContext.h"
#include "FrameBuffer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief An Object where the Renderer can render a Scene on it.
///
/// A RenderTarget can send some specific Events :
///
/// - RenderTargetWillClose : Normally sent went the RenderTarget is
/// about to be destroyed. The Renderer should listen this Event in order
/// to unregister the RenderTarget.
/// - RenderTargetClosed : Normally sent went the RenderTarget is already
/// closed.
/// - RenderTargetChangedRenderContext : Should be sent when the RenderTarget
/// changed its RenderContext.
/// - RenderTargetChangedFramebuffer : Should be sent when the RenderTarget
/// changed its RenderFramebuffer.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderTarget : public Resource
{
public:
    
    POOLED(Pools::Render)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a RenderTarget.
    //////////////////////////////////////////////////////////////////////
    RenderTarget(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderTarget() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderTarget.
    /// In case this RenderTarget has a RenderContext object, this function
    /// may change the current RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Swap buffers if this render target has a render context.
    //////////////////////////////////////////////////////////////////////
    virtual void swapBuffers () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If has one, should bind the internal Framebuffer.
    /// This method is used by the Renderer in order to be able to draw the
    /// result of the blended Pass'es objects in a custom Framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void bindFramebuffer() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If has one, unbind the internal Framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void unbindFramebuffer() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a RenderContext
    /// and should be drawed by the Renderer during the first phase.
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsRenderContext() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a special
    /// RenderFramebuffer to draw to.
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsFramebuffer() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the RenderTarget is ready to be drawn on.
    /// For example , this should be true for a Window object if the Window
    /// is directly visible by the user on the screen ( 'isVisible' and
    /// 'isOnActiveSpace' properties on macOs ).
    //////////////////////////////////////////////////////////////////////
    virtual bool isAvailableForDrawing () const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContextUser getRenderContext() = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextUser getRenderContext() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderFramebufferHolder this RenderTarget contains,
    /// or null.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferUser getFramebuffer() = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderFramebufferHolder this RenderTarget contains,
    /// or null.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderFramebufferUser getFramebuffer() const = 0;
};

/// @brief SpecializedCountedObjectHolder for RenderTargetPrivate.
typedef SpecializedCountedObjectHolder<RenderTarget> RenderTargetHolder;

/// @brief SpecializedResourceHolderList for RenderTargetPrivate.
typedef SpecializedResourceHolderList<RenderTarget> RenderTargetHolderList;

/// @brief SpecializedResourceUser for RenderTargetPrivate.
typedef SpecializedCountedObjectUser<RenderTarget> RenderTargetUser;

GreEndNamespace

#endif
