//////////////////////////////////////////////////////////////////////
//
//  Variant.cpp
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

#include "Variant.h"

GreBeginNamespace

// -----------------------------------------------------------------------------

VariantBadCast::VariantBadCast ( const std::string & to , const std::string & from )
: iException ( std::string("Bad Cast to '") + to + "' from '" + from + "'." )
{

}

const char* VariantBadCast::what () const throw ()
{
    return iException.c_str() ;
}

// -----------------------------------------------------------------------------

Variant::Variant ()
{
    iHelper = nullptr ;
    iObject = nullptr ;
}

Variant::Variant ( const Variant & rhs )
{
    iHelper = nullptr ;
    iObject = nullptr ;
    operator = ( rhs ) ;
}

Variant::Variant ( const char* cstr )
{
    iHelper = new internal::VariantHelper < std::string > () ;

    if ( !iHelper )
    throw AllocationBadMemory ( "internal::VariantHelper" , sizeof(internal::VariantHelper<std::string>) ) ;

    std::string tmp (cstr) ;
    iObject = iHelper -> copy ( &tmp ) ;

    if ( !iObject )
    throw AllocationBadMemory ( typeid(std::string).name() , sizeof(std::string) ) ;
}

Variant::~Variant ()
{
    clear () ;
}

void Variant::swap ( Variant & rhs )
{
    std::swap ( iHelper , rhs.iHelper ) ;
    std::swap ( iObject , rhs.iObject ) ;
}

Variant & Variant::operator = ( const Variant & rhs )
{
    if ( iHelper )
    clear () ;

    if ( !rhs.iHelper )
    return *this ;

    iHelper = rhs.iHelper->clone() ;

    if ( !iHelper )
    throw AllocationBadMemory ( "internal::VariantHelper" , sizeof(internal::VariantHelper_Base) ) ;

    iObject = iHelper -> copy ( rhs.iObject ) ;

    if ( !iObject )
    throw AllocationBadMemory ( iHelper -> type().name() , iHelper -> size() ) ;

    return *this ;
}

bool Variant::isInvalid () const
{
    return iHelper == nullptr ;
}

bool Variant::is ( const std::type_info & type ) const
{
    if ( !iHelper )
    return false ;

    return iHelper -> type().hash_code() == type.hash_code() ;
}

void Variant::clear ()
{
    if ( iHelper )
    {
        if ( !iObject )
        throw GreExceptionWithText ( "Bad deallocation memory in function 'Variant::clear()'." ) ;

        iHelper -> destroy ( iObject ) ;
        delete iHelper ;

        iObject = nullptr ;
        iHelper = nullptr ;
    }
}

GreEndNamespace
