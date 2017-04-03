//////////////////////////////////////////////////////////////////////
//
//  Property.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 21/03/2017.
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

#ifndef Property_h
#define Property_h

#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines an interface for a property. A property can be listened
/// as whether it is dirty or not. Also its access is thread-safe.
//////////////////////////////////////////////////////////////////////
template < typename Class >
class Property : public Resource
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Property () : iValue() , iDirty(true) { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Property ( const Property & rhs ) : iValue ( rhs ) , iDirty(true) { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Property ( const Class & rhs ) : iValue ( rhs ) , iDirty(true) { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~Property () noexcept ( false ) { }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the value and set the dirty flag to true.
    //////////////////////////////////////////////////////////////////////
    void set ( const Class & rhs )
    {
        GreAutolock ; iValue = rhs ;
        iDirty = true ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the value.
    //////////////////////////////////////////////////////////////////////
    const Class & get () const
    {
        GreAutolock ; return iValue ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clean the dirty flag (set to false).
    //////////////////////////////////////////////////////////////////////
    void clean () const
    {
        GreAutolock ; iDirty = false ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the dirty flag.
    //////////////////////////////////////////////////////////////////////
    bool isDirty () const
    {
        GreAutolock ; return iDirty ;
    }
    
private:
    
    /// @brief Value this property holds.
    Class iValue ;
    
    /// @brief True if the value has changed since the last 'clean' call.
    mutable bool iDirty ;
};

/// @brief Should create a property of given type and name. Setter/Getter are created on the fly ,
/// and Setter / Getter for property also. The mode is public , protected or private.

#define GRE_DECLARE_PROPERTY(name, class, mode) \
    public : \
        void set ## name ( const class & prop ) ; \
        const class & get ## name () const ; \
        const Property < class > & get ## name ## Property () const ; \
    mode : \
        Property < class > i ## name ;

#define GRE_IMPL_PROPERTY( name , class , superclass ) \
    void superclass :: set ## name ( const class & prop ) { GreAutolock ; i ## name .set( prop ) ; } \
    const class & superclass :: get ## name () const { GreAutolock ; return i ## name .get() ; } \
    const Property < class > & superclass :: get ## name ## Property () const { GreAutolock ; return i ## name ; }

#define GRE_IMPL_PROPERTY_NOSETTER( name , class , superclass ) \
    const class & superclass :: get ## name () const { GreAutolock ; return i ## name .get() ; } \
    const Property < class > & superclass :: get ## name ## Property () const { GreAutolock ; return i ## name ; }

GreEndNamespace

#endif /* Property_h */
