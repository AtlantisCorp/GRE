//////////////////////////////////////////////////////////////////////
//
//  MatVarWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/07/2017.
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

#include "MatVarWorker.h"
#include <ResourceManager.h>

// -----------------------------------------------------------------------------
// MatVarWorker implementation.

MatVarWorker::MatVarWorker( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "56c8da45-1394-40ed-a1e6-e604c517fa1a" );
}

MatVarWorker::~MatVarWorker() noexcept( false )
{

}

bool MatVarWorker::handles( const std::string & definition ) const
{
    return ( definition == "Global" ) || ( definition == "GRE:Global" ) ;
}

bool MatVarWorker::isOverwritten( const Uuid & uuid ) const
{
    return false ;
}

bool MatVarWorker::process ( const DefinitionFileNode * node ,
                        DefinitionContext * ctxt ,
                        const DefinitionWorkerHandlingMap & defs ,
                        const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    auto tm = ResourceManager::Get() -> getTechniqueManager() ;

    if ( tm.isInvalid() )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Wait for dependents workers here.

    // waitDependentDefinitions( parser , defs );

    //////////////////////////////////////////////////////////////////////
    // Gets Variable type and name.

    std::string type = node -> getDefinitionWord( 1 );
    std::string name = node -> getDefinitionWord( 2 );

    if ( type.empty() || name.empty() )
    {
        ctxt -> pushError({
            123 , "Global variable has no type or no name." ,
            std::string() , 0 , false
        });

        return false ;
    }

    HdwProgVarType realtype = HdwProgVarTypeFromString( type );
    tm -> addGlobal( name , realtype , RealProgramVariable() );

    GreDebug( "[INFO] Processed Variable : " ) << name << gendl ;
    return true ;
}

const std::vector< std::string > MatVarWorker::getDependentDefinitions() const
{
    return { };
}

const std::vector< std::string > MatVarWorker::definitions() const
{
    return { "Global" , "GRE:Global" };
}
