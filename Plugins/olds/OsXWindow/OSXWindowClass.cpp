//////////////////////////////////////////////////////////////////////
//
//  OSXWindow.cpp
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

#include "OSXWindow.h"
#include "WindowEvent.h"
#include "Key.h"

Key DarwinWindowTranslateKey ( int key )
{
    // For now , we only handle ASCII characters. So extract the ASCII character from given key.
    return (Key) key ;
}

DarwinWindow::DarwinWindow(const std::string & name, int x0, int y0, int wid, int hei)
: WindowPrivate(name)
, iHasBeenClosed( false )
, iWindow ( NULL )
, iEventQueue(nullptr)
{
    GreResourceAutolock ;
    
    // First , try to create the WindowEventQueue . If we do not success creating a WindowEventQueue , just use
    // the Global Queue . Please note the Global Queue should not be destroyed by this object .
    
    iEventQueue = EventQueueCreate() ;
    
    if ( !iEventQueue )
    {
        GreDebugPretty() << "'EventQueueCreate()' couldn't create a new Event Queue. Using Global Queue instead." << Gre::gendl;
        iEventQueue = WindowGlobalQueue ;
    }
    
    // As soon as possible, we try to create the Window.
    NsCreateWindow( &iWindow, x0, y0, wid, hei );
    
    if( iWindow == NULL )
    {
        GreDebugPretty() << "Impossible to create 'DarwinWindow' implementation." << Gre::gendl;
        throw GreExceptionWithText( "Impossible to create 'DarwinWindow' implementation." ) ;
    }
    
    else
    {
//      This should not be here. The 'WETWindowDidExpose' event should be used instead .
//      iClosed = false;
        
        NsWindowSetEventQueue( &iWindow , iEventQueue ) ;
        
        // NOTE ( 13.10.2016 ) : 'WETWindowDidExpose' never sent by the NSWindow object. So
        // initialize here the Surface object.
        
        iSurface.top = x0;
        iSurface.left = y0;
        iSurface.width = wid;
        iSurface.height = hei;
    }
}

DarwinWindow::~DarwinWindow() noexcept (false)
{
    GreResourceAutolock ;
    
    // First , destroy the Window using the NsWindowDestroy() function . This will normally send a 'WETWindowWilClose'
    // event , which will not be processed by this Window object . ( 'pollEvent' will never be called. )
    
    if ( iWindow )
    {
        NsDestroyWindow(&iWindow);
        iWindow = NULL;
    }
    
    // Try to destroy the Event Queue if it is not the Global Queue .
    
    if ( iEventQueue && iEventQueue != WindowGlobalQueue )
    {
        EventQueueDestroy( iEventQueue ) ;
    }
}

void DarwinWindow::bind()
{
    GreResourceAutolock ;
    
    if( !getRenderContext().isInvalid() )
    {
        getRenderContext()->bind();
    }
}

void DarwinWindow::bindFramebuffer()
{
    
}

void DarwinWindow::unbind()
{
    GreResourceAutolock ;
    
    if( !getRenderContext().isInvalid() )
    {
        getRenderContext()->flush();
        getRenderContext()->unbind();
    }
}

void DarwinWindow::unbindFramebuffer()
{
    
}

bool DarwinWindow::isAvailableForDrawing() const
{
    GreResourceAutolock ;
    return NsWindowPropertyIsVisible ( &iWindow ) && NsWindowPropertyIsOnActiveSpace ( &iWindow );
}

void DarwinWindow::iSetTitle(const std::string &title) const
{
    GreResourceAutolock ;
    NsSetWindowTitle( const_cast<CFTypeRef*>(&iWindow) , title.c_str() );
}

void DarwinWindow::iSetRenderContext(const RenderContextHolder &ctxt) const
{
    if( !ctxt.isInvalid() )
    {
        GreResourceAutolock ;
        
        uintptr_t* _mContext = (uintptr_t*) ( (const CGLContextObj*) ctxt->getProperty("CGLContext") );
        // When RenderContext is changed, we must notifiate the CustomWindow
        // for it to change the OpenGlCustomView.
        NsWindowSetRenderContext( const_cast<CFTypeRef*>(&iWindow) , *((CGLContextObj*)_mContext) );
    }
}

void DarwinWindow::onUpdateEvent(const Gre::UpdateEvent &e)
{
    // Poll Event only for this Window.
    
    iPollEvent();
    
    // When receiving an update Event, we should first call 'Window::onUpdateEvent'. This
    // should also call 'RenderContext::onUpdateEvent', but the DarwinWindow would not be
    // updated. So call this after.
    
    WindowPrivate::onUpdateEvent(e);
}

void DarwinWindow::iPollEvent()
{
    // 'iPollEvent' should see in the Event Queue if there are any Events for this Window . This should be
    // treated as one event per function call . If there are no events in the private event queue for this
    // window , and the private queue is not the global queue , this object will also look in the Global
    // Queue .
    
    GreResourceAutolock ;
    
    if ( iEventQueue )
    {
        WindowEventBase * nextevent = EventQueueGet( iEventQueue, iWindow, true );
        
        // If 'nextevent' is null , try to get one using the Global Queue .
        
        if ( !nextevent && iEventQueue != WindowGlobalQueue )
        {
            nextevent = EventQueueGet( WindowGlobalQueue, iWindow, true ) ;
        }
        
        if ( nextevent )
        {
            if ( nextevent->WinEventType == WETWindowDidMove )
            {
                WindowEventDidMove * didmove = reinterpret_cast<WindowEventDidMove*>(nextevent) ;
                
                GreDebugPretty() << "New Window Coordinates : " << Gre::gendl;
                GreDebugPretty() << "  X = " << didmove->x << Gre::gendl;
                GreDebugPretty() << "  Y = " << didmove->y << Gre::gendl;
                
                iSurface.left = didmove->x ;
                iSurface.top = didmove->y ;
                
                WindowMovedEvent * event = new WindowMovedEvent ( this , didmove->x , didmove->y ) ;
                sendEvent ( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowDidResize )
            {
                WindowEventDidResize * didresize = reinterpret_cast<WindowEventDidResize*>(nextevent) ;
                
                GreDebugPretty() << "New Window Size : " << Gre::gendl;
                GreDebugPretty() << "  Width  = " << didresize->width << Gre::gendl;
                GreDebugPretty() << "  Height = " << didresize->height << Gre::gendl;
                
                iSurface.width = didresize->width ;
                iSurface.height = didresize->height ;
                
                WindowSizedEvent * event = new WindowSizedEvent ( this , didresize->width , didresize->height ) ;
                sendEvent( event , false ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowKeyPressed )
            {
                WindowEventKeyPressed * keypressed = reinterpret_cast<WindowEventKeyPressed*>(nextevent) ;
                
                GreDebugPretty() << "Key pressed : '" << keypressed->key << "'." << Gre::gendl;
                
                KeyDownEvent * event = new KeyDownEvent ( this , DarwinWindowTranslateKey(keypressed->key) ) ;
                sendEvent( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowKeyReleased )
            {
                WindowEventKeyReleased * keyreleased = reinterpret_cast<WindowEventKeyReleased*>(nextevent) ;
                
                GreDebugPretty() << "Key released : '" << keyreleased->key << "'." << Gre::gendl;
                
                KeyUpEvent * event = new KeyUpEvent ( this , DarwinWindowTranslateKey(keyreleased->key) ) ;
                sendEvent( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowExposed )
            {
                WindowEventExposed * exposed = reinterpret_cast<WindowEventExposed*>(nextevent) ;
                
                GreDebugPretty() << "Window is exposed." << Gre::gendl;
                
                iSurface.left = exposed->x ;
                iSurface.top = exposed->y ;
                iSurface.width = exposed->width ;
                iSurface.height = exposed->height ;
                iExposed = true;
                
                WindowExposedEvent * event = new WindowExposedEvent ( this , iSurface ) ;
                sendEvent( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowHidden )
            {
                WindowEventHidden * hidden = reinterpret_cast<WindowEventHidden*>(nextevent) ;
                
                GreDebugPretty() << "Window is hidden." << Gre::gendl;
                
                WindowHiddenEvent * event = new WindowHiddenEvent ( this ) ;
                sendEvent ( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowWillClose )
            {
                GreDebugPretty() << "Window '" << getName() << "' will close." << Gre::gendl;
                
                iExposed = false ;
                iClosed = true ;
                
                WindowWillCloseEvent * event = new WindowWillCloseEvent ( this ) ;
                sendEvent ( event ) ;
            }
            
            else if ( nextevent->WinEventType == WETWindowTitleChanged )
            {
                WindowEventTitleChanged * titlechanged = reinterpret_cast<WindowEventTitleChanged*>(nextevent) ;
                
                GreDebugPretty() << "Window '" << getName() << "' title changed." << Gre::gendl;
                GreDebugPretty() << "  Title = '" << titlechanged->title << "'." << Gre::gendl;
                
                iTitle = std::string ( titlechanged->title , titlechanged->size ) ;
                
                WindowTitleChangedEvent * event = new WindowTitleChangedEvent ( this , iTitle ) ;
                sendEvent ( event ) ;
            }
            
            // At end , always destroy this WindowEventBase .
            free ( nextevent ) ;
        }
    }
}

// ---------------------------------------------------------------------------------------

DarwinWindowLoader::DarwinWindowLoader()
{
    
}

DarwinWindowLoader::~DarwinWindowLoader()
{
    
}

WindowHolder DarwinWindowLoader::load(const std::string &name, int x0, int y0, int wid, int height) const
{
    return WindowHolder ( new DarwinWindow(name, x0, y0, wid, height) );
}

ResourceLoader* DarwinWindowLoader::clone() const
{
    return new DarwinWindowLoader();
}

bool DarwinWindowLoader::isLoadable(const std::string &filepath) const
{
    return false;
}
