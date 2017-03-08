//////////////////////////////////////////////////////////////////////
//
//  macOSWindow.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 09/02/2017.
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

#ifndef macOSWindow_h
#define macOSWindow_h

#include "macOSPlatform.h"
#include "nsContext.h"
#include <Window.h>

using namespace Gre ;

//////////////////////////////////////////////////////////////////////
/// @brief macOS implementation of Gre::Window.
//////////////////////////////////////////////////////////////////////
class macWindow : public Window
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    macWindow ( const std::string& name , const WindowOptions& options = WindowOptions() ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~macWindow () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderTarget.
    /// In case this RenderTarget has a RenderContext object, this function
    /// may change the current RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Swap buffers if this render target has a render context.
    //////////////////////////////////////////////////////////////////////
    virtual void swapBuffers () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderTarget contains a RenderContext
    /// and should be drawed by the Renderer during the first phase.
    //////////////////////////////////////////////////////////////////////
    virtual bool holdsRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the RenderTarget is ready to be drawn on.
    /// For example , this should be true for a Window object if the Window
    /// is directly visible by the user on the screen ( 'isVisible' and
    /// 'isOnActiveSpace' properties on macOs ).
    //////////////////////////////////////////////////////////////////////
    virtual bool isAvailableForDrawing () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContextUser getRenderContext();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to the RenderContext associated to this
    /// RenderTarget, or null.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextUser getRenderContext() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Hides/Unhides the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void toggleCursor ( bool hidden ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Centers the cursor if the window is focused.
    //////////////////////////////////////////////////////////////////////
    virtual void centerCursor ( ) const ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sends a WindowWillClose event. 
    //////////////////////////////////////////////////////////////////////
    void notifiateWindowShouldClose () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateSize ( int width, int height ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiatePosition ( int x , int y ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateMouseDown () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateMouseUp () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateRightMouseDown () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateRightMouseUp () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateMouseExited () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateMouseEntered () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateKeyDown ( const Key & key , int mods ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateKeyUp ( const Key& key , int mods ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void notifiateMouseMoved ( float dx , float dy ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal function to set a title to the Window.
    //////////////////////////////////////////////////////////////////////
    virtual void _setTitle ( const std::string& title ) const;
    
public:
    
    /// @brief [NSWindow] 
    id nsWindow ;
    
    /// @brief [macOSWindowDelegate]
    id nsDelegate ;
    
    /// @brief [NSView]
    id view ;
    
    /// @brief [NSOpenGLContext] OpenGL Context associated to this Window.
    nsContext* nsglContext ;
    
    /// @brief
    RenderContextHolder context ;
};

//////////////////////////////////////////////////////////////////////
/// @brief macOS Window loader.
//////////////////////////////////////////////////////////////////////
class macOSWindowLoader : public WindowLoader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    macOSWindowLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~macOSWindowLoader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Window.
    //////////////////////////////////////////////////////////////////////
    virtual WindowHolder load ( const std::string& name , const WindowOptions & info ) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
};

#endif /* macOSWindow_h */
