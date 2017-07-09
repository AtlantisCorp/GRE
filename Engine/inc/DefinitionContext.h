//////////////////////////////////////////////////////////////////////
//
//  DefinitionContext.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 20/06/2017.
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

#ifndef GRE_DEFINITIONCONTEXT_H
#define GRE_DEFINITIONCONTEXT_H

#include "DefinitionContextError.h"
#include "DefinitionFileNode.h"
#include "Variant.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Represents the base of a tree with every Definitions found
/// after a parsing stage from the DefinitionParser.
///
/// As this object should not be shared between non-relevant object ,
/// owning is made by the asynchronous thread launched by DefinitionParser
/// and is destroyed at its end.
///
/// DefinitionWorkers may access this context in locking and unloking
/// it to prevent multithread overwriting.
///
//////////////////////////////////////////////////////////////////////
class DefinitionContext : public Lockable
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefinitionContext () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~DefinitionContext () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Pushes a new Error.
    //////////////////////////////////////////////////////////////////////
    void pushError( const DefinitionContextError & error ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the root node of this context.
    //////////////////////////////////////////////////////////////////////
    DefinitionFileNode * getRootNode () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setCurrentFilepath ( const std::string & filepath );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const std::string getCurrentFilepath () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if at least one pushed error is critical.
    //////////////////////////////////////////////////////////////////////
    bool hasCriticalError () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const DefinitionContextErrors getErrors () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the name of definitions held by the direct children
    /// of root node.
    //////////////////////////////////////////////////////////////////////
    const std::vector < std::string > getFirstLevelDefinitionNames () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the local variable name is present , and type
    /// argument matches.
    //////////////////////////////////////////////////////////////////////
    template < typename Class >
    bool hasLocalVariableTpl ( const std::string & name ) const
    {
        GreAutolock ;

        if ( name.empty() )
        return false ;

        auto it = iLocalVariables.find( name );

        return it != iLocalVariables.end()
            && it->second.is( typeid(Class) ) ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a copy of the local variable with given name if it
    /// exists and if type match. If not , behaviour is undefined but
    /// default constructor should be used or exception may be thrown.
    //////////////////////////////////////////////////////////////////////
    template < typename Class >
    Class getLocalVariableTpl ( const std::string & name ) const
    {
        GreAutolock ;

        auto it = iLocalVariables.find( name );

        if ( it != iLocalVariables.end() )
        return it->second.to<Class>() ;

        else
        return Class () ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Copies a given object to the local variable table.
    /// Notes if the name given to register this variable is already taken
    /// by another variable, false is returned. True is returned in case of
    /// success.
    //////////////////////////////////////////////////////////////////////
    template < typename Class >
    bool setLocalVariableTpl ( const std::string & name , const Class & object )
    {
        GreAutolock ;

        if ( name.empty() )
        return false ;

        auto it = iLocalVariables.find( name );
        if ( it != iLocalVariables.end() )
        return false ;

        iLocalVariables[name] = object ;
        return true ;
    }

protected:

    /// @brief Errors emitted by the parser or any workers.
    DefinitionContextErrors iErrors ;

    /// @brief Root node holding the whole parsing context.
    DefinitionFileNode * iRootNode ;

    /// @brief A temporary path indicating , in parsing stage , at which file the
    /// parser is currently processing.
    std::string iCurrentFilepath ;

    /// @brief Holds local variables that workers may create when processing files ,
    /// as projections objects.
    Dictionnary iLocalVariables ;
};

GreEndNamespace

#endif // GRE_DEFINITIONCONTEXT_H
