//////////////////////////////////////////////////////////////////////
//
//  MouseEvents.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/03/2017.
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

#include "MouseEvents.h"
#include "Window.h"

GreBeginNamespace

// ---------------------------------------------------------------------------------------------------

MouseEvent::MouseEvent ( const Window* emitter , const EventType& type )
: Gre::Event(emitter, type) , iWindow(emitter)
{
    
}

MouseEvent::~MouseEvent() noexcept ( false )
{
    
}

const Window* MouseEvent::getWindowPointer() const
{
    return iWindow ;
}

// ---------------------------------------------------------------------------------------------------

LeftMousePressEvent::LeftMousePressEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::LeftMousePress )
{
    
}

Event* LeftMousePressEvent::clone() const
{
    return new LeftMousePressEvent ( getWindowPointer() ) ;
}

// ---------------------------------------------------------------------------------------------------

LeftMouseReleaseEvent::LeftMouseReleaseEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::LeftMouseRelease )
{
    
}

Event* LeftMouseReleaseEvent::clone() const
{
    return new LeftMouseReleaseEvent ( getWindowPointer() ) ;
}

// ---------------------------------------------------------------------------------------------------

RightMousePressEvent::RightMousePressEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::RightMousePress )
{
    
}

Event* RightMousePressEvent::clone() const
{
    return new RightMousePressEvent ( getWindowPointer() ) ;
}

// ---------------------------------------------------------------------------------------------------

RightMouseReleaseEvent::RightMouseReleaseEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::RightMouseRelease )
{
    
}

Event* RightMouseReleaseEvent::clone() const
{
    return new RightMouseReleaseEvent ( getWindowPointer() ) ;
}

// ---------------------------------------------------------------------------------------------------

MouseExitedWindowEvent::MouseExitedWindowEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::MouseExitedWindow )
{
    
}

Event* MouseExitedWindowEvent::clone() const
{
    return new MouseExitedWindowEvent ( getWindowPointer() ) ;
}

// ---------------------------------------------------------------------------------------------------

MouseEnteredWindowEvent::MouseEnteredWindowEvent ( const Window* emitter )
: Gre::MouseEvent ( emitter , EventType::MouseEnteredWindow )
{
    
}

Event* MouseEnteredWindowEvent::clone() const
{
    return new MouseEnteredWindowEvent ( getWindowPointer() ) ;
}

GreEndNamespace
