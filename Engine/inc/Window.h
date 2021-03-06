//////////////////////////////////////////////////////////////////////
//
//  Window.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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

#include "Resource.h"
#include "RenderContext.h"
#include "RenderTarget.h"
#include "Variant.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines the Window Style the user can use when setting the
/// WindowOptions .
enum class WindowStyle : int
{
    /// @brief Displays none of the peripheral elements.
    Borderless ,

    /// @brief Displays the title bar.
    Titled ,

    /// @brief Displays a close button.
    Closable ,

    /// @brief Displays a miniature button.
    Miniaturizable ,

    /// @brief Displays a resize control.
    Resizable ,

    /// @brief Makes the created window fullscreen.
    Fullscreen
};

/// @brief Holds a set of WindowStyles. Notes those styles may not be compatible
/// between each other. You can set those styles in the window loading options under
/// the 'Styles' key.
typedef std::vector < WindowStyle > WindowStyles ;

//////////////////////////////////////////////////////////////////////
/// @brief Defines Window's Context Attributes.
/// Uses those attributes in the WindowContextAttributes vector to set
/// options for the context created by the window.
enum WindowContextAttribute
{
    /// @brief Indicates double-buffering should be used. (boolean)
    WCADoubleBuffer = 0 ,

    /// @brief Indicates only stereo pixel formats are accepted. (boolean)
    WCAStereo ,

    /// @brief Indicates only pixel formats in fullscreen mode are accepted. (boolean)
    WCAFullscreen ,

    /// @brief Indicates only Hardware-Accelerated Renderers are accepted. (boolean)
    WCAAccelerated ,

    /// @brief Indicates only Multisampled Renderers are accepted. (boolean)
    WCAMultiSample ,

    /// @brief Indicates the number of Multisamples buffers that should be used. (int)
    WCASampleBuffers ,

    /// @brief Indicates the number of Multisamples that should be used. (int)
    WCASamples ,

    /// @brief Indicates Major version mininum required for the pixel format. (int)
    WCAMajorVersion ,

    /// @brief Indicates Minor version minimum required for the pixel format. (int)
    WCAMinorVersion ,

    /// @brief Indicates Depth component buffer size. (int)
    WCADepthSize ,

    /// @brief Indicates Color component buffer size. (int)
    WCAColorSize ,

    /// @brief Indicates Stencil component buffer size. (int)
    WCAStencilSize ,

    /// @brief Indicates SuperSampling is preferred over multisampling. (boolean)
    WCASuperSample ,

    /// @brief Indicates Triple buffering should be enabled. (boolean)
    WCATripleBuffer ,
    
    /// @brief Indicates Red Color size. (int) Default value is 8.
    WCAColorRedSize ,
    
    /// @brief Indicates Green Color size. (int) Default value is 8.
    WCAColorGreenSize ,
    
    /// @brief Indicates Blue Color size. (int) Default value is 8.
    WCAColorBlueSize ,
    
    /// @brief Indicates Alpha Color sie. (int) Default value is 8.
    WCAColorAlphaSize
};

/// @brief Defines a set of Context's Attributes.
typedef std::vector < WindowContextAttribute > WindowContextAttributes ;

//////////////////////////////////////////////////////////////////////
/// @brief A Window Object.
///
/// A Window is a specific RenderTarget normally visible by the Holder on
/// the Screen .
///
/// ### Subclassing 'Gre::Window'
///
/// The 'Gre::Application' object is responsible for sending 'UpdateEvent'
/// to the 'WindowManger' . This Manager sends events to every Window
/// registered.
///
/// In consequence , one should always overwrite the 'Gre::Resource::onUpdateEvent()'
/// in order to process every updates things , like polling events , and
/// others. He should also call 'WindowPrivate::onUpdateEvent()' at the
/// end of the function.
///
/// ### Window's loading options
///
/// Those options should be used accordingly when loading a new Window
/// object.
///
///  - 'Loader.Name' : Loader's name. If empty , default loader will be
/// used instead.
///  - 'Window.Title' : Window's title (std::string).
///  - 'Window.Size' : Window's size (std::string). Format should be
/// 'WidthxHeight' or 'WidthXHeight'.
///  - 'Window.Styles' : Window's styles (WindowStyles).
///  - 'Window.ContextAttributes' : WIndow's Context Attributes (WindowContextAttributes).
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Window : public RenderTarget
{
public:

    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Window ( const std::string& name , const ResourceLoaderOptions & options = ResourceLoaderOptions () ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Window() noexcept(false);

    //////////////////////////////////////////////////////////////////////
    /// @brief Set a new title for this Window.
    //////////////////////////////////////////////////////////////////////
    virtual void setTitle(const std::string& title);

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a description of the Surface this Window covers.
    //////////////////////////////////////////////////////////////////////
    virtual Surface getSurface() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Hides/Unhides the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void toggleCursor ( bool hidden ) = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Centers the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void centerCursor ( ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets to true if you want this Window to center the Cursor
    /// at each update.
    //////////////////////////////////////////////////////////////////////
    virtual void setCursorCentered ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual bool isFocused () const ;

public: // Visibility Functions .

    //////////////////////////////////////////////////////////////////////
    /// @brief Return True if Window has been destroyed .
    //////////////////////////////////////////////////////////////////////
    virtual bool isClosed() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Window is currently visible .
    //////////////////////////////////////////////////////////////////////
    virtual bool isExposed() const;

public: // RenderFramebuffer Functions .

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'RenderFramebuffer ( nullptr )' .
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder getFramebuffer();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'RenderFramebuffer ( nullptr )' .
    //////////////////////////////////////////////////////////////////////
    virtual const RenderFramebufferHolder getFramebuffer() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Do nothing.
    //////////////////////////////////////////////////////////////////////
    virtual void bindFramebuffer() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Do nothing.
    //////////////////////////////////////////////////////////////////////
    virtual void unbindFramebuffer() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns false .
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsFramebuffer() const;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Internal function to set a title to the Window.
    //////////////////////////////////////////////////////////////////////
    virtual void _setTitle ( const std::string& title ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// [thread-safe]
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);

protected:

    /// @brief Window's title.
    std::string iTitle;

    /// @brief Surface used by the Window.
    Surface iSurface;

    /// @brief True if Window is mapped on the screen.
    bool iExposed;

    /// @brief True if Window is destroyed.
    bool iClosed;

    /// @brief True if the Window has the focus.
    bool iFocused;

    /// @brief True if this Window has to center the Cursor at each update.
    bool iCenterCursor ;
};

/// @brief Holder for WindowPrivate.
typedef Holder < Window > WindowHolder ;

/// @brief SpecializedResourceHolderList for WindowPrivate.
typedef SpecializedResourceHolderList < Window > WindowHolderList ;

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for WindowPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowLoader : public ResourceLoader
{
public:

    POOLED(Pools::Loader)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowLoader();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~WindowLoader();

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Window.
    //////////////////////////////////////////////////////////////////////
    virtual WindowHolder load ( const std::string& name , const ResourceLoaderOptions & info ) const = 0;
};

/// @brief ResourceLoaderFactory for WindowLoader.
typedef ResourceLoaderFactory<WindowLoader> WindowLoaderFactory;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceManager for Window.
///
/// The WindowManager is only responsible for polling events from the
/// platform and from the Engine, thus transmetting it correctly to
/// every Windows.
///
/// You can use the WindowManager to listen to every Key events. To
/// achieve this purpose, use 'addGlobalKeyListener()' . The manager
/// use and internal Listener which listens to every Window.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowManager : public SpecializedResourceManager< Window , WindowLoader >
{
public:

    POOLED ( Pools::Manager )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowManager ( const std::string & name = "DefaultWindowManager" ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~WindowManager ( ) noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Window given its parameters.
    //////////////////////////////////////////////////////////////////////
    virtual WindowHolder load ( const std::string & name , const ResourceLoaderOptions & info ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Listener to the Global Key Listener.
    //////////////////////////////////////////////////////////////////////
    virtual void addGlobalKeyListener ( const EventProceederHolder & proceeder ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Polls Event and returns when an event has been treated.
    //////////////////////////////////////////////////////////////////////
    virtual void pollEvents ( const Duration& elapsed ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Closes every Window presents in this Manager.
    //////////////////////////////////////////////////////////////////////
    virtual void closeWindows ( ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Filters events received by this manager. If the emitter is one
    /// of the managed windows, it will only accept WindowWillCloseEvent.
    ////////////////////////////////////////////////////////////////////////
    virtual void onEvent ( EventHolder & holder ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Implementation specific poll events.
    //////////////////////////////////////////////////////////////////////
    virtual void _pollEvents () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);

    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Window Object will be closed.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowWillCloseEvent ( const WindowWillCloseEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the given proceeder in the window list.
    //////////////////////////////////////////////////////////////////////
    virtual bool _findWindow ( const EventProceeder* window ) const ;

protected:

    /// @brief Listens to every Windows.
    EventProceederHolder iGlobalListener ;

    /// @brief True if this WindowManager has already launched the events.
    mutable bool iEventLaunched ;
};

/// @brief Holder for WindowManager Resource .
typedef Holder < WindowManager > WindowManagerHolder ;

GreEndNamespace
#endif /* defined(__GResource__Window__) */
