//////////////////////////////////////////////////////////////////////
//
//  EventQueue.h
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

#ifndef EventQueue_h
#define EventQueue_h

#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Events Sending Command .
/// Use this structure to describe to which Listener the Event must be
/// sent.
//////////////////////////////////////////////////////////////////////
struct EventSendingCommand
{
    EventHolder iEvent ;
    std::map < std::string , Holder < Resource > > iListeners ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Threaded Event Dispatcher .
/// The EventDispatcher dispatches events through a paralell thread to
/// registered Listeners . The EventDispatcher is started using 'start()'
/// and can be stopped using 'terminate()' function .
/// The EventDispatcher can listen to Resource Objects to dispatch Events
/// when they are sent to it.
//////////////////////////////////////////////////////////////////////
class EventDispatcher : public Resource
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    EventDispatcher ( const std::string& name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~EventDispatcher () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Starts the Dispatching Loop .
    //////////////////////////////////////////////////////////////////////
    virtual void start () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Stops the Dispatching Loop .
    //////////////////////////////////////////////////////////////////////
    virtual void terminate () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Dispatching Loop is started.
    //////////////////////////////////////////////////////////////////////
    virtual bool isStarted () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Registers an Event to be dispatched .
    //////////////////////////////////////////////////////////////////////
    virtual void onEvent ( EventHolder& e );

    //////////////////////////////////////////////////////////////////////
    /// @brief Registers an Event to be dispatched .
    //////////////////////////////////////////////////////////////////////
    virtual void sendEvent ( EventHolder& e );

    //////////////////////////////////////////////////////////////////////
    /// @brief Registers an EventSendingCommand to the dispatcher.
    //////////////////////////////////////////////////////////////////////
    virtual void sendEventCommand ( const EventSendingCommand& cmd ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns always true .
    //////////////////////////////////////////////////////////////////////
    virtual bool shouldTransmitEvents() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Stops the loop and reset everything .
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Function called by the Dispatch Thread.
    //////////////////////////////////////////////////////////////////////
    static void iDispatchThreadFunction ( EventDispatcher* dispatcher ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Events in the Queue.
    //////////////////////////////////////////////////////////////////////
    virtual void iClearEvents () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Runs the dispatch loop.
    //////////////////////////////////////////////////////////////////////
    virtual void run () ;

protected:

    /// @brief A thread that holds the dispatch loop.
    std::thread iDispatchThread ;

    /// @brief A Queue that holds every Events. Those will be send to every Listeners
    /// registered in this EventDispatcher.
    std::queue < EventHolder > iEventQueue ;

    /// @brief A Queue that holds specialized Events Sending Command . The Events holded in those
    /// Commands will be sent to the Listener list specified.
    std::queue < EventSendingCommand > iEventCommandsQueue ;

    /// @brief True if 'iDispatchThread' is started.
    bool iStarted ;

    /// @brief Internal boolean with value 'false' if 'iDispatchThread' must stop.
    bool iShouldTerminate ;
};

/// @brief Holder for EventDispatcher .
typedef Holder < EventDispatcher > EventDispatcherHolder ;

/// @brief SpecializedResourceHolderList for EventDispatcher .
typedef SpecializedResourceHolderList < EventDispatcher > EventDispatcherHolderList ;

GreEndNamespace

#endif /* EventQueue_h */
