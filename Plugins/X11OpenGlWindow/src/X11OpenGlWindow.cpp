//////////////////////////////////////////////////////////////////////
//
//  X11OpenGlWindow.cpp
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

#include "X11OpenGlWindow.h"
#include "X11OpenGlContext.h"
#include "X11Keycodes.h"
#include "X11Application.h"

#include <X11/cursorfont.h>

// -----------------------------------------------------------------------------
// X11 Utilities.

//////////////////////////////////////////////////////////////////////
// Returns the Visual Attribs hold by the key 'Window.ContextAttributes'.
int* GetVisualAttribs ( const Gre::ResourceLoaderOptions& options , int* glmajor , int* glminor , bool* fullscreen )
{
	auto it = options.find("Window.ContextAttributes") ;

	if ( it == options.end() )
	{
		//////////////////////////////////////////////////////////////////////
		// Makes a default attribute list.

		int* visual_attribs = (int*) malloc(sizeof(int) * 40) ;

		if ( !visual_attribs )
		return nullptr ;

        memset( visual_attribs , 0 , sizeof(int) * 40 );

		visual_attribs [0] = GLX_X_RENDERABLE ;  visual_attribs [1] = True ;
		visual_attribs [2] = GLX_DRAWABLE_TYPE ; visual_attribs [3] = GLX_WINDOW_BIT ;
		visual_attribs [4] = GLX_RENDER_TYPE ;   visual_attribs [5] = GLX_RGBA_BIT ;
		visual_attribs [6] = GLX_X_VISUAL_TYPE ; visual_attribs [7] = GLX_TRUE_COLOR ;

		visual_attribs [8]  = GLX_RED_SIZE ;     visual_attribs [9]  = 8 ;
		visual_attribs [10] = GLX_BLUE_SIZE ;    visual_attribs [11] = 8 ;
		visual_attribs [12] = GLX_GREEN_SIZE ;   visual_attribs [13] = 8 ;
		visual_attribs [14] = GLX_ALPHA_SIZE ;   visual_attribs [15] = 8 ;
		visual_attribs [16] = GLX_DEPTH_SIZE ;   visual_attribs [17] = 24 ;
		visual_attribs [18] = GLX_STENCIL_SIZE ; visual_attribs [19] = 8 ;

		visual_attribs [20] = GLX_DOUBLEBUFFER ; visual_attribs [21] = True ;
		visual_attribs [22] = None ;

		return visual_attribs ;
	}

	else
	{
	    GreDebug( "[INFO] Creating custom Visual Attributes." ) << Gre::gendl ;

		//////////////////////////////////////////////////////////////////////
		// Creates a container for the attributes. Notes as default GRE attributes
		// are not the same as X11 default FB Configs values , uses booleans to
		// know if values are present or not in the ContextAttributes.

		int* visual_attribs = (int*) malloc ( sizeof(int) * 60 ) ;

		if ( !visual_attribs )
		return nullptr ;

        memset( visual_attribs , 0 , sizeof(int) * 60 );

		int current = -1 ;
		bool redsize = false ; bool greensize = false ; bool bluesize = false ;
		bool alphasize = false ; bool depthsize = false ; bool stencilsize = false ;

		//////////////////////////////////////////////////////////////////////
		// Default constants.

		visual_attribs [current++] = GLX_X_RENDERABLE ;  visual_attribs [current++] = True ;
		visual_attribs [current++] = GLX_DRAWABLE_TYPE ; visual_attribs [current++] = GLX_WINDOW_BIT ;
		visual_attribs [current++] = GLX_RENDER_TYPE ;   visual_attribs [current++] = GLX_RGBA_BIT ;
		visual_attribs [current++] = GLX_X_VISUAL_TYPE ; visual_attribs [current++] = GLX_TRUE_COLOR ;

		//////////////////////////////////////////////////////////////////////
		// Parse the Context Attributes.

		const Gre::WindowContextAttributes & attribs = it->second.to<Gre::WindowContextAttributes>() ;

		for ( unsigned int i = 0 ; i < attribs.size() ; ++i )
		{
			const Gre::WindowContextAttribute & attr = attribs.at(i) ;

			if ( attr == Gre::WCADoubleBuffer )
			{
				visual_attribs [current++] = GLX_DOUBLEBUFFER ;
				visual_attribs [current++] = True ;
			}

			else if ( attr == Gre::WCAStereo )
			{
				visual_attribs [current++] = GLX_STEREO ;
				visual_attribs [current++] = True ;
			}

			else if ( attr == Gre::WCAMultiSample )
			{
				visual_attribs [current++] = GLX_CONFIG_CAVEAT ;
			    visual_attribs [current++] = GLX_NON_CONFORMANT_CONFIG ;
			}

			else if ( attr == Gre::WCASampleBuffers )
			{
				visual_attribs [current++] = GLX_SAMPLE_BUFFERS ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
			}

			else if ( attr == Gre::WCASamples )
			{
				visual_attribs [current++] = GLX_SAMPLES ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
			}

			else if ( attr == Gre::WCAColorRedSize )
			{
				visual_attribs [current++] = GLX_RED_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				redsize = true ;
			}

			else if ( attr == Gre::WCAColorGreenSize )
			{
				visual_attribs [current++] = GLX_GREEN_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				greensize = true ;
			}

			else if ( attr == Gre::WCAColorBlueSize )
			{
				visual_attribs [current++] = GLX_BLUE_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				bluesize = true ;
			}

			else if ( attr == Gre::WCAColorAlphaSize )
			{
				visual_attribs [current++] = GLX_ALPHA_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				alphasize = true ;
			}

			else if ( attr == Gre::WCADepthSize )
			{
				visual_attribs [current++] = GLX_DEPTH_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				depthsize = true ;

				GreDebug( "[INFO] Depth Size = " ) << (int) attribs.at(i) << Gre::gendl ;
			}

			else if ( attr == Gre::WCAStencilSize )
			{
				visual_attribs [current++] = GLX_STENCIL_SIZE ;
				visual_attribs [current++] = (int) attribs.at(++i) ;
				stencilsize = true ;
			}

			else if ( attr == Gre::WCAMajorVersion && glmajor ) {
				(*glmajor) = (int) attribs.at(i++) ;
			}

			else if ( attr == Gre::WCAMinorVersion && glminor ) {
				(*glminor) = (int) attribs.at(i++) ;
			}

			else if ( attr == Gre::WCAFullscreen )
			{
			    if ( fullscreen )
			    *fullscreen = true ;
			}
		}

		//////////////////////////////////////////////////////////////////////
		// Computes default values.

		if ( !redsize )
		{
			visual_attribs [current++] = GLX_RED_SIZE ;
			visual_attribs [current++] = 8 ;
		}

		if ( !greensize )
		{
			visual_attribs [current++] = GLX_GREEN_SIZE ;
			visual_attribs [current++] = 8 ;
		}

		if ( !bluesize )
		{
			visual_attribs [current++] = GLX_BLUE_SIZE ;
			visual_attribs [current++] = 8 ;
		}

		if ( !alphasize )
		{
			visual_attribs [current++] = GLX_ALPHA_SIZE ;
			visual_attribs [current++] = 8 ;
		}

		if ( !depthsize )
		{
			visual_attribs [current++] = GLX_DEPTH_SIZE ;
			visual_attribs [current++] = 24 ;
		}

		if ( !stencilsize )
		{
			visual_attribs [current++] = GLX_STENCIL_SIZE ;
			visual_attribs [current++] = 8 ;
		}

		visual_attribs [current++] = None ;
		return visual_attribs ;
	}
}

GLXFBConfig GetBestFBConfig ( GLXFBConfig* fbc , Display* display , int fbcount )
{
	GreDebug( "[INFO] Getting XVisualInfos." ) << Gre::gendl ;

	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for (i=0; i<fbcount; ++i)
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );

		if ( vi )
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );

			GreDebug( "[INFO]  Matching fbconfig " ) << i << ", visual ID 0x" << std::hex << (uintptr_t) vi->visualid << std::dec <<
				": SAMPLES_BUFFERS = " << samp_buf << ", SAMPLES = " << (int) samples << "." << Gre::gendl ;

			if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
			best_fbc = i, best_num_samp = samples;

			if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
			worst_fbc = i, worst_num_samp = samples;
		}

		XFree( vi );
	}

	return fbc[best_fbc] ;
}

//////////////////////////////////////////////////////////////////////
// Returns the Window's size using the normal size format , or a default
// pair ( 1024 , 768 ). The size format is 'WidthXHeight' or 'WidthXHeight'.
// The Window's size is under the 'Window.Size' option.
std::pair<int, int> GetWindowSize ( const Gre::ResourceLoaderOptions& ops )
{
	auto formattedit = ops.find ( "Window.Size" ) ;

    if ( formattedit == ops.end() )
    return std::make_pair ( 1024 , 768 ) ;

    std::string formatted = formattedit -> second.to<std::string>() ;

    if ( formatted.empty() )
    return std::make_pair ( 1024 , 768 ) ;

    auto formsizes = split ( formatted , 'x' ) ;

    if ( formsizes.size() < 2 )
    formsizes = split ( formatted , 'X' ) ;

    if ( formsizes.size() < 2 )
    return std::make_pair ( 1024 , 768 ) ;

    int width  = strtol ( formsizes.at(0).c_str() , NULL , 10 ) ;
    int height = strtol ( formsizes.at(1).c_str() , NULL , 10 ) ;

    if ( width <= 0 || height <= 0 )
    return std::make_pair ( 1024 , 768 ) ;

    return std::make_pair ( width , height ) ;
}

//////////////////////////////////////////////////////////////////////
// Returns the Window's title depending on options given. The Window's
// title is under the 'Window.Title' option. No default option.
std::string GetWindowTitle ( const Gre::ResourceLoaderOptions & ops )
{
    auto titleit = ops.find ( "Window.Title" ) ;

    if ( titleit == ops.end() )
    return std::string () ;

    return titleit -> second.to<std::string> () ;
}

//////////////////////////////////////////////////////////////////////
// Returns true if WCAFullscreen is present under 'Window.ContextAttributes'.

bool WindowIsFullscreen ( const Gre::ResourceLoaderOptions & ops )
{
    auto it = ops.find( "Window.ContextAttributes" ) ;

    if ( it == ops.end() )
    return false ;

    auto attrs = it->second.to<Gre::WindowContextAttributes>() ;

    for ( auto attr : attrs )
    if ( attr == Gre::WCAFullscreen )
    return true ;

    return false ;
}

// -----------------------------------------------------------------------------
// X11OpenGlWindow

X11OpenGlWindow::X11OpenGlWindow(const std::string& name, Display* dpy, ::Window win, GLXContext ctxt, Colormap cmap, ::XF86VidModeModeInfo deskmode , bool fs , ::Atom wmDelete , const std::string& title, const Gre::ResourceLoaderOptions& options)
: Gre::Window(name, options)
{
    iX11Display = dpy ;
    iX11Window = win ;
    iX11Context = ctxt ;
    iX11Colormap = cmap ;
    iX11DeskMode = deskmode ;
    iX11Fullscreen = fs ;
    iX11WMDelete = wmDelete ;
    sX11Windows.push_back( this ) ;
}

X11OpenGlWindow::~X11OpenGlWindow () noexcept (false)
{
    if ( iX11Context )
    DestroyGlXContext ( iX11Display , iX11Context ) ;

    if ( iX11Window )
    XDestroyWindow ( iX11Display , iX11Window ) ;

    if ( iX11Colormap )
    XFreeColormap ( iX11Display , iX11Colormap ) ;

    if ( iX11Fullscreen )
    {
        XF86VidModeSwitchToMode( iX11Display , DefaultScreen(iX11Display) , &iX11DeskMode ) ;
        XF86VidModeSetViewPort( iX11Display , DefaultScreen(iX11Display) , 0 , 0 ) ;
    }

    sX11Windows.erase( std::find( sX11Windows.begin() , sX11Windows.end() , this ) ) ;
}

void X11OpenGlWindow::toggleCursor(bool hidden)
{
    GreAutolock ;

    if ( !iX11Window || !iX11Display )
    return ;

    //////////////////////////////////////////////////////////////////////
    // If one wants to hide the cursor , we may create an invisible one
    // by setting an blank pixmap and defining a new cursor.

    if ( hidden )
    {
        ::Cursor invisiblecursor ;
        ::Pixmap bitmapnodata ;
        ::XColor black ;
        static char noData[] = { 0,0,0,0,0,0,0,0 };
        black.red = black.green = black.blue = 0;

        bitmapnodata = XCreateBitmapFromData ( iX11Display , iX11Window , noData , 8 , 8 ) ;
        invisiblecursor = XCreatePixmapCursor( iX11Display , bitmapnodata , bitmapnodata ,
                                               &black , &black , 0 , 0 ) ;

        XDefineCursor ( iX11Display , iX11Window , invisiblecursor ) ;
        XFreeCursor ( iX11Display , invisiblecursor ) ;
        XFreePixmap ( iX11Display , bitmapnodata ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // If one wants to restore the cursor , just restore the default left
    // handed pointer cursor.

    else
    {
        ::Cursor cursor = XCreateFontCursor ( iX11Display , XC_left_ptr ) ;
        XDefineCursor ( iX11Display , iX11Window , cursor ) ;
        XFreeCursor ( iX11Display , cursor ) ;
    }
}

void X11OpenGlWindow::centerCursor() const
{
    GreAutolock ;

    if ( !iX11Window || !iX11Display )
    return ;

    XWarpPointer ( iX11Display , None , iX11Window , 0 , 0 , 0 , 0 , iSurface.width/2 , iSurface.height/2 ) ;
    XFlush ( iX11Display ) ;
}

void X11OpenGlWindow::bind() const
{
    GreAutolock ;

    if ( !iX11Display || !iX11Context )
    return ;

    glXMakeCurrent ( iX11Display , iX11Window , iX11Context ) ;
}

void X11OpenGlWindow::unbind() const
{
    GreAutolock ;

    if ( !iX11Display || !iX11Context )
    return ;

    glXMakeCurrent ( iX11Display , None , None ) ;
}

void X11OpenGlWindow::swapBuffers() const
{
    GreAutolock ;

    if (!iX11Display || ! iX11Context )
    return ;

    glXSwapBuffers ( iX11Display , iX11Window ) ;
}

bool X11OpenGlWindow::holdsRenderContext() const
{
    GreAutolock ; return iX11Context ;
}

bool X11OpenGlWindow::isAvailableForDrawing() const
{
    // For now it's a stub !!
    return true ;
}

Gre::RenderContextHolder X11OpenGlWindow::getRenderContext()
{
    GreAutolock ; return new X11Context ( getName() + ".context" , iX11Display , iX11Window , iX11Context ) ;
}

const Gre::RenderContextHolder X11OpenGlWindow::getRenderContext() const
{
    GreAutolock ; return new X11Context ( getName() + ".context" , iX11Display , iX11Window , iX11Context ) ;
}

void X11OpenGlWindow::X11TreatExposeEvent(const XEvent& event, const Gre::EventHolder& updateevent)
{
    GreAutolock ;

    Gre::Window::iExposed = true ;

    //////////////////////////////////////////////////////////////////////
    // Just ignores Expose events when there are more coming for us. We don't
    // need partial updates of the Window.

    if ( event.xexpose.count > 0 )
	return ;

    Gre::EventHolder ee = new Gre::WindowExposedEvent ( this , { event.xexpose.x , event.xexpose.y , event.xexpose.width , event.xexpose.height } ) ;
    sendEvent ( ee ) ;
}

void X11OpenGlWindow::X11TreatButtonPressEvent(const XEvent& event, const Gre::EventHolder& updateevent)
{
    GreAutolock ;

    if ( event.xbutton.button == Button1 )
    {
        Gre::EventHolder lbp = new Gre::LeftMousePressEvent ( this ) ;
        sendEvent ( lbp ) ;
    }

    else if ( event.xbutton.button == Button2 )
    {
        Gre::EventHolder rbp = new Gre::RightMousePressEvent ( this ) ;
        sendEvent ( rbp ) ;
    }
}

void X11OpenGlWindow::X11TreatButtonReleaseEvent( const XEvent& event , const Gre::EventHolder & updateevent )
{
	GreAutolock ;

	if ( event.xbutton.button == Button1 )
	{
		Gre::EventHolder lbr = new Gre::LeftMouseReleaseEvent ( this ) ;
		sendEvent ( lbr ) ;
	}

	else if ( event.xbutton.button == Button2 )
	{
		Gre::EventHolder rbr = new Gre::RightMouseReleaseEvent ( this ) ;
		sendEvent ( rbr ) ;
	}
}

void X11OpenGlWindow::X11TreatKeyPressEvent( const XEvent& event , const Gre::EventHolder & updateevent )
{
	GreAutolock ;

	XComposeStatus status ;
	KeySym keysym = NoSymbol ;
	char text[32] = {} ;

	XLookupString( const_cast<XKeyEvent*>(&event.xkey), text, 31, &keysym, &status );

	GreDebug( "[INFO] keycode = " ) << event.xkey.keycode << Gre::gendl ;
	GreDebug( "[INFO] keysym  = " ) << keysym << Gre::gendl ;
	GreDebug( "[INFO] string  = " ) << text << Gre::gendl ;

	int minkc ; int maxkc ;
	XDisplayKeycodes( iX11Display, &minkc, &maxkc );

	GreDebug( "[INFO] minkc   = " ) << minkc << Gre::gendl ;
	GreDebug( "[INFO] maxkc   = " ) << maxkc << Gre::gendl ;

    int countkc = maxkc + 1 - minkc ;
    int kspkc ;

    KeySym* keysyms = XGetKeyboardMapping( iX11Display, minkc, countkc, &kspkc );

    GreDebug( "[INFO] countkc = " ) << countkc << Gre::gendl ;
    GreDebug( "[INFO] kspkc   = " ) << kspkc << Gre::gendl ;

    for( unsigned int i = 0 ; i < countkc ; ++i )
    GreDebug( "[INFO] keysyms[" ) << i << "] = " << keysyms[(i - minkc)*kspkc] << Gre::gendl ;

	Gre::Key keycode = X11TranslateKeyCode ( event.xkey.keycode ) ;
	int mods = (int) X11TranslateKeyModifiers ( event.xkey.state ) ;

	Gre::EventHolder kpe = new Gre::KeyDownEvent ( this , keycode , mods ) ;
    sendEvent( kpe );
}

void X11OpenGlWindow::X11TreatKeyReleaseEvent( const XEvent& event , const Gre::EventHolder & updateevent )
{
    GreAutolock ;

    Gre::Key keycode = X11TranslateKeyCode ( event.xkey.keycode ) ;
    int mods = (int) X11TranslateKeyModifiers ( event.xkey.state ) ;

    Gre::EventHolder kpe = new Gre::KeyUpEvent ( this , keycode , mods ) ;
    sendEvent( kpe );
}

void X11OpenGlWindow::X11TreatDeleteEvent(const XEvent& event, const Gre::EventHolder& updateevent)
{
    GreAutolock ;

    Gre::EventHolder e = new Gre::WindowWillCloseEvent( this ) ;
    sendEvent( e );
}

void X11OpenGlWindow::X11TreatStructureNotifyEvent(const XEvent& event, const Gre::EventHolder& updateevent)
{
    GreAutolock ;

    if ( iSurface.width != event.xconfigure.width || iSurface.height != event.xconfigure.height )
    {
        iSurface.width = event.xconfigure.width ;
        iSurface.height = event.xconfigure.height ;

        Gre::EventHolder e = new Gre::WindowSizedEvent( this , iSurface.width , iSurface.height ) ;
        sendEvent( e );
    }

    if ( iSurface.left != event.xconfigure.x || iSurface.top != event.xconfigure.y )
    {
        iSurface.left = event.xconfigure.x ;
        iSurface.top = event.xconfigure.y ;

        Gre::EventHolder e = new Gre::WindowMovedEvent ( this , iSurface.left , iSurface.top ) ;
        sendEvent( e );
    }
}

void X11OpenGlWindow::_setTitle(const std::string& title) const
{
    GreAutolock ;

    if (!iX11Display || ! iX11Context )
    return ;

    XStoreName ( iX11Display , iX11Window , title.c_str() ) ;
}

// -----------------------------------------------------------------------------
// X11OpenGlWindow static implementation.

std::vector < X11OpenGlWindow* > X11OpenGlWindow::sX11Windows = std::vector < X11OpenGlWindow* > () ;

X11OpenGlWindow* X11OpenGlWindow::FindX11(const ::Window& win)
{
    for ( auto window : sX11Windows )
    if ( window->iX11Window == win )
    return window ;
}

// -----------------------------------------------------------------------------
// X11OpenGlWindowLoader

X11OpenGlWindowLoader::X11OpenGlWindowLoader()
{

}

X11OpenGlWindowLoader::~X11OpenGlWindowLoader() noexcept (false)
{

}

Gre::WindowHolder X11OpenGlWindowLoader::load( const std::string& name , const Gre::ResourceLoaderOptions& options ) const
{
	//////////////////////////////////////////////////////////////////////
	// Tries to open X11 display.

	Display* display = gX11Display ;

	if ( !display )
	{
		GreDebug( "[WARN] XOpenDisplay() failed, or no X11Application were loaded before loading the Window." ) << Gre::gendl ;
		return Gre::WindowHolder ( nullptr ) ;
	}

	//////////////////////////////////////////////////////////////////////
	// Gets the FBConfig attributes from given options. Notes after utilisation,
	// the visual_attribs should be deleted.

	int glmajor = 3 , glminor = 2 ; bool needsfs = false ;
	int* visual_attribs = GetVisualAttribs ( options , &glmajor , &glminor , &needsfs ) ;

	if ( !visual_attribs )
	{
		GreDebug( "[WARN] GetVisualAttribs() return null value." ) << Gre::gendl ;
		return Gre::WindowHolder ( nullptr ) ;
	}

	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig ( display , DefaultScreen(display) , visual_attribs , &fbcount ) ;

	if ( !fbc )
	{
		GreDebug( "[WARN] glXChooseFBConfig() failed." ) << Gre::gendl ;

		free( visual_attribs );
		return Gre::WindowHolder ( nullptr ) ;
	}

	GLXFBConfig bestFbc = GetBestFBConfig ( fbc , display , fbcount ) ;

	free( visual_attribs );
	XFree ( fbc ) ;

	//////////////////////////////////////////////////////////////////////
	// Gets Visual Info from best fbc.

	XVisualInfo* vi = glXGetVisualFromFBConfig( display , bestFbc ) ;
	GreDebug( "[INFO] Chosen visual ID = 0x" ) << std::hex << (uintptr_t) vi->visualid << std::dec << "." << Gre::gendl ;

	//////////////////////////////////////////////////////////////////////
	// Creates ColorMap and attributes.

	XSetWindowAttributes swa ;
	Colormap cmap;
	swa.colormap = cmap = XCreateColormap( display,
								RootWindow( display, vi->screen ),
								vi->visual, AllocNone );
	swa.background_pixmap = None ;
	swa.border_pixel      = 0;
	swa.event_mask        = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | KeyReleaseMask | ButtonReleaseMask ;

    long unsigned int swaattr = CWBorderPixel|CWColormap|CWEventMask ;

	//////////////////////////////////////////////////////////////////////
	// Creates Window.

	std::pair<int, int> windowsize = GetWindowSize ( options ) ;
	std::string windowtitle = GetWindowTitle ( options ) ;

    //////////////////////////////////////////////////////////////////////
    // Checks X11 XF86 Video Modes , to see if one matches our window
    // setting. This mode will so be considered 'bestmode'.

    int xf86vidmodemajor , xf86vidmodeminor ;
    XF86VidModeQueryVersion ( display , &xf86vidmodemajor , &xf86vidmodeminor ) ;
    GreDebug ( "[INFO] XF86VidModeExtension - Version " ) << xf86vidmodemajor << "." << xf86vidmodeminor << Gre::gendl ;

    XF86VidModeModeInfo ** vidmodes ; int vidmodenum ;
    XF86VidModeGetAllModeLines( display , DefaultScreen(display) , &vidmodenum , &vidmodes ) ;

    XF86VidModeModeInfo deskmode = *vidmodes[0] ;
    int bestmode = 0 ;

    for ( int i = 0 ; i < vidmodenum ; ++i )
    if ( (vidmodes[i]->hdisplay == windowsize.first) && (vidmodes[i]->vdisplay == windowsize.second) )
    bestmode = i ;

    //////////////////////////////////////////////////////////////////////
    // Checks if Window needs fullscreen mode.

    if ( needsfs )
    {
        //////////////////////////////////////////////////////////////////////
        // Changes the XF86 Video Mode.

        XF86VidModeSwitchToMode( display , DefaultScreen(display) , vidmodes[bestmode] ) ;
        XF86VidModeSetViewPort( display , DefaultScreen(display) , 0 , 0 ) ;

        GreDebug ( "[INFO] Switching to resolution '" ) << vidmodes[bestmode]->hdisplay << "x" << vidmodes[bestmode]->vdisplay << "'." << Gre::gendl ;
        XFree( vidmodes ) ;

        swa.override_redirect = true ;
        swaattr = swaattr | CWOverrideRedirect ;
    }

	Window win = XCreateWindow (display , RootWindow(display, vi->screen) ,
								0, 0, windowsize.first, windowsize.second,
								0, vi->depth, InputOutput, vi->visual,
								swaattr, &swa) ;

	if ( !win )
	{
		GreDebug( "[WARN] XCreateWindow() failed." ) << Gre::gendl ;
		return Gre::WindowHolder ( nullptr ) ;
	}

	XFree ( vi ) ;
	GreDebug ( "[INFO] Window '" ) << windowtitle << "' created." << Gre::gendl ;

    Atom wmDelete ;

    //////////////////////////////////////////////////////////////////////
    // Grabs pointer and keyboard for window only in fullscreen mode.

    if ( needsfs )
    {
        XWarpPointer( display , None , win , 0, 0, 0, 0, 0, 0 ) ;
        XGrabKeyboard( display , win , True , GrabModeAsync , GrabModeAsync , CurrentTime ) ;
        XGrabPointer( display , win , True , ButtonPressMask , GrabModeAsync , GrabModeAsync , win , None , CurrentTime ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Else , stores an atomic variable to manage delete event.

    else
    {
        wmDelete = XInternAtom( display , "WM_DELETE_WINDOW" , True ) ;
        XSetWMProtocols( display , win , &wmDelete , 1 ) ;
    }

	XStoreName ( display, win, windowtitle.c_str() ) ;
	XMapRaised ( display, win ) ;

	//////////////////////////////////////////////////////////////////////
	// Creates GLX Context.

	GLXContext ctxt = CreateGlXContext ( display, bestFbc, glmajor, glminor, options ) ;

	if ( !ctxt )
	{
		GreDebug( "[WARN] GlX Context failed." ) << Gre::gendl ;

		XDestroyWindow ( display , win ) ;
		XFreeColormap ( display , cmap ) ;
		return Gre::WindowHolder ( nullptr ) ;
	}

	//////////////////////////////////////////////////////////////////////
	// Creates X11OpenGlWindow.

	X11OpenGlWindow* x11win = new X11OpenGlWindow ( name , display , win , ctxt , cmap , deskmode , needsfs , wmDelete , windowtitle , options ) ;

	if ( !x11win )
	{
		DestroyGlXContext ( display , ctxt ) ;
		XDestroyWindow ( display , win ) ;
		XFreeColormap ( display , cmap ) ;
		return Gre::WindowHolder ( nullptr ) ;
	}

	GreDebug( "[INFO] X11OpenGlWindow '" ) << windowtitle << "' created." << Gre::gendl ;
	return Gre::WindowHolder ( x11win ) ;
}

Gre::ResourceLoader* X11OpenGlWindowLoader::clone() const
{
	return new X11OpenGlWindowLoader () ;
}

bool X11OpenGlWindowLoader::isLoadable( const std::string& ) const
{
	return false ;
}
