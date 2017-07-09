//////////////////////////////////////////////////////////////////////
//
//  DefinitionContextError.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 18/06/2017.
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

#ifndef GRE_DEFINITIONCONTEXTERROR_H
#define GRE_DEFINITIONCONTEXTERROR_H

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a structure for reporting error when working on a
/// DefinitionContext. DefinitionWorker should use this structure to
/// returns error in 'Working Stage'.
typedef struct
{
    /// @brief Error code associated with this error. When 0 , no error is
    /// designated. When not zero , the meaning of the error is dependent of
    /// the Worker that emitted it.
    int code ;

    /// @brief Message associated with the error. Notes this message may be empty.
    std::string message ;

    /// @brief File associated with the error , or empty if no file is associated.
    std::string filepath ;

    /// @brief Line of the Definition in the file associated with the error , or
    /// zero if no file is associated.
    int line ;

    /// @brief True if this error should stop the current processing.
    bool critical ;

} DefinitionContextError ;

/// @brief List of Errors.
typedef std::vector< DefinitionContextError > DefinitionContextErrors ;

/// @brief Lets default printing for every errors.
std::ostream & operator << ( std::ostream & stream , const DefinitionContextErrors & errors );

GreEndNamespace

#endif // GRE_DEFINITIONCONTEXTERROR_H
