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

ReferenceCountedObjectHolder::~ReferenceCountedObjectHolder() noexcept ( false )
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

ReferenceCountedObjectUser::ReferenceCountedObjectUser ()
: iObject ( nullptr ) , iCounter ( nullptr )
{

}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObject* objectcst )
: iObject ( nullptr ) , iCounter ( nullptr )
{

    ReferenceCountedObject* object = const_cast<ReferenceCountedObject*>( objectcst ) ;

    if ( object )
    {
        object->threadLock() ;

        // See if it has a Counter. If the counter is invalid, we do not create one as a holder would
        // immediately destroy it when using the lock() function. Notes that if the counter has an invalid
        // holder count, we assume the memory pointed by the object is invalid, and so we do not initialize
        // the object.

        ReferenceCounter* counter = object->getCounter() ;

        if ( counter )
        {
            counter -> threadLock() ;
            
            if ( counter -> getHolderCount() > 0 )
            {
                iObject = object ;
                iCounter = counter ;
                iCounter -> use() ;
            }
            else
            {
                iObject = nullptr ;
                iCounter = nullptr ;
            }
            
            counter -> threadUnlock() ;
        }

        else
        {
            iObject = object ;
            iCounter = nullptr ;
        }

        object->threadUnlock() ;
    }

    else
    {
        iObject = nullptr ;
        iCounter = nullptr ;
    }
}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObjectHolder& holder )
: iObject ( nullptr ) , iCounter ( nullptr )
{
    ReferenceCountedObject* object = const_cast<ReferenceCountedObject*>( holder.getObject() ) ;

    if ( object )
    {
        object->threadLock() ;
        
        // See if it has a Counter. If the counter is invalid, we do not create one as a holder would
        // immediately destroy it when using the lock() function. Notes that if the counter has an invalid
        // holder count, we assume the memory pointed by the object is invalid, and so we do not initialize
        // the object.
        
        ReferenceCounter* counter = object->getCounter() ;
        
        if ( counter )
        {
            counter -> threadLock() ;
            
            if ( counter -> getHolderCount() > 0 )
            {
                iObject = object ;
                iCounter = counter ;
                iCounter -> use() ;
            }
            else
            {
                iObject = nullptr ;
                iCounter = nullptr ;
            }
            
            counter -> threadUnlock() ;
        }
        
        else
        {
            iObject = object ;
            iCounter = nullptr ;
        }
        
        object->threadUnlock() ;
    }
    
    else
    {
        iObject = nullptr ;
        iCounter = nullptr ;
    }
}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObjectUser & user )
: iObject ( nullptr ) , iCounter ( nullptr )
{
    ReferenceCountedObject * object = user.iObject ;
    ReferenceCounter* counter = user.iCounter ;

    if ( object )
    {
        object->threadLock() ;
        
        // See if it has a Counter. If the counter is invalid, we do not create one as a holder would
        // immediately destroy it when using the lock() function. Notes that if the counter has an invalid
        // holder count, we assume the memory pointed by the object is invalid, and so we do not initialize
        // the object.
        
        if ( counter )
        {
            counter -> threadLock() ;
            
            if ( counter -> getHolderCount() > 0 )
            {
                iObject = object ;
                iCounter = counter ;
                iCounter -> use() ;
            }
            else
            {
                iObject = nullptr ;
                iCounter = nullptr ;
            }
            
            counter -> threadUnlock() ;
        }
        
        else
        {
            iObject = object ;
            iCounter = nullptr ;
        }
        
        object->threadUnlock() ;
    }
    
    else
    {
        iObject = nullptr ;
        iCounter = nullptr ;
    }
}

ReferenceCountedObjectUser::~ReferenceCountedObjectUser() noexcept ( false )
{
    // Here , just unuse and see if we must delete the counter. Indeed, the counter may have been decremented
    // by the holder before us.

    if ( iCounter )
    {
        iCounter->unuse();

        if ( iCounter->getUserCount() == 0 )
        delete iCounter ;
    }
}

ReferenceCountedObjectHolder ReferenceCountedObjectUser::lock()
{
    GreAutolock ;
    
    iCheckCounterValidity();

    // We create the holder depending on the object. The fallback here is , if the
    // iObject was initialized without initializing the counter , thus this object
    // is destroyed and we then try to lock the user , the holder will be initialized
    // with invalid memory pointer and will lead to unexpected behaviour.

    return ReferenceCountedObjectHolder ( iObject ) ;
}

const ReferenceCountedObjectHolder ReferenceCountedObjectUser::lock() const
{
    GreAutolock ;
    
    iCheckCounterValidity();

    // We create the holder depending on the object. The fallback here is , if the
    // iObject was initialized without initializing the counter , thus this object
    // is destroyed and we then try to lock the user , the holder will be initialized
    // with invalid memory pointer and will lead to unexpected behaviour.

    return ReferenceCountedObjectHolder ( iObject ) ;
}

bool ReferenceCountedObjectUser::isInvalid() const
{
    GreAutolock ; iCheckCounterValidity();
    return iObject == nullptr ;
}

void ReferenceCountedObjectUser::clear()
{
    GreAutolock ;

    if ( iCounter )
    {
        iCounter->unuse() ;

        // If the Counter has no other users or holders , we can delete it
        // safely.

        if ( iCounter->getUserCount() == 0 )
        {
            delete iCounter ;
        }
    }

    // Resets the properties.

    iCounter = nullptr ;
    iObject = nullptr ;
}

bool ReferenceCountedObjectUser::operator== ( const ReferenceCountedObjectUser & rhs ) const
{
    GreAutolock ;
    return iCounter == rhs.iCounter && iObject == rhs.iObject ;
}

ReferenceCountedObjectUser & ReferenceCountedObjectUser::operator=(const Gre::ReferenceCountedObjectUser &rhs)
{
    GreAutolock ; clear() ;

    iCounter = rhs.iCounter ;
    iObject = rhs.iObject ;

    if ( iCounter )
        iCounter->use() ;

    return *this ;
}

bool ReferenceCountedObjectUser::operator<(const Gre::ReferenceCountedObjectUser &rhs) const
{
    GreAutolock ; return (uintptr_t) iCounter < (uintptr_t) rhs.iCounter ;
}

void ReferenceCountedObjectUser::iCheckCounterValidity() const
{
    GreAutolock ;
    
    //////////////////////////////////////////////////////////////////////
    // Checks that the object is still valid. This is a simple but essential
    // step : if the object has been deleted by a holder, the counter should
    // hold a holder-count value of 0 . In this case , we set the object as
    // a null-pointer.
    
    if ( iObject && iCounter )
    {
        if ( iCounter -> getHolderCount() == 0 )
        iObject = nullptr ;
    }
}

GreEndNamespace
