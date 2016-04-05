//
//  OSXWindow.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#ifndef GRE_OSXWindow_h
#define GRE_OSXWindow_h

#include "OSXFunctions.h"

class DLL_PUBLIC OsXWindow : public WindowResource
{
public:
    
    /// @brief This contructor is the real one.
    /// WindowPrivateData is deprecated, as it does not contain any data and
    /// is not used by anything.
    /// Currently creates the Window with given parameter and show it on the screen.
    OsXWindow (const std::string & name, int x0, int y0, int wid, int height);
    
    /// @brief Destroys the Window.
    ~OsXWindow ();
    
    bool pollEvent();
    const std::string recommendedRenderer() const;
    void setTitle(const std::string& title);
    void swapBuffers ();
    void setVerticalSync (bool vsync);
    bool hasVerticalSync () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderTarget.
    /// In case this RenderTarget has a RenderContext object, this function
    /// may change the current RenderContext.
    //////////////////////////////////////////////////////////////////////
    void bind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the RenderTarget.
    //////////////////////////////////////////////////////////////////////
    void unbind();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If has one, should bind the internal Framebuffer.
    /// This method is used by the Renderer in order to be able to draw the
    /// result of the blended Pass'es objects in a custom Framebuffer.
    //////////////////////////////////////////////////////////////////////
    void bindFramebuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If has one, unbind the internal Framebuffer.
    //////////////////////////////////////////////////////////////////////
    void unbindFramebuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Window has been exposed, then closed.
    //////////////////////////////////////////////////////////////////////
    bool hasBeenClosed() const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by Window::setRenderContext() when a new RenderContext
    /// is linked to the Window.
    /// The object hold by _mRenderContext is the NEW RenderContext.
    //////////////////////////////////////////////////////////////////////
    void onRenderContextChanged();
    
private:
    
    /// @brief True if the Window object has been closed, after being exposed.
    bool _hasBeenClosed;
    
    /// @brief A reference to the Obj-C NsWindow object.
    CFTypeRef _nsWindow;
    
    
};

class DLL_PUBLIC OsXWindowLoader : public WindowLoader
{
public:
    
    OsXWindowLoader ();
    ~OsXWindowLoader ();
    
    ResourceLoader* clone() const;
    Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const;
};

#endif
