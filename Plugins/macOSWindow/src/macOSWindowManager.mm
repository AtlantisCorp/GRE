//////////////////////////////////////////////////////////////////////
//
//  macOSWindowManager.mm
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

#include "macOSWindowManager.h"

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

// Delegate for NSApplication related notifications.

@interface macOSApplicationDelegate : NSObject
{
    macOSWindowManager* _manager ;
}

- (id) initWithWindowManager:(macOSWindowManager*)manager ;

@end

@implementation macOSApplicationDelegate

- (id) initWithWindowManager:(macOSWindowManager *)manager
{
    self = [super init];
    if ( self != nil )
        _manager = manager ;
    
    return self ;
}

- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*)sender
{
    if ( _manager != nil )
    {
        _manager->closeWindows () ;
    }
    
    return NSTerminateCancel ;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp stop:nil];
    macOSSendEmptyEvent () ;
}

@end

@interface macOSApplication : NSApplication
@end

@implementation macOSApplication

// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
// This works around an AppKit bug, where key up events while holding
// down the command key don't get sent to the key window.
- (void)sendEvent:(NSEvent *)event
{
    if ([event type] == NSEventTypeKeyUp &&
        ([event modifierFlags] & NSEventModifierFlagCommand))
    {
        [[self keyWindow] sendEvent:event];
    }
    else
        [super sendEvent:event];
}


// No-op thread entry point
- (void)doNothing:(id)object
{
    
}

@end

// Events functions

/// @brief Sends an empty event using the current shared application.
void macOSSendEmptyEvent ()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
    [pool drain];
}

void initializeAppkit (macOSWindowManager* manager)
{
    if ( NSApp )
        return ;
    
    // Initialize Autorelease Pool.
    manager->iAutorelease = [[NSAutoreleasePool alloc] init];
    
    // Implicitly initialize the application object.
    [macOSApplication sharedApplication];
    
    // Make Cocoa enter multithreaded mode.
    [NSThread detachNewThreadSelector:@selector(doNothing:)
                             toTarget:NSApp
                           withObject:nil];
    
    // In case we are unbundled, make us a proper UI application
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    // Allocate an Application Delegate.
    manager->iDelegate = [[macOSApplicationDelegate alloc] initWithWindowManager:manager];
    [NSApp setDelegate:manager->iDelegate];
    [NSApp run];
}

void terminateAppkit ( macOSWindowManager* manager )
{
    if ( manager->iDelegate )
    {
        [NSApp setDelegate:nil];
        [manager->iDelegate release];
        manager->iDelegate = nil ;
    }
    
    [manager->iAutorelease release];
    manager->iAutorelease = nil ;
}

void macOSPollEvents ( macOSWindowManager* manager )
{
    for (;;)
    {
        NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
        if (event == nil)
            break;
        
        [NSApp sendEvent:event];
    }
    
    [manager->iAutorelease drain];
    manager->iAutorelease = [[NSAutoreleasePool alloc] init];
}



