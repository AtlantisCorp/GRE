//
//  CustomWindow.m
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#include "CustomWindow.h"

@implementation CustomWindow

/// @brief Returns the YsOpenGlView associated.
- (OpenGlCustomView*) getGlView
{
    return _nsGlView;
}

/// @brief Sets the YsOpenGlView associated.
- (void) setGlView: (OpenGlCustomView*) glView
{
    if(_nsGlView != nil)
    {
        [_nsGlView release];
        _nsGlView = nil;
    }
    _nsGlView = [glView retain];
    [self setContentView:_nsGlView];
    [self makeFirstResponder:_nsGlView];
}

/// @brief Returns the current WindowBufEntry.
- (WindowBufEntry*) getWindowEntry
{
    return _nsWindowEntry;
}

/// @brief Tells if the Window has been closed.
- (bool) isClosed
{
    return _nsIsClosed;
}

/// @brief Called when the Window is inited.
/// We must allocate a new WindowBufEntry in order to register the
/// Window.
- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg
{
    self = [super initWithContentRect:rect styleMask:wndStyle backing:bufferingType defer:deferFlg];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowDidResize:)
     name:NSWindowDidResizeNotification
     object:self];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowWillClose:)
     name:NSWindowWillCloseNotification
     object:self];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowExposed:)
     name:NSWindowDidExposeNotification
     object:self];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowDidMove:)
     name:NSWindowDidMoveNotification
     object:self];
    
    [self setAcceptsMouseMovedEvents:YES];
    //[self isReleasedWhenClosed] = YES;
    
    _nsWindowEntry = NULL;
    
    // Here we register the Window in an entry.
    WindowBufEntry* nextAvailableEntry = FindNextAvailableWindowEntry();
    if(nextAvailableEntry) {
        nextAvailableEntry->window = self;
        nextAvailableEntry->closed = false;
        nextAvailableEntry->exposed = false;
        _nsWindowEntry = nextAvailableEntry;
    }
    
    return self;
}

/// @brief Called when the Window did move.
- (void) windowDidMove: (NSNotification*) notification
{
    if(_nsWindowEntry)
    {
        _nsWindowEntry->sizeChanged = true;
        _nsWindowEntry->newWidth = _frame.size.width;
        _nsWindowEntry->newHeight = _frame.size.height;
        _nsWindowEntry->newX = _frame.origin.x;
        _nsWindowEntry->newY = _frame.origin.y;
        _nsWindowEntry->exposed = [self isVisible];
    }
}

/// @brief Called when the Window did resize.
- (void) windowDidResize: (NSNotification *)notification
{
    if(_nsWindowEntry)
    {
        _nsWindowEntry->sizeChanged = true;
        _nsWindowEntry->newWidth = _frame.size.width;
        _nsWindowEntry->newHeight = _frame.size.height;
        _nsWindowEntry->newX = _frame.origin.x;
        _nsWindowEntry->newY = _frame.origin.y;
        _nsWindowEntry->exposed = [self isVisible];
    }
}

/// @brief When Window will close, we immediatly notifiate the Entry
/// that this Window... well is closed.
- (void) windowWillClose: (NSNotification *)notification
{
    if(_nsWindowEntry) {
        _nsWindowEntry->closed = true;
        _nsWindowEntry->exposed = false;
        _nsWindowEntry->keybuf_sz = 0;
        // With this line, we 'free' the Entry. Any other Window which
        // will try to find an available Entry will find this one.
        _nsWindowEntry->window = nil;
    }
    
    _nsIsClosed = true;
}

/// @brief Notifiate the Window Entry that we are shown to the public.
- (void) windowExposed: (NSNotification *) notification
{
    if(_nsWindowEntry) {
        _nsWindowEntry->exposed = true;
        _nsWindowEntry->closed = false;
    }
    _nsIsClosed = false;
}

/// @brief When a Key is down, we add it to the Entry key queue.
/// This is the Gre::WindowPrivate object (here OsXWindow) who will
/// treat this Event.
- (void) keyDown:(NSEvent *)theEvent
{
    if(_nsWindowEntry)
    {
        if(_nsWindowEntry->keybuf_sz < KEYBUF_MAX)
        {
            struct keybuf_t nsNewKeyEvent;
            nsNewKeyEvent.key = theEvent.keyCode;
            nsNewKeyEvent.pressed = 1;
            _nsWindowEntry->keybufs[_nsWindowEntry->keybuf_sz] = nsNewKeyEvent;
            _nsWindowEntry->keybuf_sz++;
        }
    }
    else
    {
        [super keyDown:theEvent];
    }
}

/// @brief When a Key is up, we add it to the Entry key queue.
/// This is the Gre::WindowResource object (here OsXWindow) who will
/// treat this Event.
- (void) keyUp:(NSEvent *)theEvent
{
    if(_nsWindowEntry)
    {
        if(_nsWindowEntry->keybuf_sz < KEYBUF_MAX)
        {
            struct keybuf_t nsNewKeyEvent;
            nsNewKeyEvent.key = theEvent.keyCode;
            nsNewKeyEvent.pressed = 0;
            _nsWindowEntry->keybufs[_nsWindowEntry->keybuf_sz] = nsNewKeyEvent;
            _nsWindowEntry->keybuf_sz++;
        }
    }
    else
    {
        [super keyUp:theEvent];
    }
}

@end

