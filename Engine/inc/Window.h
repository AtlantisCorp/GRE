//
//  Window.h
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef __GResource__Window__
#define __GResource__Window__

#include "Resource.h"
#include "WindowResource.h"

GreBeginNamespace

class Renderer;

typedef std::pair<int, int> WindowSize;

//////////////////////////////////////////////////////////////////////
/// @brief Host for a WindowResource object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Window : public ResourceUser, public RenderTarget
{
public:
    
    Window ();
    Window (Window&& rmove);
    Window (const Window& window);
    explicit Window (const ResourceUser& ruser);
    
    virtual ~Window();
    
    Window& operator = (const ResourceUser& ruser);
    Window& operator = (const Window& wuser);
    
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
    RenderContext& getRenderContext();
    const RenderContext& getRenderContext() const;
    
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
    
private:
    
    std::weak_ptr<WindowResource> _mWindow;
};

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

GreEndNamespace
#endif /* defined(__GResource__Window__) */
