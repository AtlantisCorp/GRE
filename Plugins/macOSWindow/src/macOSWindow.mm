//////////////////////////////////////////////////////////////////////
//
//  macOSWindow.mm
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

#include "macOSWindow.h"

// HACK: The 10.12 SDK adds new symbols and immediately deprecates the old ones
#if MAC_OS_X_VERSION_MAX_ALLOWED < 101200
#   define NSWindowStyleMaskBorderless NSBorderlessWindowMask
#   define NSWindowStyleMaskClosable NSClosableWindowMask
#   define NSWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
#   define NSWindowStyleMaskResizable NSResizableWindowMask
#   define NSWindowStyleMaskTitled NSTitledWindowMask
#   define NSWindowStyleMaskFullScreen NSFullScreenWindowMask
#   define NSEventModifierFlagCommand NSCommandKeyMask
#   define NSEventModifierFlagControl NSControlKeyMask
#   define NSEventModifierFlagOption NSAlternateKeyMask
#   define NSEventModifierFlagShift NSShiftKeyMask
#   define NSEventModifierFlagDeviceIndependentFlagsMask NSDeviceIndependentModifierFlagsMask
#   define NSEventMaskAny NSAnyEventMask
#   define NSEventTypeApplicationDefined NSApplicationDefined
#   define NSEventTypeKeyUp NSKeyUp
#endif

// ---------------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// @brief Translates Gre::WindowStyle to NSWindowStyleMask .
int translateWindowStyle ( const WindowStyles & styles )
{
    int result = 0 ;

    if ( std::find(styles.begin(), styles.end(), WindowStyle::Fullscreen) != styles.end() ) result |= NSWindowStyleMaskFullScreen ;
    if ( std::find(styles.begin(), styles.end(), WindowStyle::Borderless) != styles.end() ) result |= NSWindowStyleMaskBorderless ;
    if ( std::find(styles.begin(), styles.end(), WindowStyle::Titled) != styles.end() ) result |= NSWindowStyleMaskTitled ;
    if ( std::find(styles.begin(), styles.end(), WindowStyle::Closable) != styles.end() ) result |= NSWindowStyleMaskClosable ;
    if ( std::find(styles.begin(), styles.end(), WindowStyle::Miniaturizable) != styles.end() ) result |= NSWindowStyleMaskMiniaturizable ;
    if ( std::find(styles.begin(), styles.end(), WindowStyle::Resizable) != styles.end() ) result |= NSWindowStyleMaskResizable ;

    return result ;
}

//////////////////////////////////////////////////////////////////////
// @brief Translates Gre::WindowContextAttribute to NSOpenGLPixelFormatAttribute .
void translatePixelFormat ( NSOpenGLPixelFormatAttribute* pf , size_t max , const WindowContextAttributes & attr )
{
    if ( !max || !pf )
    return ;

    int counter = 0 ;

    int major = 3 ;
    int minor = 2 ;

    //////////////////////////////////////////////////////////////////////
    // Checks every attributes, except WCAMajorVersion and WCAMinorVersion.

    for ( WindowContextAttributes::const_iterator it = attr.begin() ; it != attr.end() ; it++ )
    {
        switch ( (*it) )
        {
            case Gre::WCADoubleBuffer:
                pf[counter++] = NSOpenGLPFADoubleBuffer ;
                break ;

            case Gre::WCAStereo:
                pf[counter++] = NSOpenGLPFAStereo ;
                break ;

            case Gre::WCAAccelerated:
                pf[counter++] = NSOpenGLPFAAccelerated ;
                break ;

            case Gre::WCAMultiSample:
                pf[counter++] = NSOpenGLPFAMultisample ;
                break ;

            case Gre::WCASuperSample:
                pf[counter++] = NSOpenGLPFASupersample ;
                break ;

            case Gre::WCASampleBuffers:
                pf[counter++] = NSOpenGLPFASampleBuffers ;
                it++ ; pf[counter++] = (*it) ;
                break ;

            case Gre::WCASamples:
                pf[counter++] = NSOpenGLPFASamples ;
                it++ ; pf[counter++] = (*it) ;
                break ;

            case Gre::WCADepthSize:
                pf[counter++] = NSOpenGLPFADepthSize ;
                it++ ; pf[counter++] = (*it) ;
                break ;

            case Gre::WCAColorSize:
                pf[counter++] = NSOpenGLPFAColorSize ;
                it++ ; pf[counter++] = (*it) ;
                break ;

            case Gre::WCAStencilSize:
                pf[counter++] = NSOpenGLPFAStencilSize ;
                it++ ; pf[counter++] = (*it) ;
                break ;

            case Gre::WCATripleBuffer:
                pf[counter++] = NSOpenGLPFATripleBuffer ;
                break ;

            case Gre::WCAMajorVersion:
                it ++ ; major = (int) (*it) ;
                break ;

            case Gre::WCAMinorVersion:
                it ++ ; minor = (int) (*it) ;
                break ;

            case Gre::WCAFullscreen:
                GreDebug ( "[INFO] Gre::WCAFullscreen attribute not used on this platform." ) << Gre::gendl ;
                break ;

            default:
                break ;
        }

        if ( counter == max -1 )
        break ;
    }

    //////////////////////////////////////////////////////////////////////
    // Checks WCAMajorVersion and WCAMinorVersion. By default, we take
    // the OpenGl 3.2 Core Version. On MAC OS X , only versions 3.2 and
    // 4.1 profiles are supported.

    if ( counter < max - 2 )
    {
        if ( major == 3 && minor == 2 )
        {
            pf[counter++] = NSOpenGLPFAOpenGLProfile ;
            pf[counter++] = NSOpenGLProfileVersion3_2Core ;
        }

        else if ( major == 4 && minor == 1 )
        {
            pf[counter++] = NSOpenGLPFAOpenGLProfile ;
            pf[counter++] = NSOpenGLProfileVersion4_1Core ;
        }

        else
        GreDebug ( "[WARN] Requested OpenGl Version not supported (") << major << "." << minor << ")." << gendl ;
    }

    //////////////////////////////////////////////////////////////////////
    // Check if we can set the NSOpenGLOFAClosestPolicy.

    if ( counter < max )
    pf[counter++] = NSOpenGLPFAClosestPolicy ;

    pf[counter++] = 0 ;
}

// ---------------------------------------------------------------------------------------------------------
// KeyCodes translation.

#define MAX_SCANCODES 0xFF

Key * keycodes = nullptr ;
unsigned int * scancodes = nullptr ;

void initKeycodes ()
{
    keycodes = (Key*) malloc ( MAX_SCANCODES * sizeof(Key) ) ;
    memset ( keycodes , 0 , MAX_SCANCODES * sizeof(Key) ) ;

    scancodes = (unsigned int*) malloc ( MAX_SCANCODES * sizeof(unsigned int) ) ;
    memset ( scancodes , 0 , MAX_SCANCODES * sizeof(unsigned int) ) ;

    keycodes[0x1D] = Key::K0;
    keycodes[0x12] = Key::K1;
    keycodes[0x13] = Key::K2;
    keycodes[0x14] = Key::K3;
    keycodes[0x15] = Key::K4;
    keycodes[0x17] = Key::K5;
    keycodes[0x16] = Key::K6;
    keycodes[0x1A] = Key::K7;
    keycodes[0x1C] = Key::K8;
    keycodes[0x19] = Key::K9;
    keycodes[0x00] = Key::A;
    keycodes[0x0B] = Key::B;
    keycodes[0x08] = Key::C;
    keycodes[0x02] = Key::D;
    keycodes[0x0E] = Key::E;
    keycodes[0x03] = Key::F;
    keycodes[0x05] = Key::G;
    keycodes[0x04] = Key::H;
    keycodes[0x22] = Key::I;
    keycodes[0x26] = Key::J;
    keycodes[0x28] = Key::K;
    keycodes[0x25] = Key::L;
    keycodes[0x2E] = Key::M;
    keycodes[0x2D] = Key::N;
    keycodes[0x1F] = Key::O;
    keycodes[0x23] = Key::P;
    keycodes[0x0C] = Key::Q;
    keycodes[0x0F] = Key::R;
    keycodes[0x01] = Key::S;
    keycodes[0x11] = Key::T;
    keycodes[0x20] = Key::U;
    keycodes[0x09] = Key::V;
    keycodes[0x0D] = Key::W;
    keycodes[0x07] = Key::X;
    keycodes[0x10] = Key::Y;
    keycodes[0x06] = Key::Z;

    keycodes[0x27] = Key::Apostrophe;
    keycodes[0x2A] = Key::Backslash;
    keycodes[0x2B] = Key::Comma;
    keycodes[0x18] = Key::Equal;
    keycodes[0x32] = Key::GraveAccent;
    keycodes[0x21] = Key::LeftBracket;
    keycodes[0x1B] = Key::Minus;
    keycodes[0x2F] = Key::Period;
    keycodes[0x1E] = Key::RightBracket;
    keycodes[0x29] = Key::Semicolon;
    keycodes[0x2C] = Key::Slash;
    keycodes[0x0A] = Key::World1;

    keycodes[0x33] = Key::Backspace;
    keycodes[0x39] = Key::CapsLock;
    keycodes[0x75] = Key::Delete;
    keycodes[0x7D] = Key::Down;
    keycodes[0x77] = Key::End;
    keycodes[0x24] = Key::Enter;
    keycodes[0x35] = Key::Escape;
    keycodes[0x7A] = Key::F1;
    keycodes[0x78] = Key::F2;
    keycodes[0x63] = Key::F3;
    keycodes[0x76] = Key::F4;
    keycodes[0x60] = Key::F5;
    keycodes[0x61] = Key::F6;
    keycodes[0x62] = Key::F7;
    keycodes[0x64] = Key::F8;
    keycodes[0x65] = Key::F9;
    keycodes[0x6D] = Key::F10;
    keycodes[0x67] = Key::F11;
    keycodes[0x6F] = Key::F12;
    keycodes[0x69] = Key::F13;
    keycodes[0x6B] = Key::F14;
    keycodes[0x71] = Key::F15;
    keycodes[0x6A] = Key::F16;
    keycodes[0x40] = Key::F17;
    keycodes[0x4F] = Key::F18;
    keycodes[0x50] = Key::F19;
    keycodes[0x5A] = Key::F20;
    keycodes[0x73] = Key::Home;
    keycodes[0x72] = Key::Insert;
    keycodes[0x7B] = Key::Left;
    keycodes[0x3A] = Key::LeftAlt;
    keycodes[0x3B] = Key::LeftControl;
    keycodes[0x38] = Key::LeftShift;
    keycodes[0x37] = Key::LeftSuper;
    keycodes[0x6E] = Key::Menu;
    keycodes[0x47] = Key::NumLock;
    keycodes[0x79] = Key::PageDown;
    keycodes[0x74] = Key::PageUp;
    keycodes[0x7C] = Key::Right;
    keycodes[0x3D] = Key::RightAlt;
    keycodes[0x3E] = Key::RightControl;
    keycodes[0x3C] = Key::RightShift;
    keycodes[0x36] = Key::RightSuper;
    keycodes[0x31] = Key::Space;
    keycodes[0x30] = Key::Tab;
    keycodes[0x7E] = Key::Up;

    keycodes[0x52] = Key::KP0;
    keycodes[0x53] = Key::KP1;
    keycodes[0x54] = Key::KP2;
    keycodes[0x55] = Key::KP3;
    keycodes[0x56] = Key::KP4;
    keycodes[0x57] = Key::KP5;
    keycodes[0x58] = Key::KP6;
    keycodes[0x59] = Key::KP7;
    keycodes[0x5B] = Key::KP8;
    keycodes[0x5C] = Key::KP9;
    keycodes[0x45] = Key::KPAdd;
    keycodes[0x41] = Key::KPDecimal;
    keycodes[0x4B] = Key::KPDivide;
    keycodes[0x4C] = Key::KPEnter;
    keycodes[0x51] = Key::KPEqual;
    keycodes[0x43] = Key::KPMultiply;
    keycodes[0x4E] = Key::KPSubstract;

    for ( int scancode = 0 ; scancode < MAX_SCANCODES ; scancode++ )
    {
        if ( (int) keycodes[scancode] > 0 )
        scancodes[(int)keycodes[scancode]] = scancode ;
    }
}

// ---------------------------------------------------------------------------------------------------------
// Keys functions ( translateKey , translateFlags )

Key translateKey ( unsigned int key )
{
    if ( !keycodes )
        initKeycodes () ;

    return keycodes[key] ;
}

int translateFlags ( NSUInteger flags )
{
    int mods = 0 ;

    if ( flags & NSEventModifierFlagShift )
        mods |= (int) KeyModifier::Shift ;
    if ( flags & NSEventModifierFlagControl )
        mods |= (int) KeyModifier::Control ;
    if ( flags & NSEventModifierFlagOption )
        mods |= (int) KeyModifier::Alt ;
    if ( flags & NSEventModifierFlagCommand )
        mods |= (int) KeyModifier::Super ;

    return mods ;
}

// ---------------------------------------------------------------------------------------------------------
// nsWindowDelegate

@interface nsWindowDelegate : NSObject
{
    macWindow* _window ;
}

- (id) initWithWindow:(macWindow*)window ;

@end

@implementation nsWindowDelegate

- (id) initWithWindow:(macWindow *)window
{
    self = [super init];
    if ( self != nil )
        _window = window ;

    return self ;
}

- (BOOL) windowShouldClose:(id)sender
{
    _window->notifiateWindowShouldClose () ;
    return TRUE ;
}

- (void) windowDidResize:(NSNotification*)notification
{
    NSRect esurface = [[notification object] frame] ;
    _window->notifiateSize( esurface.size.width , esurface.size.height ) ;
}

- (void) windowDidMove:(NSNotification*)notification
{
    NSRect pos = [[notification object] contentRectForFrameRect:[[notification object] frame] ];
    _window->notifiatePosition ( pos.origin.x , pos.origin.y ) ;
}

// [03.01.2017] NOTES : NSWindowDidExposeNotification is sent only to 'nunretained' NSWindow
// subclass. As we need a buffered window, this notification will not be send. We can use
// NSWindowDidChangeOcclusionState to see if the window is still visible. Then, we update
// the frame size with NSWindow's frame size.

/*
- (void) windowDidExpose:(NSNotification*)notification
{
    NSRect frame = [[notification object] frame] ;
    _window -> notifiateSize ( frame.size.width, frame.size.height ) ;
}
 */

- (void) windowDidChangeOcclusionState:(NSNotification*) notification
{
    NSWindow* win = notification.object ;

    if ( win.occlusionState & NSWindowOcclusionStateVisible )
    {
        NSRect frame = [[notification object] frame] ;
        _window -> notifiateSize ( frame.size.width, frame.size.height ) ;
    }
}

@end

// ---------------------------------------------------------------------------------------------------------
// nsWindow class

@interface nsWindow : NSWindow
@end

@implementation nsWindow

- (BOOL) canBecomeKeyWindow
{
    return YES ;
}

@end

// ---------------------------------------------------------------------------------------------------------
// nsWindowContentView

@interface nsWindowContentView : NSView
{
    macWindow* window ;
    NSTrackingArea* trackingArea ;
}

- (id) initWithWindow: (macWindow*) win ;

@end

@implementation nsWindowContentView

- (id) initWithWindow:(macWindow *)win
{
    self = [super init];
    if ( self != nil )
    {
        window = win ;
        trackingArea = nil ;
    }

    return self ;
}

- (void) dealloc
{
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES ;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void) mouseDown:(NSEvent *)theEvent
{
    window->notifiateMouseDown () ;
}

- (void) mouseUp:(NSEvent *)theEvent
{
    window->notifiateMouseUp () ;
}

- (void) rightMouseDown:(NSEvent *)theEvent
{
    window->notifiateRightMouseDown () ;
}

- (void) rightMouseUp:(NSEvent *)theEvent
{
    window->notifiateRightMouseUp () ;
}

- (void) mouseExited:(NSEvent *)theEvent
{
    window->notifiateMouseExited () ;
}

- (void) mouseEntered:(NSEvent *)theEvent
{
    window->notifiateMouseEntered () ;
}

-(void) updateTrackingAreas
{
    if ( trackingArea != nil ) {
        [self removeTrackingArea:trackingArea] ;
        [trackingArea release] ;
    }

    int opts = (NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways);
    trackingArea = [ [NSTrackingArea alloc] initWithRect:[self bounds]
                                                 options:opts
                                                   owner:self
                                                userInfo:nil];
    [self addTrackingArea:trackingArea];
}

- (void) mouseMoved:(NSEvent *)theEvent
{
    window -> notifiateMouseMoved ( theEvent.deltaX , theEvent.deltaY ) ;
}

- (void) keyDown:(NSEvent *)theEvent
{
    const Key key = translateKey([theEvent keyCode]) ;
    const int mods = translateFlags([theEvent modifierFlags]) ;

    window->notifiateKeyDown ( key , mods ) ;
}

- (void) keyUp:(NSEvent *)theEvent
{
    const Key key = translateKey([theEvent keyCode]) ;
    const int mods = translateFlags([theEvent modifierFlags]) ;

    window->notifiateKeyUp ( key , mods ) ;
}

@end

// ---------------------------------------------------------------------------------------------------------
// macWindow C++ Class

macWindow::macWindow ( const std::string & name , const ResourceLoaderOptions& options )
: Gre::Window ( name , options ) , context ( nullptr )
{
    nsWindow = nil ;
    nsDelegate = nil ;
    nsglContext = nullptr ;
    view = nil ;
}

macWindow::~macWindow () noexcept ( false )
{

}

void macWindow::bind () const
{
    if ( nsglContext )
        nsglContext->bind () ;
}

void macWindow::unbind () const
{
    if ( nsglContext )
        nsglContext->unbind() ;
}

void macWindow::swapBuffers() const
{
    if ( nsglContext )
        nsglContext->flush() ;
}

bool macWindow::holdsRenderContext() const
{
    return nsglContext != nullptr ;
}

bool macWindow::isAvailableForDrawing() const
{
    return true ;
}

RenderContextHolder macWindow::getRenderContext()
{
    return context ;
}

const RenderContextHolder macWindow::getRenderContext() const
{
    return context ;
}

void macWindow::toggleCursor ( bool hide )
{
    if ( hide ) {
        [NSCursor hide];
    } else {
        [NSCursor unhide];
    }
}

void macWindow::centerCursor() const
{
    const NSRect localRect = NSMakeRect(iSurface.width/2, iSurface.height/2 - 1, 0, 0);
    const NSRect globalRect = [nsWindow convertRectToScreen:localRect];
    const NSPoint globalPoint = globalRect.origin ;
    CGWarpMouseCursorPosition(CGPointMake( globalPoint.x , CGDisplayBounds(CGMainDisplayID()).size.height - globalPoint.y ));
}

void macWindow::notifiateWindowShouldClose()
{
    EventHolder e = EventHolder ( new WindowWillCloseEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateSize(int width, int height)
{
    if ( nsglContext != nullptr )
        [nsglContext->nsglContext update] ;

    iSurface.width = width ;
    iSurface.height = height ;

    EventHolder e = EventHolder ( new WindowSizedEvent(this , width , height) ) ;
    sendEvent(e);
}

void macWindow::notifiatePosition(int x, int y)
{
    iSurface.top = y ;
    iSurface.left = x ;

    EventHolder e = EventHolder ( new WindowMovedEvent(this, x, y) ) ;
    sendEvent(e);
}

void macWindow::notifiateMouseDown()
{
    EventHolder e = EventHolder ( new LeftMousePressEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateMouseUp()
{
    EventHolder e = EventHolder ( new LeftMouseReleaseEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateRightMouseDown()
{
    EventHolder e = EventHolder ( new RightMousePressEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateRightMouseUp()
{
    EventHolder e = EventHolder ( new RightMouseReleaseEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateMouseExited ()
{
    EventHolder e = EventHolder ( new MouseExitedWindowEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateMouseEntered()
{
    EventHolder e = EventHolder ( new MouseEnteredWindowEvent(this) ) ;
    sendEvent(e);
}

void macWindow::notifiateKeyDown(const Gre::Key &key, int mods)
{
    EventHolder e = EventHolder ( new KeyDownEvent(this, key, mods) ) ;
    sendEvent(e);
}

void macWindow::notifiateKeyUp(const Gre::Key &key, int mods)
{
    EventHolder e = EventHolder ( new KeyUpEvent(this, key, mods) ) ;
    sendEvent(e);
}

void macWindow::notifiateMouseMoved ( float dx , float dy )
{
    EventHolder e = EventHolder ( new CursorMovedEvent(this, dx, dy) ) ;
    sendEvent(e);
}

void macWindow::_setTitle ( const std::string & title ) const
{
    [nsWindow setTitle:[NSString stringWithUTF8String:title.c_str()]] ;
}

// ---------------------------------------------------------------------------------------------------------
// Creates an NSOpenGLContext and loads it to the macOSWindow object.

void CreateContextForWindow ( macWindow* window , NSOpenGLPixelFormatAttribute* attributes )
{
    if ( !attributes )
    return ;

    window->nsglContext = new nsContext ( "" , RenderContextInfo() , nil , nil ) ;
    if ( !window->nsglContext )
    {
        GreDebug("[WARN] Can't create nsContext.") << Gre::gendl ;
        return ;
    }

    window->nsglContext->nsglPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    if ( window->nsglContext->nsglPixelFormat == nil )
    {
        GreDebug("[WARN] Can't create NSOpenGLPixelFormat.") << Gre::gendl ;

        delete window->nsglContext ;
        window->nsglContext = nullptr ;

        return ;
    }

    window->nsglContext->nsglContext = [[NSOpenGLContext alloc] initWithFormat:window->nsglContext->nsglPixelFormat
                                                                  shareContext:nil];
    if ( window->nsglContext->nsglContext == nil )
    {
        GreDebug("[WARN] Can't create NSOpenGLContext.") << Gre::gendl ;

        [window->nsglContext->nsglPixelFormat release];
        window->nsglContext->nsglPixelFormat = nil ;
        delete window->nsglContext;
        window->nsglContext = nullptr ;

        return ;
    }

    [window->nsglContext->nsglContext setView:window->view];
    return ;
}

// ---------------------------------------------------------------------------------------------------------
// macOSWindowLoader C++

macOSWindowLoader::macOSWindowLoader ()
{

}

macOSWindowLoader::~macOSWindowLoader() noexcept ( false )
{

}

//////////////////////////////////////////////////////////////////////
// Returns the Window's size using the normal size format , or a default
// pair ( 1024 , 768 ). The size format is 'WidthXHeight' or 'WidthXHeight'.
// The Window's size is under the 'Window.Size' option.
std::pair < int , int > GetWindowSize ( const ResourceLoaderOptions & ops )
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
std::string GetWindowTitle ( const ResourceLoaderOptions & ops )
{
    auto titleit = ops.find ( "Window.Title" ) ;

    if ( titleit == ops.end() )
    return std::string () ;

    return titleit -> second.to<std::string> () ;
}

//////////////////////////////////////////////////////////////////////
// Returns the Window's style mask with given option. The default mask
// is Closable | Resizable | Miniaturizable | Titled.
int GetWindowMask ( const ResourceLoaderOptions & ops )
{
    int defaultmask =
        NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
        NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable ;

    auto maskit = ops.find ( "Window.Styles" ) ;

    if ( maskit == ops.end() )
    return defaultmask ;

    return translateWindowStyle ( maskit->second.to<WindowStyles>() ) ;
}

WindowHolder macOSWindowLoader::load(const std::string &name, const ResourceLoaderOptions &info) const
{
    if ( name.empty() )
    GreDebug("[WARN] Given name is empty when creating Window.");

    //////////////////////////////////////////////////////////////////////
    // Gets Window's Size.

    auto size = GetWindowSize ( info ) ;

    //////////////////////////////////////////////////////////////////////
    // Gets Window's title.

    std::string title = GetWindowTitle ( info ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the macOSWindow class.

    GreDebug("[INFO] Creating Mac OS X Window class.") << Gre::gendl ;

    macWindow* window = new macWindow ( name , info ) ;
    WindowHolder test ( window ) ;
    
    if ( !window )
    {
        GreDebug("[WARN] Can't create Window '") << name << "'." << Gre::gendl ;
        return WindowHolder ( nullptr ) ;
    }

    GreDebug("[INFO] Creating Mac OS X WindowDelegate class.") << Gre::gendl ;

    window->nsDelegate = [[nsWindowDelegate alloc] initWithWindow:window];

    if ( window->nsDelegate == nil )
    {
        GreDebug("[WARN] Can't associate Delegate to Window '") << name << "'." << Gre::gendl ;
        delete window ;
        return WindowHolder ( nullptr ) ;
    }

    GreDebug("[INFO] Creating NSWindow object.") << Gre::gendl ;

    //////////////////////////////////////////////////////////////////////
    // Guesses the Window's style mask and surface rect.

    int windowmask = GetWindowMask ( info ) ;
    NSRect contentrect = NSMakeRect(0, 0, size.first, size.second);

    window->nsWindow = [[nsWindow alloc]
        initWithContentRect:contentrect
        styleMask:windowmask
        backing:NSBackingStoreBuffered
        defer:NO];

    if ( window->nsWindow == nil )
    {
        GreDebug("[WARN] Can't create Window '") << name << "' object." << Gre::gendl ;
        [window->nsDelegate release];
        window->nsDelegate = nil ;
        delete window ;
        return WindowHolder ( nullptr ) ;
    }

    [window->nsWindow center];

    GreDebug("[INFO] Creating NSView object.") << Gre::gendl ;

    window->view = [[nsWindowContentView alloc] initWithWindow:window];

    if ( window->view == nil )
    {
        GreDebug("[WARN] Can't create Window View '") << name << "'." << Gre::gendl ;

        [window->nsWindow release];
        window->nsWindow = nil ;
        [window->nsDelegate release];
        window->nsDelegate = nil ;
        delete window ;

        return WindowHolder ( nullptr ) ;
    }

    [window->view setWantsBestResolutionOpenGLSurface:YES];

    [window->nsWindow setContentView:window->view];
    [window->nsWindow makeFirstResponder:window->view];
    [window->nsWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
    [window->nsWindow setDelegate:window->nsDelegate];
    [window->nsWindow setAcceptsMouseMovedEvents:YES];
    [window->nsWindow setRestorable:NO];

    //////////////////////////////////////////////////////////////////////
    // Guesses the NSOpenGLPixelFormat. Default attributes is Doublebuffer,
    // Accelerated, 24bits Depth size, ClosestPolicy and Version 3.2.

    NSOpenGLPixelFormatAttribute pf [40] ;
    memset(pf, 0, sizeof(NSOpenGLPixelFormatAttribute)*40);

    auto windowit = info.find ( "Window.ContextAttributes" ) ;

    if ( windowit != info.end () ) {
        translatePixelFormat ( & pf[0] , 40 , windowit->second.to<WindowContextAttributes>() ) ;
    }

    else
    {
        pf [0] = NSOpenGLPFADoubleBuffer ;
        pf [1] = NSOpenGLPFAAccelerated ;
        pf [2] = NSOpenGLPFADepthSize ; pf [3] = 24 ;
        pf [4] = NSOpenGLPFAOpenGLProfile ; pf [5] = NSOpenGLProfileVersion3_2Core ;
        pf [6] = NSOpenGLPFAClosestPolicy ;
        pf [7] = 0 ;
    }

    CreateContextForWindow ( window , pf ) ;

	if ( window->nsglContext == nil )
	{
        GreDebug("[WARN] Can't create NSOpenGLContext object.") << Gre::gendl ;

        [window->view release];
        window->view = nil ;
        [window->nsWindow release];
        window->nsWindow = nil ;
        [window->nsDelegate release];
        window->nsDelegate = nil ;
        delete window ;

        return WindowHolder ( nullptr ) ;
	}

    window->context = RenderContextHolder ( window->nsglContext ) ;

    [window->nsWindow makeKeyAndOrderFront:nil];
    [window->nsWindow makeMainWindow];

	GreDebug("[INFO] Created Mac OS X native Window '") << name << "'." << Gre::gendl ;
    return WindowHolder ( window ) ;
}

ResourceLoader* macOSWindowLoader::clone() const
{
    return new macOSWindowLoader () ;
}

bool macOSWindowLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}
