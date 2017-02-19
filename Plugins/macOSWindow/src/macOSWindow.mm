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

macWindow::macWindow ( const std::string & name , const WindowOptions& options )
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

RenderContextUser macWindow::getRenderContext()
{
    return RenderContextUser ( context ) ;
}

const RenderContextUser macWindow::getRenderContext() const
{
    return RenderContextUser ( context ) ;
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

void macWindow::_setTitle ( const std::string & title ) const
{
    [nsWindow setTitle:[NSString stringWithUTF8String:title.c_str()]] ;
}

// ---------------------------------------------------------------------------------------------------------
// Creates an NSOpenGLContext and loads it to the macOSWindow object.

void CreateContextForWindow ( macWindow* window )
{
    NSOpenGLPixelFormatAttribute attributes[40] ;
    int attributeCount = 0 ;
    
#define ADD_ATTR(x) { attributes[attributeCount++] = x; }
#define ADD_ATTR2(x, y) { ADD_ATTR(x); ADD_ATTR(y); }
    
    ADD_ATTR(NSOpenGLPFAAccelerated);
    ADD_ATTR(NSOpenGLPFAClosestPolicy);
    
    ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
    ADD_ATTR(NSOpenGLPFADoubleBuffer);
    
    ADD_ATTR(0);
    
#undef ADD_ATTR
#undef ADD_ATTR2
    
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

WindowHolder macOSWindowLoader::load(const std::string &name, const WindowOptions &info) const
{
    if ( name.empty() ) {
        GreDebug("[WARN] Given name is empty when creating Window.");
    }
    
    auto it = info.find("Size");
    if ( it == info.end() ) {
        GreDebug("[WARN] Invalid size for Window '") << name << "'." << Gre::gendl ;
        return WindowHolder ( nullptr ) ;
    }
    
    std::vector<std::string> sizes = split((*it).second.toString(), 'x');
    std::pair<int, int> size = std::make_pair(strtol(sizes[0].c_str(), NULL, 10) , strtol(sizes[1].c_str(), NULL, 10));
    
    if ( size.first <= 0 || size.second <= 0 ) {
        GreDebug("[WARN] Invalid size format for Window '") << name << "'." << Gre::gendl ;
        return WindowHolder ( nullptr ) ;
    }
    
    it = info.find("Title");
    std::string title = "macOS Default Window" ;
    
    if ( it != info.end() )
        title = (*it).second.toString() ;
    
    // Create the macOSWindow class.
	GreDebug("[INFO] Creating Mac OS X Window class.") << Gre::gendl ;
    
    macWindow* window = new macWindow ( name , info ) ;
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
    
    NSRect contentrect = NSMakeRect(0, 0, size.first, size.second);
    window->nsWindow = [[nsWindow alloc]
                        initWithContentRect:contentrect
                        styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskResizable|NSWindowStyleMaskMiniaturizable
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
    
    CreateContextForWindow ( window ) ;
	
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
    
    [window->nsWindow orderFront:nil];
    
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


