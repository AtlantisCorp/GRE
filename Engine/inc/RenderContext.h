//////////////////////////////////////////////////////////////////////
//
//  RenderContext.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 12/02/2016.
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

#ifndef GRE_RenderContext_h
#define GRE_RenderContext_h

#include "RenderContextInfo.h"
#include "Resource.h"
#include "Viewport.h"
#include "FrameBuffer.h"
#include "Color.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief List the Buffer's bits the RenderContext can clear between
/// each render.
/// @see https://www.opengl.org/sdk/docs/man2/xhtml/glClear.xml for
/// meaning of those values. Notes those values are from OpenGl.
//////////////////////////////////////////////////////////////////////
enum class RenderContextClearBuffer
{
    ColorBufferBit ,
    DepthBufferBit ,
    AccumBufferBit ,
    StencilBufferBit
};

/// @brief Array of RenderContextClearBuffer.
typedef std::vector < RenderContextClearBuffer > RenderContextClearBuffers;

//////////////////////////////////////////////////////////////////////
/// @brief Defines a Context which, when binded, enables the Renderer
/// to draw on it.
///
/// A RenderContext can be defined as the representation of a Surface
/// where the Renderer object can render. Implementation - specific, it
/// should also permit the Rendering API to store objects in the GPU
/// memory.
///
/// OpenGl implementation theory
/// The OpenGl implementation should create a RenderContext for each
/// Window. Also, a RenderContext should be used to stores objects in the
/// GPU memory. This method should develop multithreading calls.
/// Example : A Window holds a RenderContext. The Renderer also holds
/// a RenderContext. Every RenderContext created should be shared with
/// the Renderer's one. Writing to the GPU uses the Global RenderContext,
/// Reading the GPU's memory is done by the RenderContext of the Window.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderContext : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContext ( const std::string& name ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContext ( const std::string& name , const RenderContextInfo& ctxtInfo ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderContext() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContextInfo's object.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextInfo& getInfo() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this object is binded.
    //////////////////////////////////////////////////////////////////////
    virtual bool isBinded() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void flush() = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Viewport in this Context.
    //////////////////////////////////////////////////////////////////////
    virtual void addViewport(const Viewport& viewport);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Viewport in this RenderContext.
    /// For the last parameters, if a negative value is given, the opposite
    /// value is set.
    //////////////////////////////////////////////////////////////////////
    virtual void createViewport(const std::string& name, float topratio = 0.0f, float leftratio = 0.0f, float widthratio = 1.0f, float heightratio = 1.0f, bool activated = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Viewports.
    //////////////////////////////////////////////////////////////////////
    virtual const ViewportList& getViewports() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default Viewport, aka the first one.
    //////////////////////////////////////////////////////////////////////
    virtual Viewport getDefaultViewport() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void removeViewport(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a description of this RenderContext.
    /// This function can be overloaded by a subclass in order to provide
    /// other infos to the user.
    //////////////////////////////////////////////////////////////////////
    virtual std::string toString() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffers the RenderContext should clear at each
    /// loop. Buffers available for clearing are listed in 'RenderContextClearBuffer'
    /// enum. Notes one instance of any RenderContextClearBuffer should be listed.
    //////////////////////////////////////////////////////////////////////
    virtual void setClearBuffers ( const RenderContextClearBuffers& clearbuffs ) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a Color used when clearing the ColorBufferBit.
    //////////////////////////////////////////////////////////////////////
    virtual void setClearColor ( const Color& color ) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new RenderFramebuffer for this RenderContext. Also
    /// appends this RenderFramebuffer to the RenderFramebuffer's list.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder createFramebuffer ( const std::string& name = std::string() ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of RenderFramebuffers. This list is populated
    /// with newly-created RenderFramebuffer if its size is not big enough.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolderList getFramebuffers ( int sz ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method that creates a RenderFramebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder iCreateFramebuffer ( const std::string& name ) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method to updates Viewport's Borders .
    //////////////////////////////////////////////////////////////////////
    virtual void iUpdatesViewportsBorders ( const Surface& surface ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has attached a new RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowAttachContextEvent ( const WindowAttachContextEvent& e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object has sized its surface .
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowSizedEvent ( const WindowSizedEvent& e ) ;
    
protected:
    
    /// @brief Holds some info about this Context.
    RenderContextInfo iContextInfo;
    
    /// @brief True if this Context is currently binded.
    mutable bool iIsBinded;
    
    /// @brief Holds the Viewport objects.
    /// Those objects should be updated when the Window object changes its size,
    /// using Listener/Emitter system to handle EventType::WindowSizeChanged.
    ViewportList iViewports;
    
    /// @brief Holds every RenderFramebuffer created for this RenderContext.
    RenderFramebufferHolderList iFramebuffers;
};

/// @brief SpecializedCountedObjectHolder for RenderContext.
typedef SpecializedCountedObjectHolder<RenderContext> RenderContextHolder;

/// @brief SpecializedResourceHolderList for RenderContext.
typedef SpecializedResourceHolderList<RenderContext> RenderContextHolderList;

/// @brief SpecializedCountedObjectUser.
typedef SpecializedCountedObjectUser<RenderContext> RenderContextUser;

////////////////////////////////////////////////////////////////////////
/// @brief Manages RenderContext objects.
/// This Manager is created by the Renderer you are using. You can load
/// a RenderContext directly using 'RenderContextManager::load()'.
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderContextManager : public ResourceManagerBase < RenderContext >
{
public:
    
    POOLED ( Pools::Manager )
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    RenderContextManager ( ) ;
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    RenderContextManager ( const std::string & name ) ;
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~RenderContextManager ( ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a new RenderContext with given name.
    ////////////////////////////////////////////////////////////////////////
    virtual RenderContextUser load ( const std::string & name , const RenderContextInfo & info ) = 0 ;
} ;

/// @brief SpecializedCountedObjectHolder for RenderContextManager .
typedef SpecializedCountedObjectHolder< RenderContextManager > RenderContextManagerHolder ;

GreEndNamespace

#endif
