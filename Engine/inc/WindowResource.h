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
/// @brief A Window Object.
///
/// A Window is a specific RenderTarget, which should also contains
/// a RenderContext. The Window should also provides handling of the
/// Hardware Keyboard when focused, and Hardware Mouse.
///
/// The Window object is highly platform-dependent, and should be
/// different for each platforms.
///
/// In order to draw onto this Window, you should select a SceneManager
/// using RenderTarget::selectScene(), and then register this RenderTarget
/// to the RendererDrawLoop using ResourceManager::getRendererDrawLoop().registerRenderTarget().
/// Also, don't forget to associate a RenderContext using RenderTarget::setRenderContext().
/// Remember this RenderContext must be created by the Renderer.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowPrivate : public RenderTargetPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a WindowPrivate object.
    //////////////////////////////////////////////////////////////////////
    WindowPrivate (const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the WindowPrivate.
    //////////////////////////////////////////////////////////////////////
    virtual ~WindowPrivate() noexcept;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Treat Event in the Event Queue, if it has one.
    /// @return True if an Event has been treated. The return value of this
    /// function is not an Error check.
    //////////////////////////////////////////////////////////////////////
    virtual bool pollEvent();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return True if Window is not opened.
    //////////////////////////////////////////////////////////////////////
    virtual bool isClosed() const;
    
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
    virtual bool isExposed() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a RenderContext
    /// and should be drawed by the Renderer during the first phase.
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderContext(const RenderContext& renderCtxt);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContextHolder getRenderContext();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextHolder getRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a loop behaviour function.
    //////////////////////////////////////////////////////////////////////
    virtual void addLoopBehaviour(LoopBehaviour behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Erases every Loop Behaviours.
    //////////////////////////////////////////////////////////////////////
    virtual void clearLoopBehaviour();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Window has been exposed, then closed.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasBeenClosed() const;
    
protected:
    
    /// @brief Window's title.
    std::string iTitle;
    
    /// @brief Surface used by the Window.
    Surface iSurface;
    
    /// @brief True if Window is exposed.
    bool iExposed;
    
    /// @brief True if Window is closed.
    bool iClosed;
    
    /// @brief Holds the RenderContext linked to this Window.
    RenderContextHolder iRenderContext;
    
    /// @brief Helper object to hold LoopBehaviour functions.
    LoopBehaviours iLoopBehaviours;
    
    /// @brief Last Update time.
    UpdateTime    iLastUpdate;
};

/// @brief SpecializedResourceHolder for WindowPrivate.
typedef SpecializedResourceHolder<WindowPrivate> WindowHolder;

/// @brief SpecializedResourceHolderList for WindowPrivate.
typedef SpecializedResourceHolderList<WindowPrivate> WindowHolderList;

GreEndNamespace
#endif
