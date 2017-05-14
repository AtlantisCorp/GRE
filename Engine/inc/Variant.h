//////////////////////////////////////////////////////////////////////
//
//  Variant.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 25/01/2016.
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

#ifndef GRE_Variant_h
#define GRE_Variant_h

#include "Pools.h"
#include "Color.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Exception thrown when Variant experience a bad cast.
class VariantBadCast : public GreException
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    VariantBadCast ( const std::string & to , const std::string & from ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const char* what () const throw () ;

    /// @brief Exception's message.
    std::string iException ;
};

namespace internal
{
    class VariantHelper_Base
    {
    public:

        virtual ~VariantHelper_Base () { }
        virtual void* allocate () const = 0 ;
        virtual void destroy ( void* ) const = 0 ;
        virtual void* copy ( const void* ptr ) const = 0 ;
        virtual size_t size () const = 0 ;
        virtual const std::type_info & type () const = 0 ;
        virtual VariantHelper_Base* clone () const = 0 ;
    };

    template < typename Class >
    class VariantHelper : public VariantHelper_Base
    {
    public:

        virtual ~VariantHelper () { }

        virtual void* allocate () const { return new Class () ; }
        virtual void destroy ( void* ptr ) const { delete (const Class*) ptr ; }
        virtual void* copy ( const void* ptr ) const { return new Class ( *(const Class*) ptr ) ; }
        virtual size_t size () const { return sizeof( Class ) ; }
        virtual const std::type_info & type () const { return typeid(Class) ; }
        virtual VariantHelper_Base* clone () const { return new VariantHelper<Class> () ; }
    };
}

//////////////////////////////////////////////////////////////////////
/// @brief Experimental Generic Variant.
///
/// Should be able to support any type, with destructor called when
/// deleting the object. A type is supported by this class only if it
/// has a default and a copy constructor.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Variant
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Variant () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Variant ( const Variant & rhs ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Converts a const char [] C style string to std::string.
    //////////////////////////////////////////////////////////////////////
    Variant ( const char* cstr ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Copies the given object.
    //////////////////////////////////////////////////////////////////////
    template < typename Class > Variant ( const Class & object )
    {
        iHelper = new internal::VariantHelper < Class > () ;

        if ( !iHelper )
        throw AllocationBadMemory ( "internal::VariantHelper" , sizeof(internal::VariantHelper<Class>) ) ;

        iObject = iHelper -> copy ( &object ) ;

        if ( !iObject )
        throw AllocationBadMemory ( typeid(Class).name() , sizeof(Class) ) ;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~Variant () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Swaps this variant with given one. Notes the object is not
    /// copied during the process.
    //////////////////////////////////////////////////////////////////////
    void swap ( Variant & rhs ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Variant & operator = ( const Variant & rhs ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Variant does not hold anything.
    //////////////////////////////////////////////////////////////////////
    bool isInvalid () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Variant has the same type as given.
    //////////////////////////////////////////////////////////////////////
    bool is ( const std::type_info & type ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief If type is the same as given , converts it to the original
    /// type. If not , throw a VariantBadCast exception.
    //////////////////////////////////////////////////////////////////////
    template < typename Class > Class & to ()
    {
        if ( !iHelper )
        throw VariantBadCast ( typeid(Class).name() , "null" ) ;

        if ( !is(typeid(Class)) )
        throw VariantBadCast ( typeid(Class).name() , iHelper -> type().name() ) ;

        return *(Class*) iObject ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief If type is the same as given , converts it to the original
    /// type. If not , throw a VariantBadCast exception.
    //////////////////////////////////////////////////////////////////////
    template < typename Class > const Class & to () const
    {
        if ( !iHelper )
        throw VariantBadCast ( typeid(Class).name() , "null" ) ;

        if ( !is(typeid(Class)) )
        throw VariantBadCast ( typeid(Class).name() , iHelper -> type().name() ) ;

        return *(Class*) iObject ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the object and reallocate a correct helper.
    //////////////////////////////////////////////////////////////////////
    template < typename Class > void set ( const Class & object )
    {
        if ( iHelper )
        clear () ;

        iHelper = new internal::VariantHelper < Class > () ;

        if ( !iHelper )
        throw AllocationBadMemory ( "internal::VariantHelper" , sizeof(internal::VariantHelper<Class>) ) ;

        iObject = iHelper -> copy ( &object ) ;

        if ( !iObject )
        throw AllocationBadMemory ( typeid(Class).name() , sizeof(Class) ) ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Destroy the object.
    //////////////////////////////////////////////////////////////////////
    void clear () ;

public:

    /// @brief Holds functions depending on the type of the allocated object. This
    /// object should be modified when the type of the holded object is changed.
    /// Notes when no object is set, this object is null.
    internal::VariantHelper_Base* iHelper ;

    /// @brief Pointer to the holded object.
    void* iObject ;
};

/// @brief Associates a Variant to a string for key , also named a dictionnary.
typedef std::map < std::string , Variant > Dictionnary ;

GreEndNamespace

#endif
