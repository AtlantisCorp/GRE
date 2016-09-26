//////////////////////////////////////////////////////////////////////
//
//  OSXWindow.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 10/12/2015.
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

#ifndef GRE_OSXWindow_h
#define GRE_OSXWindow_h

#include "OSXFunctions.h"

class DLL_PUBLIC DarwinWindow : public WindowPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Currently creates the Window with given parameter and show
    /// it on the screen.
    //////////////////////////////////////////////////////////////////////
    DarwinWindow (const std::string & name, int x0, int y0, int wid, int height);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DarwinWindow () noexcept (false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Treat Event in the Event Queue, if it has one.
    /// @return True if an Event has been treated. The return value of this
    /// function is not an Error check.
    //////////////////////////////////////////////////////////////////////
    virtual bool pollEvent();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set a new title for this Window.
    //////////////////////////////////////////////////////////////////////
    virtual void setTitle(const std::string& title);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the RenderContext used by this Window.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderContext(const RenderContext& renderCtxt);
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw immediately the surface from this RenderTarget.
    //////////////////////////////////////////////////////////////////////
    void draw();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the RenderTarget is ready to be drawn on.
    /// For example , this should be true for a Window object if the Window
    /// is directly visible by the user on the screen ( 'isVisible' and
    /// 'isOnActiveSpace' properties on macOs ).
    /// @return True if 'isVisible' and 'isOnActiveSpace' properties are
    /// true.
    //////////////////////////////////////////////////////////////////////
    bool isAvailableForDrawing () const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// [thread-safe]
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by 'onUpdateEvent', treat MacOS Event.
    //////////////////////////////////////////////////////////////////////
    virtual void iPollEvent();
    
private:
    
    /// @brief True if the Window object has been closed, after being exposed.
    bool iHasBeenClosed;
    
    /// @brief A reference to the Obj-C NsWindow object.
    CFTypeRef iWindow;
};

//////////////////////////////////////////////////////////////////////
/// @brief WindowLoader for DarwinWindow.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC DarwinWindowLoader : public WindowLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DarwinWindowLoader();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DarwinWindowLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Window.
    //////////////////////////////////////////////////////////////////////
    virtual WindowHolder load ( const std::string& name , int x0 , int y0 , int wid , int height ) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
};

#endif
