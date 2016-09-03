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
    // As soon as possible, we try to create the Window.
    NsCreateWindow(&iWindow, x0, y0, wid, hei);
    
    if(iWindow == NULL)
    {
        GreDebugPretty() << "Impossible to create Window (Os X Plugin)." << std::endl;
    }
}

DarwinWindow::~DarwinWindow() noexcept (false)
{
    NsDestroyWindow(&iWindow);
    iWindow = NULL;
}

bool DarwinWindow::pollEvent()
{
    bool ret = WindowPrivate::pollEvent();
    
    if(!iClosed)
    {
        ret = NsPollEvent();
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
        
        return ret;
    }
    
    return true || ret;
}

bool DarwinWindow::hasBeenClosed() const
{
    return iHasBeenClosed;
}

void DarwinWindow::setTitle(const std::string& title)
{
    WindowPrivate::setTitle(title);
    NsSetWindowTitle(&iWindow, title.c_str());
}

/*
void DarwinWindow::swapBuffers ()
{
    
    NsWindowSwapBuffers(&iWindow);
}
 */

void DarwinWindow::bind()
{
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
    if( !getRenderContext().isInvalid() )
    {
        getRenderContext()->flush();
        getRenderContext()->unbind();
    }
}

void DarwinWindow::unbindFramebuffer()
{
    
}

void DarwinWindow::setRenderContext(const Gre::RenderContext &renderCtxt)
{
    WindowPrivate::setRenderContext(renderCtxt);
    
    if( !iRenderContext.isInvalid() )
    {
        uintptr_t* _mContext = (uintptr_t*) ( (const CGLContextObj*) iRenderContext->getProperty("CGLContext") );
        // When RenderContext is changed, we must notifiate the CustomWindow
        // for it to change the OpenGlCustomView.
        NsWindowSetRenderContext(&iWindow, *((CGLContextObj*)_mContext));
    }
}

// ---------------------------------------------------------------------------------------------------

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
