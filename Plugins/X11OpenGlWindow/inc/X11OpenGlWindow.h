//////////////////////////////////////////////////////////////////////
//
//  X11OpenGlWindow.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 17/04/2017.
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
 
#ifndef GRE_X11OPENGLWINDOW_H
#define GRE_X11OPENGLWINDOW_H

#include "X11OpenGl.h"

#include <X11/extensions/xf86vmode.h>

//////////////////////////////////////////////////////////////////////
/// @brief A X11 Window using OpenGl driver (Generally implemented by
/// MESA). 
//////////////////////////////////////////////////////////////////////
class X11OpenGlWindow : public Gre::Window 
{
public:
	
    POOLED( Gre::Pools::Resource )
	
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a new X11OpenGlWindow object. The Window should
    /// then be initialized using 'initialize()' to create the real 
    /// Window. 
    //////////////////////////////////////////////////////////////////////
    X11OpenGlWindow ( const std::string& name , 
                      ::Display* dpy ,
                      ::Window win ,
                      ::GLXContext ctxt ,
                      ::Colormap cmap ,
                      ::XF86VidModeModeInfo deskmode ,
                      bool fs ,
                      ::Atom wmdelete ,
                      const std::string & title ,
                      const Gre::ResourceLoaderOptions& options ) ;
                      
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~X11OpenGlWindow () noexcept ( false ) ;
	
    //////////////////////////////////////////////////////////////////////
    /// @brief Hides/Unhides the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void toggleCursor ( bool hidden ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Centers the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void centerCursor ( ) const ;
	
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderTarget.
    /// In case this RenderTarget has a RenderContext object, this function
    /// may change the current RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Swap buffers if this render target has a render context.
    //////////////////////////////////////////////////////////////////////
    virtual void swapBuffers () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a RenderContext
    /// and should be drawed by the Renderer during the first phase.
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsRenderContext() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the RenderTarget is ready to be drawn on.
    /// For example , this should be true for a Window object if the Window
    /// is directly visible by the user on the screen ( 'isVisible' and
    /// 'isOnActiveSpace' properties on macOs ).
    //////////////////////////////////////////////////////////////////////
    virtual bool isAvailableForDrawing () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::RenderContextHolder getRenderContext() ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual const Gre::RenderContextHolder getRenderContext() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'Expose' X11 event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatExposeEvent ( const ::XEvent & event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'ButtonPress' X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatButtonPressEvent ( const ::XEvent& event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'ButtonRelease' X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatButtonReleaseEvent ( const ::XEvent& event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'KeyPress' X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatKeyPressEvent ( const XEvent & event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'KeyRelease' X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatKeyReleaseEvent ( const XEvent & event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'WM_PROTOCOLS' deleting X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatDeleteEvent ( const XEvent & event , const Gre::EventHolder & updateevent ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'StructureNotify' X11 Event.
    //////////////////////////////////////////////////////////////////////
    virtual void X11TreatStructureNotifyEvent ( const XEvent & event , const Gre::EventHolder & updateevent ) ;
    
    
protected:
  
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal function to set a title to the Window.
    //////////////////////////////////////////////////////////////////////
    virtual void _setTitle ( const std::string& title ) const ;
    
protected:
    
    /// @brief Holds the Window's title.
    std::string iTitle ;
    
    /// @brief X11 Display.
    ::Display* iX11Display ;
    
    /// @brief X11 Window.
    ::Window iX11Window ;
    
    /// @brief X11 GlX Context.
    ::GLXContext iX11Context ;
    
    /// @brief X11 Colormap.
    ::Colormap iX11Colormap ;
    
    /// @brief X11 XF86 Desktop mode. When the Window is changed into fullscreen mode , the 
    /// desktop video mode must be saved to be restored when killing the window.
    ::XF86VidModeModeInfo iX11DeskMode ;
    
    /// @brief true if this Window is fullscreen. When destroying the Window , the XF86 Video 
    /// mode will be restored to the desktop mode saved during creation.
    bool iX11Fullscreen ;
    
    /// @brief Atomic variable used to store WM_DELETE_WINDOW protocol.
    ::Atom iX11WMDelete ;
    
public:
    
    /// @brief A list of X11 Windows created.
    static std::vector <  X11OpenGlWindow* > sX11Windows ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a X11 handle in created X11 Windows.
    //////////////////////////////////////////////////////////////////////
    static X11OpenGlWindow* FindX11 ( const ::Window & win ) ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Loads a new X11OpenGlWindow.
//////////////////////////////////////////////////////////////////////
class X11OpenGlWindowLoader : public Gre::WindowLoader 
{
public:
	
    POOLED( Gre::Pools::Loader )
	
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    X11OpenGlWindowLoader () ;
	
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~X11OpenGlWindowLoader () noexcept (false) ;
	
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Gre::WindowHolder load ( const std::string& name , const Gre::ResourceLoaderOptions & info ) const ;
	
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    ////////////////////////////////////////////////////////////////////////
    Gre::ResourceLoader* clone() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns alway false.
    //////////////////////////////////////////////////////////////////////
    bool isLoadable( const std::string& filepath ) const ;
};

#endif // GRE_X11OPENGLWINDOW_H
