//////////////////////////////////////////////////////////////////////
//
//  Version.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 07/02/2017.
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

#ifndef Exceptions_h
#define Exceptions_h

#ifdef GreHasExceptions

#include <stdexcept>
#include <exception>

// As this file is included into Version.h , we don't want to include anything else but exceptions.
// Those exceptions are by default, nested (C++11 feature). Also, the Exception subsystem is not part
// of the Gre namespace.

class GreException : public std::exception
{
public:
    
    GreException ( const std::string & desc ) ;
    virtual ~GreException () throw() ;
    
    virtual const char* what () const throw() ;
    
protected:
    
    std::string iDesc ;
};

// Using this macro will lead to automatic launching only if 'GreHasException' is set.
#define GreThrow(msg) std::throw_with_nested( GreException( msg ) ) 

#else 

#define GreThrow(msg) 

#endif /* GreHasExceptions */
#endif /* Exceptions_h */
