//////////////////////////////////////////////////////////////////////
//
//  Event.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 23/11/2015.
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

#include "Event.h"
#include "Resource.h"
#include "ResourceUser.h"

GreBeginNamespace

Event::Event(const Resource* emitter , const EventType& etype)
: iEmitter ( nullptr )
, iType ( etype )
{
    iEmitter = new ResourceUser ( emitter ) ;
    
    if ( !iEmitter )
        throw GreExceptionWithText("'iEmitter' couldn't be allocated.") ;
}

Event::~Event() noexcept(false)
{
    if ( iEmitter )
        delete iEmitter ;
}

const ResourceUser& Event::getEmitter() const
{
    return * iEmitter ;
}

const EventType& Event::getType() const
{
    return iType;
}

// ---------------------------------------------------------------------------------------------------

KeyDownEvent::KeyDownEvent ( const Resource* emitter , Key key )
: Gre::Event( emitter , EventType::KeyDown )
, iKey(key)
{
    
}

// ---------------------------------------------------------------------------------------------------

KeyUpEvent::KeyUpEvent ( const Resource* emitter , Key key )
: Gre::Event( emitter , EventType::KeyUp )
, iKey(key)
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowMovedEvent::WindowMovedEvent ( const Resource* emitter , int left , int top )
: Gre::Event( emitter , EventType::WindowMoved )
, Left(left) , Top(top)
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowSizedEvent::WindowSizedEvent ( const Resource* emitter , int width , int height )
: Gre::Event( emitter , EventType::WindowSized )
, Width(width) , Height(height)
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowExposedEvent::WindowExposedEvent ( const Resource* emitter , const Surface& surface )
: Gre::Event( emitter , EventType::WindowExposed )
, iSurface(surface)
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowHiddenEvent::WindowHiddenEvent ( const Resource* emitter )
: Gre::Event( emitter , EventType::WindowExposed )
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowWillCloseEvent::WindowWillCloseEvent ( const Resource* emitter )
: Gre::Event( emitter , EventType::WindowWillClose )
{
    
}

// ---------------------------------------------------------------------------------------------------

WindowTitleChangedEvent::WindowTitleChangedEvent ( const Resource* emitter , const std::string& title )
: Gre::Event( emitter , EventType::WindowTitleChanged )
, iTitle(title)
{
    
}

// ---------------------------------------------------------------------------------------------------

UpdateEvent::UpdateEvent( const Resource* emitter , const UpdateClock& t )
: Event( emitter , EventType::Update )
, elapsedTime(t)
{
    
}

GreEndNamespace