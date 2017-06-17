//////////////////////////////////////////////////////////////////////
//
//  RenderContext.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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
    virtual void flush() const = 0;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the surface where the context can draw. This surface
    /// may be an off-screen , on-screen or full-screen drawable object
    /// associated with this context.
    //////////////////////////////////////////////////////////////////////
    virtual Surface getSurface () const = 0 ;

protected:

    /// @brief True if this Context is currently binded.
    mutable bool iIsBinded;
};

/// @brief Holder for RenderContext.
typedef Holder<RenderContext> RenderContextHolder;

/// @brief SpecializedResourceHolderList for RenderContext.
typedef SpecializedResourceHolderList<RenderContext> RenderContextHolderList;

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
    virtual RenderContextHolder load ( const std::string & name , const RenderContextInfo & info ) = 0 ;
} ;

/// @brief Holder for RenderContextManager .
typedef Holder< RenderContextManager > RenderContextManagerHolder ;

GreEndNamespace

#endif
