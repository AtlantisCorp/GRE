//////////////////////////////////////////////////////////////////////
//
//  ReferenceCountedObject.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
    if ( iCounter )
    {
        if ( iCounter->getHolderCount() > 0 )
        {
            throw GreExceptionWithText ( "Destructing 'ReferenceCountedObject' but 'iCounter->getHolderCount() > 0'." );
        }
    }
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
    GreAutolock ;
    
    if ( object )
    {
        // See if ownship has been done.
        
        if ( object->getCounter() )
        {
            // Yes , so holds the counter.
            object->iCounter->hold();
            iObject = const_cast<ReferenceCountedObject*>(object);
        }
        
        else
        {
            // No , creates a new ReferenceCounter.
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
            iObject = const_cast<ReferenceCountedObject*>(object);
        }
    }
}

ReferenceCountedObjectHolder::ReferenceCountedObjectHolder ( const ReferenceCountedObjectHolder& holder )
: iObject ( nullptr )
{
    GreAutolock ;
    ReferenceCountedObject* object = holder.iObject ;
    
    if ( object )
    {
        // See if ownship has been done.
        
        if ( object->getCounter() )
        {
            // Yes , so holds the counter.
            object->iCounter->hold();
            iObject = object;
        }
        
        else
        {
            // No , creates a new ReferenceCounter.
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
            iObject = object;
        }
    }
}

ReferenceCountedObjectHolder & ReferenceCountedObjectHolder::operator=(const Gre::ReferenceCountedObjectHolder &holder)
{
    GreAutolock ;
    clear() ;
    
    ReferenceCountedObject* object = holder.iObject ;
    
    if ( object )
    {
        // See if ownship has been done.
        
        if ( object->getCounter() )
        {
            // Yes , so holds the counter.
            object->iCounter->hold();
            iObject = object;
        }
        
        else
        {
            // No , creates a new ReferenceCounter.
            object->iCounter = new ReferenceCounter () ;
            object->iCounter->hold () ;
            iObject = object;
        }
    }
    
    return *this ;
}

ReferenceCountedObjectHolder::~ReferenceCountedObjectHolder() noexcept ( false )
{
    clear();
}

ReferenceCountedObject * ReferenceCountedObjectHolder::getObject()
{
    GreAutolock ;
    return iObject ;
}

const ReferenceCountedObject * ReferenceCountedObjectHolder::getObject() const
{
    GreAutolock ;
    return iObject ;
}

bool ReferenceCountedObjectHolder::isInvalid() const
{
    GreAutolock ;
    return iObject == nullptr ;
}

void ReferenceCountedObjectHolder::clear()
{
    GreAutolock ;
    
    if ( iObject )
    {
        // See if iCounter is valid.
        
        if ( iObject->getCounter() )
        {
            // Yes. Unhold it and see.
            iObject->iCounter->unhold();
            
            if ( iObject->iCounter->getHolderCount() == 0 )
            {
                // If the iCounter has no user too , we have to destroy it.
                
                if ( iObject->iCounter->getUserCount() == 0 )
                {
                    delete iObject->iCounter ;
                    iObject->iCounter = nullptr ;
                }
                
                // If we do not have any holder left , just delete the object.
                // As the users always have a copy , if we are the last holder we
                // don't want to get a copy of the counter pointer.
                delete iObject ;
                iObject = nullptr ;
            }
            
            else
            {
                // Just do nothing here. Some holders are holding this object yet ,
                // so we don't want them to have invalid object.
                iObject = nullptr ;
            }
        }
        
        else
        {
            // iCounter is invalid. Destroy the object and pray anything bad occurs.
            delete iObject ;
            iObject = nullptr ;
        }
    }
}

// ---------------------------------------------------------------------------------------------------

ReferenceCountedObjectUser::ReferenceCountedObjectUser ()
: iObject ( nullptr )
, iCounter ( nullptr )
{
    
}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObject* object )
: iObject ( nullptr )
, iCounter ( nullptr )
{
    GreAutolock ;
    
    if ( object )
    {
        // See if it has a Counter. If it does not have any counter , the object is not initialized using the
        // Holder/User system. Function 'iCheckCounterValidity()' will set 'iObject' to null. A non-initialized
        // object can't be used by this class.
        
        if ( object->getCounter() )
        {
            iCounter = object->iCounter ;
            iCounter->use();
            
            iObject = const_cast<ReferenceCountedObject*>(object) ;
        }
        
        else
        {
            iCounter = nullptr ;
            iObject = const_cast<ReferenceCountedObject*>(object) ;
        }
    }
}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObjectHolder& holder )
: iObject ( nullptr )
, iCounter ( nullptr )
{
    GreAutolock ;
    
    ReferenceCountedObject* object = const_cast<ReferenceCountedObject*>( holder.getObject() ) ;
    
    if ( object )
    {
        // See if it has a Counter. If it does not have any counter , the object is not initialized using the
        // Holder/User system. Function 'iCheckCounterValidity()' will set 'iObject' to null. A non-initialized
        // object can't be used by this class.
        
        if ( object->getCounter() )
        {
            iCounter = object->iCounter ;
            iCounter->use();
            
            iObject = object ;
        }
        
        else
        {
            iCounter = nullptr ;
            iObject = object ;
        }
    }
}

ReferenceCountedObjectUser::ReferenceCountedObjectUser ( const ReferenceCountedObjectUser& user )
: iObject ( nullptr )
, iCounter ( nullptr )
{
    GreAutolock ;
    
    ReferenceCountedObject * object = user.iObject ;
    ReferenceCounter* counter = user.iCounter ;
    
    if ( counter )
    {
        if ( counter->getHolderCount() > 0 )
        {
            // object ptr is valid , so use it to initialize.
            iObject = object ;
            iCounter = counter ;
            iCounter->use();
        }
        
        else
        {
            // object ptr is not valid , so use the counter but initialize the object to null.
            iObject = nullptr ;
            iCounter = counter ;
            iCounter->use();
        }
    }
    
    else
    {
        // If iCounter is null , we must rely on the validity of iObject. This is not safe at all.
        // Note that when locking the object , if the object here is valid , the holder will create
        // a new Counter. If the object is invalid , this will lead to unexpected behaviour.
        
        iObject = object ;
        iCounter = nullptr ;
    }
}

ReferenceCountedObjectUser::~ReferenceCountedObjectUser() noexcept ( false )
{
    GreAutolock ;
    
    // Here , just unsuse and see if we mus delete the counter.
    
    if ( iCounter )
    {
        iCounter->unuse();
        
        if ( iCounter->getUserCount() == 0 )
        {
            delete iCounter ;
        }
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
    
    ReferenceCountedObjectHolder holder ( iObject ) ;
    
    // The holder should have created a counter , if it was not already the case.
    
    if ( !iCounter )
    {
        iCounter = holder.getObject()->getCounter();
    }
    
    return holder ;
}

const ReferenceCountedObjectHolder ReferenceCountedObjectUser::lock() const
{
    GreAutolock ;
    iCheckCounterValidity();
    
    // We create the holder depending on the object. The fallback here is , if the
    // iObject was initialized without initializing the counter , thus this object
    // is destroyed and we then try to lock the user , the holder will be initialized
    // with invalid memory pointer and will lead to unexpected behaviour.
    
    ReferenceCountedObjectHolder holder ( iObject ) ;
    
    // The holder should have created a counter , if it was not already the case.
    
    if ( !iCounter )
    {
        iCounter = holder.getObject()->getCounter();
    }
    
    return holder ;
}

bool ReferenceCountedObjectUser::isInvalid() const
{
    GreAutolock ;
    iCheckCounterValidity();
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
    GreAutolock ;
    clear() ;
    
    iCounter = rhs.iCounter ;
    iObject = rhs.iObject ;
    
    if ( iCounter )
        iCounter->use() ;
    
    return *this ;
}

void ReferenceCountedObjectUser::iCheckCounterValidity() const
{
    GreAutolock ;
    if ( iCounter )
    {
        if ( iCounter->getHolderCount() == 0 )
        {
            // Holder Count is 0. We must invalidate the iObject.
            iObject = nullptr ;
        }
    }
}

GreEndNamespace
