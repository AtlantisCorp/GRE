//////////////////////////////////////////////////////////////////////
//
//  EventQueue.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 16/10/2016.
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

#include "EventDispatcher.h"

GreBeginNamespace

EventDispatcher::EventDispatcher(const std::string& name)
: Gre::Resource(name)
, iDispatchThread()
, iEventQueue()
, iStarted(false)
, iShouldTerminate(false)
{
    
}

EventDispatcher::~EventDispatcher() noexcept ( false )
{
    clear();
}

void EventDispatcher::start()
{
    if ( isStarted() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "EventDispatcher '" << getName() << "' is already started." << Gre::gendl;
#endif
        return ;
    }
    
    GreAutolock ;
    
    iDispatchThread = std::thread ( & EventDispatcher::iDispatchThreadFunction , this ) ;
    iStarted = true ;
    iShouldTerminate = false ;
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "EventDispatcher '" << getName() << "' started." << Gre::gendl;
#endif
}

void EventDispatcher::terminate()
{
    // In order to terminate the thread properly, we set the 'iShouldTerminate' flag to true and
    // wait for the dispatch thread to join.
    
    if ( iDispatchThread.joinable() )
    {
        {
            GreAutolock ;
            iShouldTerminate = true ;
        }
        
        iDispatchThread.join() ;
        iStarted = false ;
        iShouldTerminate = false ;
        iClearEvents() ;
    }
}

bool EventDispatcher::isStarted() const
{
    GreAutolock ;
    return iShouldTerminate ;
}

void EventDispatcher::onEvent(Gre::EventHolder &e)
{
    GreAutolock ;
    assert(!e.isInvalid() && "Argument 'e' is invalid.");
    
    Event* nextevent = e->clone() ;
    iEventQueue.push(nextevent) ;
}

void EventDispatcher::sendEvent(Gre::EventHolder &e)
{
    GreAutolock ;
    assert(!e.isInvalid() && "Argument 'e' is invalid.");
    
    Event* nextevent = e->clone () ;
    iEventQueue.push(nextevent) ;
}

void EventDispatcher::sendEventCommand(const Gre::EventSendingCommand &cmd)
{
    GreAutolock ;
    
    if ( !cmd.iEvent.isInvalid() && !cmd.iListeners.empty() )
    {
        EventSendingCommand nextcmd ;
        nextcmd.iEvent = EventHolder ( cmd.iEvent->clone() ) ;
        nextcmd.iListeners = cmd.iListeners ;
        iEventCommandsQueue.push(nextcmd) ;
    }
}

bool EventDispatcher::shouldTransmitEvents() const
{
    return true ;
}

void EventDispatcher::clear()
{
    GreAutolock ;
    if ( isStarted() ) terminate() ;
}

void EventDispatcher::iDispatchThreadFunction(Gre::EventDispatcher *dispatcher)
{
    if ( !dispatcher )
        return;
    
    dispatcher->run();
}

void EventDispatcher::iClearEvents()
{
    GreAutolock ;
    std::queue < EventHolder > () .swap(iEventQueue) ;
}

void EventDispatcher::run()
{
    bool shouldterminate = false ;
    
    while ( !shouldterminate )
    {
        GreAutolock ;
        
        if ( !iEventQueue.empty() )
        {
            EventHolder & nextevent = iEventQueue.front() ;
            
            if ( !nextevent.isInvalid() )
            {
                EventProceeder::sendEvent( nextevent ) ;
            }
            
            iEventQueue.pop() ;
        }
        
        if ( !iEventCommandsQueue.empty() )
        {
            EventSendingCommand & nextcmd = iEventCommandsQueue.front() ;
            EventHolder & nextevent = nextcmd.iEvent ;
            
            if ( !nextevent.isInvalid() )
            {
                for ( auto it : nextcmd.iListeners )
                {
                    if ( !it.second.isInvalid() )
                    {
                        it.second.lock() -> onEvent(nextevent) ;
                    }
                }
            }
            
            iEventCommandsQueue.pop() ;
        }
        
        shouldterminate = iShouldTerminate ;
    }
}

GreEndNamespace
