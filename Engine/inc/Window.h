//////////////////////////////////////////////////////////////////////
//
//  Window.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 07/11/2015.
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

#ifndef __GResource__Window__
#define __GResource__Window__

#include "WindowResource.h"

GreBeginNamespace

class Renderer;

typedef std::pair<int, int> WindowSize;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for WindowPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Window : public RenderTarget, public SpecializedResourceUser<WindowPrivate>
{
public:
    
    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a Window from pointer.
    //////////////////////////////////////////////////////////////////////
    Window (const WindowPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a Window from holder.
    //////////////////////////////////////////////////////////////////////
    Window (const WindowHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a Window from user.
    //////////////////////////////////////////////////////////////////////
    Window (const Window& user);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the Window.
    //////////////////////////////////////////////////////////////////////
    virtual ~Window();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder<WindowPrivate> lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const SpecializedResourceHolder<WindowPrivate> lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Treat Event in the Event Queue, if it has one.
    /// @return True if an Event has been treated. The return value of this
    /// function is not an Error check.
    //////////////////////////////////////////////////////////////////////
    bool pollEvent();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return True if Window is not opened.
    //////////////////////////////////////////////////////////////////////
    bool isClosed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Recommended Renderer name for this Object.
    ///
    /// @note This function is deprecated, because now it is the Renderer
    /// Object which create the RenderContext, and then link it to the Window
    /// Object. If the RenderContext Object is not compatible with the Window
    /// system, the function Window::setRenderContext() will throw an
    /// exception.
    //////////////////////////////////////////////////////////////////////
    const std::string recommendedRenderer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set a new title for this Window.
    //////////////////////////////////////////////////////////////////////
    void setTitle(const std::string& title);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a description of the Surface this Window covers.
    //////////////////////////////////////////////////////////////////////
    Surface getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called after pollEvent.
    //////////////////////////////////////////////////////////////////////
    void update();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Window is visible by the User.
    /// @note Window::isExposed() and Window::isClosed() can return the same
    /// value (false) if Window is minimized.
    //////////////////////////////////////////////////////////////////////
    bool isExposed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return true if a RenderContext is linked with this Window.
    //////////////////////////////////////////////////////////////////////
    bool hasRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    void setRenderContext(const RenderContext& renderCtxt);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    RenderContextHolder getRenderContext();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    const RenderContextHolder getRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a loop behaviour function.
    //////////////////////////////////////////////////////////////////////
    void addLoopBehaviour(LoopBehaviour behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Erases every Loop Behaviours.
    //////////////////////////////////////////////////////////////////////
    void clearLoopBehaviour();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Window has been exposed, then closed.
    //////////////////////////////////////////////////////////////////////
    bool hasBeenClosed() const;
    
    /// @brief A Null Window object.
    static Window Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for WindowPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    WindowLoader();
    virtual ~WindowLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

typedef ResourceLoaderFactory<WindowLoader> WindowLoaderFactory;

/// @brief SpecializedResourceManager for WindowPrivate.
typedef SpecializedResourceManager<WindowPrivate> WindowManager;

GreEndNamespace
#endif /* defined(__GResource__Window__) */
