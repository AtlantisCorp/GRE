//////////////////////////////////////////////////////////////////////
//
//  X11Application.cpp
//  This source file is part of Gre
//      (Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 30/05/2017.
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

#include <X11Application.h>
#include <X11OpenGlWindow.h>
#include <X11OpenGl.h>
#include <X11Keycodes.h>

// -----------------------------------------------------------------------------
// Globals

::Display* gX11Display = NULL ;

// -----------------------------------------------------------------------------
// X11Application implementation.

X11Application::X11Application(const std::string& name, const std::string& author, const std::string& description): Application(name, author, description)
{
    iX11Display =
    gX11Display = XOpenDisplay(0) ;

    iX11XKB.available = XkbQueryExtension( iX11Display, &iX11XKB.majorOpcode, &iX11XKB.eventBase, &iX11XKB.errorBase, &iX11XKB.major, &iX11XKB.minor );

    if ( iX11XKB.available )
    {
        Bool supported ;

        if ( XkbSetDetectableAutoRepeat(iX11Display, True, &supported) )
        {
            if ( supported )
            {
                iX11XKB.detectable = true ;
            }
        }

        GreDebug( "X11 XKB Extension (" ) << iX11XKB.major << "." << iX11XKB.minor << ") available." << Gre::gendl ;
    }

    else
    {
        GreDebug( "X11 XKB Extension not available." ) << Gre::gendl ;
    }
}

X11Application::~X11Application()
{
    XCloseDisplay( iX11Display ) ;
    gX11Display = NULL ;
}

void X11Application::iMainThreadLoop()
{
    Gre::Application::startWorkerThread () ;

    //////////////////////////////////////////////////////////////////////
    // Standard Application run loop.

    while ( ! Gre::Application::iShouldTerminate )
    {
        if ( !Gre::Application::isElapsedTime() )
        continue ;

        auto event = Gre::Application::getNextUpdateEvent () ;

        //////////////////////////////////////////////////////////////////////
        // Treats every events X11 may have.

        iX11HandleEvents ( event ) ;

        //////////////////////////////////////////////////////////////////////
        // Standard Application updates and render.

        Gre::Application::iRendererManager -> render();
        Gre::Application::iWindowManager -> onEvent( event ) ;
    }
}

void X11Application::iX11HandleEvents(const Gre::EventHolder& updateevent)
{
    GreAutolock ;

    XEvent event ;

    //////////////////////////////////////////////////////////////////////
    // While there are events , force them to be treated.

    while ( XPending( iX11Display ) )
    {
        XNextEvent( iX11Display , &event ) ;

        ::Window win = event.xany.window ;
		X11OpenGlWindow* x11win = X11OpenGlWindow::FindX11 ( win ) ;

		if ( !x11win ) {
			GreDebug ( "[WARN] Invalid X11 Window handle : " ) << (long unsigned int) win << Gre::gendl ;
			continue;
		}

        switch ( event.type )
        {
            case Expose:

                x11win -> X11TreatExposeEvent ( event , updateevent ) ;
                break;

            case ButtonPress:

                x11win -> X11TreatButtonPressEvent ( event , updateevent ) ;
                break;

            case ButtonRelease:

                x11win -> X11TreatButtonReleaseEvent ( event , updateevent ) ;
                break;

            case KeyPress:

                x11win -> X11TreatKeyPressEvent ( event , updateevent ) ;
                break;

            case KeyRelease:

                x11win -> X11TreatKeyReleaseEvent ( event , updateevent ) ;
                break;

            case ClientMessage:

                if ( *XGetAtomName( iX11Display , event.xclient.message_type ) == *"WM_PROTOCOLS" )
                x11win -> X11TreatDeleteEvent ( event , updateevent ) ;

                break;

            case ConfigureNotify:

                x11win -> X11TreatStructureNotifyEvent ( event , updateevent ) ;
                break;

            default:
                GreDebug ( "[INFO] X11 unhandled event type : " ) << (int) event.type << Gre::gendl ;
                break;
        }
    }
}

bool X11Application::X11Initialize()
{
    X11InternalInitKeycodes();

    //////////////////////////////////////////////////////////////////////
    // Checks threads compatibility is correct.

    if ( XInitThreads() == 0 )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Checks that opening the default display is correct.

    ::Display* dpy = XOpenDisplay(0) ;
    bool ret = ( dpy != nullptr ) ;

    XCloseDisplay( dpy ) ;
    return ret ;
}

// -----------------------------------------------------------------------------
// X11ApplicationLoader Implementation

X11ApplicationLoader::X11ApplicationLoader()
{

}

X11ApplicationLoader::~X11ApplicationLoader() noexcept(false)
{

}

Gre::ApplicationHolder X11ApplicationLoader::load(const std::string& name, const std::string& author, const std::string& desc) const
{
    return new X11Application( name , author , desc ) ;
}

Gre::ResourceLoader* X11ApplicationLoader::clone() const
{
    return new X11ApplicationLoader() ;
}

bool X11ApplicationLoader::isLoadable(const std::string& file) const
{
    return false ;
}

