//////////////////////////////////////////////////////////////////////
//
//  ProjectionWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/07/2017.
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

#include "ProjectionWorker.h"
#include <Projection.h>

// -----------------------------------------------------------------------------
// ProjectionWorker implementation.

ProjectionWorker::ProjectionWorker ( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "37dc9a59-6540-4bf4-90b8-dd736b37b900" );
}

ProjectionWorker::~ProjectionWorker() noexcept( false )
{

}

bool ProjectionWorker::handles( const std::string & definition ) const
{
    return ( definition == "Projection" ) || ( definition == "GRE:Projection" ) ;
}

bool ProjectionWorker::isOverwritten( const Uuid & uuid ) const
{
    return false ;
}

bool ProjectionWorker::process( const DefinitionFileNode * node ,
                      DefinitionContext * ctxt ,
                      const DefinitionWorkerHandlingMap & defs ,
                      const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Wait for dependents workers here.

    // waitDependentDefinitions( parser , defs );

    //////////////////////////////////////////////////////////////////////
    // Computes name and type.

    std::string name = node -> getDefinitionWord( 1 );
    std::string type = node -> getDefinitionWord( 2 );

    if ( name.empty() )
    {
        ctxt -> pushError({
            123 , "Projection does not have any name." ,
            std::string() , 0 , false
        });

        return false ;
    }

    if ( type.empty() || (type != "Perspective" && type != "Ortho") )
    {
        ctxt -> pushError({
            123 , std::string("Projection '") + name + "' has invalid type : " + type ,
            std::string() , 0 , false
        });

        return false ;
    }

    //////////////////////////////////////////////////////////////////////
    // Computes projections args.

    if ( type == "Perspective" )
    {
        std::string sfov = node -> getDefinitionWord( 3 );
        std::string srat = node -> getDefinitionWord( 4 );
        std::string snea = node -> getDefinitionWord( 5 );
        std::string sfar = node -> getDefinitionWord( 6 );

        float fov = (float) strtod( sfov.c_str() , nullptr ) ;
        float rat = (float) strtod( srat.c_str() , nullptr ) ;
        float nea = (float) strtod( snea.c_str() , nullptr ) ;
        float far = (float) strtod( sfar.c_str() , nullptr ) ;

        if ( fov < FloatPrecision ) fov = 70.0f ;
        if ( rat < FloatPrecision ) rat = 16.0f / 9.0f ;
        if ( nea < FloatPrecision ) nea = 0.1f ;
        if ( far < FloatPrecision ) far = 100.0f ;

        ctxt -> setLocalVariableTpl( name , Projection::Perspective( fov , rat , nea , far ) );
        return true ;
    }

    else
    {
        std::string slef = node -> getDefinitionWord( 3 );
        std::string srig = node -> getDefinitionWord( 4 );
        std::string stop = node -> getDefinitionWord( 5 );
        std::string sbot = node -> getDefinitionWord( 6 );
        std::string snea = node -> getDefinitionWord( 7 );
        std::string sfar = node -> getDefinitionWord( 8 );

        float lef = (float) strtod( slef.c_str() , nullptr ) ;
        float rig = (float) strtod( srig.c_str() , nullptr ) ;
        float top = (float) strtod( stop.c_str() , nullptr ) ;
        float bot = (float) strtod( sbot.c_str() , nullptr ) ;
        float nea = (float) strtod( snea.c_str() , nullptr ) ;
        float far = (float) strtod( sfar.c_str() , nullptr ) ;

        if ( nea < FloatPrecision ) nea = 0.1f ;
        if ( far < FloatPrecision ) far = 100.0f ;

        ctxt -> setLocalVariableTpl( name , Projection::Ortho( lef , rig , top , bot , nea , far ) );
        return true ;
    }
}

const std::vector < std::string > ProjectionWorker::getDependentDefinitions() const
{
    return
    {

    };
}

const std::vector < std::string > ProjectionWorker::definitions() const
{
    return
    {
        "Projection" , "GRE:Projection"
    };
}
