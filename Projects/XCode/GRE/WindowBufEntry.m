//
//  WindowBufEntry.m
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#define OSXFUNCTIONS_OBJC
#include "OSXFunctions.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

/// @brief Init every Window Entries in order to be able
/// to call anyone of them.
void InitWindowEntry()
{
    for(int i = 0; i < WINDOW_MAX; ++i)
    {
        WindowBufEntry nsWindowEntry;
        nsWindowEntry.window = nil;
        nsWindowEntry.closed = true;
        nsWindowEntry.exposed = false;
        nsWindowEntry.keybuf_sz = 0;
        nsWindowBufs[i] = nsWindowEntry;
    }
}

/// @brief Find a WindowEntry depending on given object's id.
WindowBufEntry* FindWindowEntry(id nsWindow)
{
    for(int i = 0; i < WINDOW_MAX; ++i)
    {
        if(nsWindow == nsWindowBufs[i].window)
        {
            return & (nsWindowBufs[i]);
        }
    }
    
    return NULL;
}

/// @brief Find the next available window entry, or return NULL.
WindowBufEntry* FindNextAvailableWindowEntry()
{
    for(int i = 0; i < WINDOW_MAX; ++i)
    {
        if(nsWindowBufs[i].window == nil)
            return & (nsWindowBufs[i]);
    }
    
    return NULL;
}
