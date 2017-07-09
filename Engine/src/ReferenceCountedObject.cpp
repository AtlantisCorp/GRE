//////////////////////////////////////////////////////////////////////
//
//  ReferenceCountedObject.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 27/10/2016.
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

#include "ReferenceCountedObject.h"

GreBeginNamespace

ReferenceCountedObject::ReferenceCountedObject ()
: iCounter(nullptr)
{

}

ReferenceCountedObject::~ReferenceCountedObject() noexcept ( false )
{
    /*
    if ( iCounter )
    {
        if ( iCounter->getHolderCount() > 0 )
        {
            throw GreExceptionWithText ( "Destructing 'ReferenceCountedObject' but 'iCounter->getHolderCount() > 0'." );
        }
    }
     */
}

ReferenceCounter* ReferenceCountedObject::getCounter() const
{
    GreAutolock ;
    return iCounter ;
}

// ---------------------------------------------------------------------------------------------------

ReferenceCountedObjectHolder::ReferenceCountedObjectHolder ()
: iObject ( nullptr )
{

}

ReferenceCountedObjectHolder::ReferenceCountedObjectHolder ( const ReferenceCountedObject* object )
: iObject ( nullptr )
{
    //////////////////////////////////////////////////////////////////////
    // If we have a valid object , we first check the counter. If this one
    // is valid , we use it to hold a reference to this object. If not , we
    // create a new one for the object.

    if ( object )
    {
        object->threadLock() ;

        if ( object->getCounter() )
        object->iCounter->hold();

        else
        {
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
        }

        iObject = const_cast<ReferenceCountedObject*>(object);
        object->threadUnlock() ;
    }
}

ReferenceCountedObjectHolder::ReferenceCountedObjectHolder ( const ReferenceCountedObjectHolder& holder )
: iObject ( nullptr )
{
    ReferenceCountedObject* object = const_cast<ReferenceCountedObject*> ( holder.getObject() ) ;

    //////////////////////////////////////////////////////////////////////
    // If we have a valid object , we first check the counter. If this one
    // is valid , we use it to hold a reference to this object. If not , we
    // create a new one for the object.

    if ( object )
    {
        object->threadLock() ;

        if ( object->getCounter() )
            object->iCounter->hold();

        else
        {
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
        }

        iObject = const_cast<ReferenceCountedObject*>(object);
        object->threadUnlock() ;
    }
}

ReferenceCountedObjectHolder & ReferenceCountedObjectHolder::operator=(const Gre::ReferenceCountedObjectHolder &holder)
{
    GreAutolock ;
    clear() ;

    ReferenceCountedObject* object = const_cast<ReferenceCountedObject*> ( holder.getObject() ) ;

    //////////////////////////////////////////////////////////////////////
    // If we have a valid object , we first check the counter. If this one
    // is valid , we use it to hold a reference to this object. If not , we
    // create a new one for the object.

    if ( object )
    {
        object->threadLock() ;

        if ( object->getCounter() )
            object->iCounter->hold();

        else
        {
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
        }

        iObject = const_cast<ReferenceCountedObject*>(object);
        object->threadUnlock() ;
    }

    return *this ;
}

bool ReferenceCountedObjectHolder::operator == ( const ReferenceCountedObjectHolder & rhs ) const
{
    GreAutolock ; return iObject == rhs.iObject ;
}

ReferenceCountedObjectHolder::~ReferenceCountedObjectHolder() // noexcept ( false )
{
    clear();
}

ReferenceCountedObject * ReferenceCountedObjectHolder::getObject()
{
    GreAutolock ; return iObject ;
}

const ReferenceCountedObject * ReferenceCountedObjectHolder::getObject() const
{
    GreAutolock ; return iObject ;
}

bool ReferenceCountedObjectHolder::isInvalid() const
{
    GreAutolock ; return iObject == nullptr ;
}

void ReferenceCountedObjectHolder::clear()
{
    GreAutolock ;

    if ( iObject )
    {
        iObject->threadLock() ;

        // See if iCounter is valid.

        if ( iObject->getCounter() )
        {
            // Yes. Unhold it and see.
            iObject->iCounter->unhold();

            if ( iObject->iCounter->getHolderCount() == 0 )
            {
                // If the iCounter has no user too , we have to destroy it. But this has to be done after
                // deleting the object. If an object holder is destroyed, the object will be deleted. But if
                // this object holds a listener holder to itself, the holder will be destroyed, destroying
                // the listening user... the parent object. The holder will so try to lock the counter
                // mutex but unsucessfully, because the counter has already been destroyed.

                ReferenceCounter* tmp = nullptr ;

                if ( iObject->iCounter->getUserCount() == 0 )
                {
                    tmp = iObject -> iCounter ;
                    iObject->iCounter = nullptr ;
                }

                // If we do not have any holder left , just delete the object.
                // As the users always have a copy , if we are the last holder we
                // don't want to get a copy of the counter pointer.

                iObject->threadUnlock() ;

                delete iObject ;
                iObject = nullptr ;

                if ( tmp )
                delete tmp ;
            }

            else
            {
                // Just do nothing here. Some holders are holding this object yet ,
                // so we don't want them to have invalid object.
                iObject->threadUnlock() ;
                iObject = nullptr ;
            }
        }

        else
        {
            // iCounter is invalid. Destroy the object and pray anything bad occurs.
            iObject->threadUnlock() ;
            delete iObject ;
            iObject = nullptr ;
        }
    }
}

// ---------------------------------------------------------------------------------------------------

GreEndNamespace
