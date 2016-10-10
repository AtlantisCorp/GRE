//////////////////////////////////////////////////////////////////////
//
//  CustomWindow.m
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 01/04/2016.
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

#import "CustomWindow.h"

@implementation CustomWindow

/// @brief Returns the OpenGlCustomView associated.
- (OpenGlCustomView*) getGlView
{
    return _nsGlView;
}

/// @brief Sets the YsOpenGlView associated.
- (void) setGlView: (OpenGlCustomView*) glView
{
    [self setContentView:glView];
    [glView setEventQueue:iEventQueue];
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
    _nsGlView = nil;
    iIsInvisible = true ;
    
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
    if ( iEventQueue )
    {
        WindowEventDidMove * event = (WindowEventDidMove*) malloc( sizeof(WindowEventDidMove) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        event->base.WindowSender = self ;
        event->base.WinEventType = WETWindowDidMove ;
        event->x = _frame.origin.x;
        event->y = _frame.origin.y;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event );
    }
}

/// @brief Called when the Window did resize.
- (void) windowDidResize: (NSNotification *)notification
{
    if ( iEventQueue )
    {
        WindowEventDidResize * event = (WindowEventDidResize*) malloc( sizeof(WindowEventDidResize) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        event->base.WindowSender = self ;
        event->base.WinEventType = WETWindowDidResize ;
        event->height = _frame.size.height ;
        event->width = _frame.size.width ;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
}

/// @brief When Window will close, we immediatly notifiate the Entry
/// that this Window... well is closed.
- (void) windowWillClose: (NSNotification *)notification
{
    if ( iEventQueue )
    {
        WindowEventBase* event = (WindowEventBase*) malloc ( sizeof(WindowEventBase) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        event->WindowSender = self;
        event->WinEventType = WETWindowWillClose;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
}

/// @brief Notifiate the Window Entry that we are shown to the public.
- (void) windowExposed: (NSNotification *) notification
{
    if ( iEventQueue )
    {
        WindowEventExposed* event = (WindowEventExposed*) malloc ( sizeof(WindowEventExposed) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        event->base.WindowSender = self;
        event->base.WinEventType = WETWindowExposed;
        
        event->x = _frame.origin.x ;
        event->y = _frame.origin.y ;
        event->width = _frame.size.width ;
        event->height = _frame.size.height ;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
}

/// @brief When a Key is down, we add it to the Entry key queue.
/// This is the Gre::WindowPrivate object (here OsXWindow) who will
/// treat this Event.
- (void) keyDown:(NSEvent *)theEvent
{
    if ( iEventQueue )
    {
        WindowEventKeyPressed* event = (WindowEventKeyPressed*) malloc ( sizeof(WindowEventKeyPressed) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        char buf [256] ; [theEvent.characters getCString:buf maxLength:255 encoding:NSASCIIStringEncoding];
        
        event->base.WindowSender = self;
        event->base.WinEventType = WETWindowKeyPressed;
        event->key = (int) buf[0] ;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
    
    [super keyDown:theEvent];
}

/// @brief When a Key is up, we add it to the Entry key queue.
/// This is the Gre::WindowResource object (here OsXWindow) who will
/// treat this Event.
- (void) keyUp:(NSEvent *)theEvent
{
    if ( iEventQueue )
    {
        WindowEventKeyReleased* event = (WindowEventKeyReleased*) malloc ( sizeof(WindowEventKeyReleased) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        char buf [256] ; [theEvent.characters getCString:buf maxLength:255 encoding:NSASCIIStringEncoding];
        
        event->base.WindowSender = self;
        event->base.WinEventType = WETWindowKeyReleased;
        event->key = (int) buf[0] ;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
    
    [super keyUp:theEvent];
}

- (void) update
{
    [super update];
    
    // Try to see if Window is visible, or being hidden.
    
    if ( [self isVisible] && [self isOnActiveSpace] && iIsInvisible )
    {
        if ( iEventQueue )
        {
            WindowEventExposed* event = (WindowEventExposed*) malloc ( sizeof(WindowEventExposed) ) ;
            if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                         reason:@"'event' could not be allocated."
                                                       userInfo:nil];
            
            event->base.WindowSender = self;
            event->base.WinEventType = WETWindowExposed;
            
            event->x = _frame.origin.x ;
            event->y = _frame.origin.y ;
            event->width = _frame.size.width ;
            event->height = _frame.size.height ;
            
            EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
        }
        
        iIsInvisible = false;
    }
    
    else if ( !iIsInvisible )
    {
        if ( ![self isVisible] || ![self isOnActiveSpace] )
        {
            if ( iEventQueue )
            {
                WindowEventHidden* event = (WindowEventHidden*) malloc ( sizeof(WindowEventHidden) ) ;
                if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                             reason:@"'event' could not be allocated."
                                                           userInfo:nil];
                
                event->base.WindowSender = self;
                event->base.WinEventType = WETWindowHidden;
                
                EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
            }
            
            iIsInvisible = true;
        }
    }
}

- (void) setWindowEventQueue:(WindowEventQueue *)queue
{
    iEventQueue = queue ;
}

- (void) setTitle:(NSString *)title
{
    [super setTitle:title];
    
    if ( iEventQueue )
    {
        WindowEventTitleChanged* event = (WindowEventTitleChanged*) malloc ( sizeof(WindowEventTitleChanged) ) ;
        if ( !event ) @throw [NSException exceptionWithName:@"BadAllocException"
                                                     reason:@"'event' could not be allocated."
                                                   userInfo:nil];
        
        event->base.WindowSender = self;
        event->base.WinEventType = WETWindowTitleChanged;
        
        event->title = (char*) malloc ( [title length] + 1 ) ;
        memcpy(event->title, [title cStringUsingEncoding:NSASCIIStringEncoding], [title length]);
        event->title[ title.length ] = '\0' ;
        event->size = (int) title.length ;
        
        EventQueuePush ( iEventQueue , (WindowEventBase*) event ) ;
    }
}

@end

