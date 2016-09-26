//
//  OSXWindowClass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#include "OSXWindow.h"
#include "WindowEvent.h"

DarwinWindow::DarwinWindow(const std::string & name, int x0, int y0, int wid, int hei)
: WindowPrivate(name)
, iWindow ( NULL )
, iHasBeenClosed( false )
{
    lockGuard();
    
    // As soon as possible, we try to create the Window.
    NsCreateWindow(&iWindow, x0, y0, wid, hei);
    
    if(iWindow == NULL)
    {
        GreDebugPretty() << "Impossible to create Window (Os X Plugin)." << std::endl;
    }
    else
    {
        iClosed = false;
    }
    
    unlockGuard();
}

DarwinWindow::~DarwinWindow() noexcept (false)
{
    lockGuard();
    
    NsDestroyWindow(&iWindow);
    iWindow = NULL;
    
    unlockGuard();
}

bool DarwinWindow::pollEvent()
{
    bool ret = WindowPrivate::pollEvent();
    
    if( !iClosed )
    {
        lockGuard();
        {
            ret = NsPollEvent();
            WindowBufEntry* nsWindowEntry = NsGetWindowBufEntry(&iWindow);
            
            if( nsWindowEntry )
            {
                while( nsWindowEntry->keybuf_sz )
                {
                    if(nsWindowEntry->keybufs[0].pressed > 0) {
                        KeyDownEvent e(Key(nsWindowEntry->keybufs[0].key));
                        sendEvent(e);
                        nsWindowEntry->keybuf_sz--;
                    } else {
                        KeyUpEvent e(Key(nsWindowEntry->keybufs[0].key));
                        sendEvent(e);
                        nsWindowEntry->keybuf_sz--;
                    }
                }
                
                if( nsWindowEntry->sizeChanged )
                {
                    iSurface.width = nsWindowEntry->newWidth;
                    iSurface.height = nsWindowEntry->newHeight;
                    iSurface.left = nsWindowEntry->newX;
                    iSurface.top = nsWindowEntry->newY;
                    nsWindowEntry->sizeChanged = false;
                    
                    WindowSizedEvent e (Window(this), iSurface);
                    onEvent(e);
                }
                
                iHasBeenClosed = iExposed && nsWindowEntry->closed;
                iClosed = nsWindowEntry->closed;
                iExposed = nsWindowEntry->exposed;
                
                // We must double-check visibility because i don't find any notification
                // to indicate Exposure.
                if(!iExposed)
                    iExposed = NsWindowIsVisible(&iWindow);
            }
        }
        unlockGuard();
    }
    
//  NsWindowSwapBuffers(&iWindow);
    
    return true || ret;
}

bool DarwinWindow::hasBeenClosed() const
{
    return iHasBeenClosed;
}

void DarwinWindow::setTitle(const std::string& title)
{
    lockGuard();
    
    WindowPrivate::setTitle(title);
    NsSetWindowTitle(&iWindow, title.c_str());
    
    unlockGuard();
}

void DarwinWindow::bind()
{
    lockGuard();
    
    if( !getRenderContext().isInvalid() )
    {
        getRenderContext()->bind();
    }
    
    unlockGuard();
}

void DarwinWindow::bindFramebuffer()
{
    
}

void DarwinWindow::unbind()
{
    lockGuard();
    
    if( !getRenderContext().isInvalid() )
    {
        getRenderContext()->flush();
        getRenderContext()->unbind();
    }
    
    unlockGuard();
}

void DarwinWindow::unbindFramebuffer()
{
    
}

void DarwinWindow::setRenderContext(const Gre::RenderContext &renderCtxt)
{
    WindowPrivate::setRenderContext(renderCtxt);
    
    if( !iRenderContext.isInvalid() )
    {
        lockGuard();
        
        uintptr_t* _mContext = (uintptr_t*) ( (const CGLContextObj*) iRenderContext->getProperty("CGLContext") );
        // When RenderContext is changed, we must notifiate the CustomWindow
        // for it to change the OpenGlCustomView.
        NsWindowSetRenderContext(&iWindow, *((CGLContextObj*)_mContext));
        
        unlockGuard();
    }
}

void DarwinWindow::draw()
{
    if ( iWindow )
    {
        lockGuard();
        
        NsWindowDisplay(&iWindow);
        
        unlockGuard();
    }
}

bool DarwinWindow::isAvailableForDrawing() const
{
    lockGuard();
    
    bool ret = NsWindowPropertyIsVisible ( &iWindow ) && NsWindowPropertyIsOnActiveSpace ( &iWindow );
    
    unlockGuard();
    
    return ret;
}

void DarwinWindow::onUpdateEvent(const Gre::UpdateEvent &e)
{
    // Poll Event only for this Window.
    
    iPollEvent();
    
    // When receiving an update Event, we should first call 'Window::onUpdateEvent'. This
    // should also call 'RenderContext::onUpdateEvent', but the DarwinWindow would not be
    // updated. So call this later.
    
    WindowPrivate::onUpdateEvent(e);
}

void DarwinWindow::iPollEvent()
{
    if(!iClosed)
    {
        // First lock the Resource's mutex in order to be sure anyone disturb us.
        
        lockGuard();
        {
            // Calls the NSPollEvent for this Window object.
            
            NsPollEvent();
//          NsPollEventForWindow(&iWindow);
            
            // Treat the Events for this Window.
            
            WindowBufEntry* nsWindowEntry = NsGetWindowBufEntry(&iWindow);
            
            if(nsWindowEntry)
            {
                while(nsWindowEntry->keybuf_sz)
                {
                    if(nsWindowEntry->keybufs[0].pressed > 0) {
                        KeyDownEvent e(Key(nsWindowEntry->keybufs[0].key));
                        sendEvent(e);
                        nsWindowEntry->keybuf_sz--;
                    } else {
                        KeyUpEvent e(Key(nsWindowEntry->keybufs[0].key));
                        sendEvent(e);
                        nsWindowEntry->keybuf_sz--;
                    }
                }
                
                if(nsWindowEntry->sizeChanged)
                {
                    iSurface.width = nsWindowEntry->newWidth;
                    iSurface.height = nsWindowEntry->newHeight;
                    iSurface.left = nsWindowEntry->newX;
                    iSurface.top = nsWindowEntry->newY;
                    nsWindowEntry->sizeChanged = false;
                    
                    WindowSizedEvent e (Window(this), iSurface);
                    onEvent(e);
                }
                
                iHasBeenClosed = iExposed && nsWindowEntry->closed;
                iClosed = nsWindowEntry->closed;
                iExposed = nsWindowEntry->exposed;
                
                // We must double-check visibility because i don't find any notification
                // to indicate Exposure.
                if(!iExposed)
                    iExposed = NsWindowIsVisible(&iWindow);
            }
        }
        
        // Unlock Resource's mutex.
        
        unlockGuard();
        
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
