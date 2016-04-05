//
//  WindowResource.h
//  GRE
//
//  Created by Jacques Tronconi on 15/11/2015.
//
//

#ifndef GRE_WindowResource_h
#define GRE_WindowResource_h

#include "Resource.h"
#include "RenderContext.h"
#include "LoopBehaviours.h"
#include "RenderTarget.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Window brief Object.
///
/// A Window should be a RenderTarget, but should also send Key Event,
/// Mouse Event, and take care about every Hardware or OS specific
/// related Events.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowResource : public Resource, public RenderTargetPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    WindowResource (const std::string& name);
    virtual ~WindowResource();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Treat Event in the Event Queue, if it has one.
    /// @return True if an Event has been treated. The return value of this
    /// function is not an Error check.
    //////////////////////////////////////////////////////////////////////
    virtual bool pollEvent();
    
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
    virtual const std::string recommendedRenderer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set a new title for this Window.
    //////////////////////////////////////////////////////////////////////
    virtual void setTitle(const std::string& title);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a description of the Surface this Window covers.
    //////////////////////////////////////////////////////////////////////
    virtual Surface getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called after pollEvent.
    //////////////////////////////////////////////////////////////////////
    virtual void update();
    
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
    virtual void setRenderContext(const RenderContext& renderCtxt);
    
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
    virtual bool hasBeenClosed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a RenderContext
    /// and should be drawed by the Renderer during the first phase.
    //////////////////////////////////////////////////////////////////////
    bool holdsRenderContext() const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by Window::setRenderContext() when a new RenderContext
    /// is linked to the Window.
    /// The object hold by _mRenderContext is the NEW RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void onRenderContextChanged();
    
protected:
    
    /// @brief Window's title.
    std::string _mTitle;
    
    /// @brief Surface used by the Window.
    Surface _mSurface;
    
    /// @brief True if Window is exposed.
    bool _mExposed;
    
    /// @brief True if Window is closed.
    bool _mClosed;
    
    /// @brief Holds the RenderContext linked to this Window.
    RenderContext _mRenderContext;
    
    /// @brief Helper object to hold LoopBehaviour functions.
    LoopBehaviours _mLoopBehaviours;
    
    UpdateTime    _lastUpdate;
};

GreEndNamespace
#endif
