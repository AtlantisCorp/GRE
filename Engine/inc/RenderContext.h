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

GreBeginNamespace

class WindowSizedEvent;

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
class DLL_PUBLIC RenderContextPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContextPrivate(const std::string& name, const RenderContextInfo& ctxtInfo);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderContextPrivate();
    
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
    virtual void bind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    virtual void unbind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void flush();
    
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
    /// @brief Treats Events then calls Actions.
    //////////////////////////////////////////////////////////////////////
    virtual void onEvent(const Event& e);
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when the attached Window changed its size.
    //////////////////////////////////////////////////////////////////////
    virtual void _onWindowSizedEvent(WindowSizedEvent& event);
    
protected:
    
    /// @brief Holds some info about this Context.
    RenderContextInfo iContextInfo;
    
    /// @brief True if this Context is currently binded.
    bool iIsBinded;
    
    /// @brief Holds the Viewport objects.
    /// Those objects should be updated when the Window object changes its size,
    /// using Listener/Emitter system to handle EventType::WindowSizeChanged.
    ViewportList iViewports;
};

/// @brief SpecializedResourceHolder for RenderContext.
typedef SpecializedResourceHolder<RenderContextPrivate> RenderContextHolder;

/// @brief SpecializedResourceHolderList for RenderContext.
typedef SpecializedResourceHolderList<RenderContextPrivate> RenderContextHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for RenderContextPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderContext : public SpecializedResourceUser<RenderContextPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContext(const RenderContextPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContext(const RenderContextHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderContext(const RenderContext& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~RenderContext();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContextInfo's object.
    //////////////////////////////////////////////////////////////////////
    const RenderContextInfo& getInfo() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this object is binded.
    //////////////////////////////////////////////////////////////////////
    bool isBinded() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderContext.
    //////////////////////////////////////////////////////////////////////
    void bind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    void unbind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    void flush();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Viewport in this Context.
    //////////////////////////////////////////////////////////////////////
    void addViewport(const Viewport& viewport);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Viewport in this RenderContext.
    /// For the last parameters, if a negative value is given, the opposite
    /// value is set.
    //////////////////////////////////////////////////////////////////////
    void createViewport(const std::string& name, float topratio = 0.0f, float leftratio = 0.0f, float widthratio = 1.0f, float heightratio = 1.0f, bool activated = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Viewports.
    //////////////////////////////////////////////////////////////////////
    const ViewportList& getViewports() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Viewport.
    //////////////////////////////////////////////////////////////////////
    void removeViewport(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a description of this RenderContext.
    //////////////////////////////////////////////////////////////////////
    std::string toString() const;
    
    /// @brief A Null typed RenderContext.
    static RenderContext Null;
};

GreEndNamespace

#endif
