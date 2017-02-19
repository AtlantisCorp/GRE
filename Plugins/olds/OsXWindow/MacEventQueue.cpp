//////////////////////////////////////////////////////////////////////
//
//  MacEventQueue.cpp
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

#include "MacEventQueue.h"

WindowEventQueue * WindowGlobalQueue = nullptr ;

void lock_boolean ( bool& b )
{
    while ( !b ) ;
    b = false ;
}

void unlock_boolean ( bool& b )
{
    b = true ;
}

WindowEventQueue * EventQueueCreate ( void )
{
    // Here we allocating a new WindowEventQueue, and... that's all.
    
    WindowEventQueue * ret = new WindowEventQueue ;
    
    if ( !ret )
        return nullptr ;
    
    ret->firstentry = nullptr ;
    ret->lastentry = nullptr ;
    ret->size = 0 ;
    ret->canuse = true ;
    
    return ret ;
}

void EventQueueDestroy ( WindowEventQueue * queue )
{
    if ( queue )
    {
        EventQueueClear( queue ) ;
        delete queue ;
    }
}

void EventQueuePush ( WindowEventQueue* queue , WindowEventBase* event )
{
    if ( queue && event )
    {
        lock_boolean( queue->canuse ) ;
        {
            struct __winevent_queue_entry_t * entry = new struct __winevent_queue_entry_t ;
            
            entry->eventptr = event ;
            entry->nextentry = nullptr ;
            entry->lastentry = queue->lastentry ;
            
            if ( entry->lastentry )
            {
                entry->lastentry->nextentry = entry ;
            }
            
            queue->lastentry = entry ;
            queue->size = queue->size + 1 ;
            
            if ( !queue->firstentry )
            {
                queue->firstentry = entry ;
            }
        }
        unlock_boolean( queue->canuse ) ;
    }
}

WindowEventBase * EventQueueGet ( WindowEventQueue* queue , const CFTypeRef window , bool remove )
{
    if ( queue && window )
    {
        lock_boolean( queue->canuse ) ;
        {
            struct __winevent_queue_entry_t * current = queue->firstentry ;
            
            while ( current )
            {
                if ( current->eventptr )
                {
                    if ( current->eventptr->WindowSender == window )
                    {
                        WindowEventBase * retvalue = current->eventptr ;
                        
                        if ( remove )
                        {
                            if ( current->lastentry )
                                current->lastentry->nextentry = current->nextentry ;
                            if ( current->nextentry )
                                current->nextentry->lastentry = current->lastentry ;
                            if ( queue->lastentry == current )
                                queue->lastentry = current->lastentry ;
                            if ( queue->firstentry == current )
                                queue->firstentry = current->nextentry ;
                            
                            queue->size = queue->size - 1 ;
                            delete current ;
                        }
                        
                        unlock_boolean( queue->canuse ) ;
                        return retvalue ;
                    }
                }
                
                current = current->nextentry ;
            }
        }
        unlock_boolean( queue->canuse ) ;
    }
    
    return nullptr ;
}

WindowEventBase * EventQueueGetFirst ( WindowEventQueue* queue , bool remove )
{
    if ( queue )
    {
        lock_boolean( queue->canuse ) ;
        {
            struct __winevent_queue_entry_t * current = queue->firstentry ;
            
            if ( current )
            {
                WindowEventBase * retvalue = current->eventptr ;
                
                if ( remove )
                {
                    if ( current->lastentry )
                        current->lastentry->nextentry = current->nextentry ;
                    if ( current->nextentry )
                        current->nextentry->lastentry = current->lastentry ;
                    if ( queue->lastentry == current )
                        queue->lastentry = current->lastentry ;
                    if ( queue->firstentry == current )
                        queue->firstentry = current->nextentry ;
                    
                    queue->size = queue->size - 1 ;
                    delete current ;
                }
                
                unlock_boolean( queue->canuse ) ;
                return retvalue ;
            }
        }
        unlock_boolean( queue->canuse ) ;
    }
    
    return nullptr ;
}

void EventQueuePop ( WindowEventQueue* queue )
{
    if ( queue )
    {
        lock_boolean( queue->canuse ) ;
        {
            struct __winevent_queue_entry_t * current = queue->firstentry ;
            
            if ( current )
            {
                WindowEventBase * retvalue = current->eventptr ;
                
                if ( current->lastentry )
                    current->lastentry->nextentry = current->nextentry ;
                if ( current->nextentry )
                    current->nextentry->lastentry = current->lastentry ;
                if ( queue->lastentry == current )
                    queue->lastentry = current->lastentry ;
                if ( queue->firstentry == current )
                    queue->firstentry = current->nextentry ;
                
                queue->size = queue->size - 1 ;
                delete current ;
                
                if ( retvalue )
                    free ( retvalue ) ;
            }
        }
        unlock_boolean( queue->canuse ) ;
    }
}

void EventQueueClear ( WindowEventQueue* queue )
{
    if ( queue )
    {
        while ( queue->size > 0 )
            EventQueuePop( queue ) ;
    }
}







