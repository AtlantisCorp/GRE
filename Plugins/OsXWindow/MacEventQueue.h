//////////////////////////////////////////////////////////////////////
//
//  MacEventQueue.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/10/2016.
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

#ifndef MacEventQueue_h
#define MacEventQueue_h

#ifndef OSXFUNCTIONS_OBJC

#   include <stdio.h>
#   include <stdlib.h>
#   include <stddef.h>
#   include <string.h>
#
#   define BUILDING_DLL
#   include "ResourceManager.h"

using namespace Gre;
typedef const void* CFTypeRef ;

#endif // OSXFUNCTIONS_OBJC

#ifdef OSXFUNCTIONS_OBJC
#   import <Foundation/Foundation.h>
#   import <Cocoa/Cocoa.h>
#endif

#include <OpenGL/OpenGL.h>

/// Obj-C - Gre Events Queue prototypes.

typedef enum WindowEventType
{
    WETWindowDidMove ,
    WETWindowDidResize ,
    WETWindowKeyPressed ,
    WETWindowKeyReleased ,
    WETWindowExposed ,
    WETWindowWillClose ,
    WETWindowTitleChanged ,
    WETWindowHidden
    
} WindowEventType ;

typedef struct __winevent_t
{
    /// @brief The Window sender CFTypeRef.
    CFTypeRef WindowSender ;
    
    /// @brief The event type.
    WindowEventType WinEventType ;
    
} WindowEventBase ;

typedef struct __winevent_didmove_t
{
    struct __winevent_t base ;
    
    /// @brief The new X coordinate.
    int x;
    
    /// @brief The new Y coordinate.
    int y;
    
} WindowEventDidMove ;

typedef struct __winevent_didresize_t
{
    struct __winevent_t base ;
    
    /// @brief The new Width.
    int width;
    
    /// @brief The new Height.
    int height;
    
} WindowEventDidResize ;

typedef struct __winevent_keypressed_t
{
    struct __winevent_t base ;
    
    /// @brief The Key id .
    int key ;
    
} WindowEventKeyPressed ;

typedef struct __winevent_keyreleased_t
{
    struct __winevent_t base ;
    
    /// @brief The Key id .
    int key ;
    
} WindowEventKeyReleased ;

typedef struct __winevent_exposed_t
{
    struct __winevent_t base ;
    
    /// @brief The new surface .
    int width ; int height ; int x ; int y ;
    
} WindowEventExposed ;

typedef struct __winevent_titlechanged_t
{
    struct __winevent_t base ;
    
    /// @brief New Window title .
    char* title ;
    /// @brief Title buffer size .
    uint32_t size ;
    
} WindowEventTitleChanged ;

typedef struct __winevent_hidden_t
{
    struct __winevent_t base ;
    
} WindowEventHidden ;

/// @brief An entry in the Event Queue.
struct __winevent_queue_entry_t
{
    /// @brief Pointer to the event.
    WindowEventBase* eventptr ;
    
    /// @brief Pointer to the next event.
    struct __winevent_queue_entry_t * nextentry ;
    
    /// @brief Pointer to the preceding event.
    struct __winevent_queue_entry_t * lastentry ;
};

typedef struct __winevent_queue_t
{
    /// @brief Event's first pushed entry.
    struct __winevent_queue_entry_t * firstentry ;
    
    /// @brief Event's last pushed entry.
    struct __winevent_queue_entry_t * lastentry ;
    
    /// @brief Obvious member to count remaining events.
    uint32_t size ;
    
    /// @brief Atomic mutex in order to see if we can use the Queue or not .
    bool canuse ;
    
} WindowEventQueue ;

/// @brief Pointer to a global Event Queue initialized at the start of the Plugin .

extern WindowEventQueue * WindowGlobalQueue ;

#ifndef OSXFUNCTIONS_OBJC

/// @brief Creates a new Event Queue.

/// This should be called by DarwinWindow when creating the object, then call NsWindowSetEventQueue with this
/// objet to set the new event queue. Or , if no queue is set , the global event queue will be used. The Global
/// event queue is created by the plugin .

extern "C" WindowEventQueue * EventQueueCreate ( void ) ;

/// @brief Destroys an Event Queue.

/// Should be called either when destroying the plugin ( for the global queue ) , either when the DarwinWindow
/// objects destroys itself. The Obj-C window should never destroy the event queue.

extern "C" void EventQueueDestroy ( WindowEventQueue* queue ) ;

/// @brief Allocates a new 'struct __winevent_queue_entry_t' and pushes it with the event to the queue.

extern "C" void EventQueuePush ( WindowEventQueue* queue , WindowEventBase* event ) ;

/// @brief Returns the first event in the queue corresponding to given window CFTypeRef . If 'remove' is true,
/// also removes it from the queue .

extern "C" WindowEventBase * EventQueueGet ( WindowEventQueue* queue , const CFTypeRef window , bool remove ) ;

/// @brief Returns the first event in the queue . If 'remove' is true, also removes it from the queue .

extern "C" WindowEventBase * EventQueueGetFirst ( WindowEventQueue* queue , bool remove ) ;

/// @brief Removes the first event in the queue . It also destroys the event object .

extern "C" void EventQueuePop ( WindowEventQueue* queue ) ;

/// @brief Clears the queue . This also destroys every events in this Queue .

extern "C" void EventQueueClear ( WindowEventQueue* queue ) ;

#endif

#endif /* MacEventQueue_h */
