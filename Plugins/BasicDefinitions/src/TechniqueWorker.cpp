//////////////////////////////////////////////////////////////////////
//
//  TechniqueWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 28/06/2017.
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

#include "TechniqueWorker.h"
#include <Technique.h>
#include <ResourceManager.h>

// -----------------------------------------------------------------------------
// TechniqueWorker implementation.

TechniqueWorker::TechniqueWorker ( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "40b1483f-6986-4e17-b098-56da0e52cd33" );
}

TechniqueWorker::~TechniqueWorker () noexcept( false )
{

}

bool TechniqueWorker::handles ( const std::string & definition ) const
{
    return ( definition == "Technique" ) || ( definition == "GRE:Technique" ) ;
}

bool TechniqueWorker::isOverwritten ( const Uuid & uuid ) const
{
    return false ;
}

bool TechniqueWorker::process ( const DefinitionFileNode * node ,
                       DefinitionContext * ctxt ,
                       const DefinitionWorkerHandlingMap & defs ,
                       const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Technique's worker needs to wait for other workers to process.

    // waitDependentDefinitions( parser , defs );

    //////////////////////////////////////////////////////////////////////
    // Works on given technique node.

    std::string nodename = node -> getName() ;

    if ( !handles( nodename ) )
    {
        GreDebug( "[WARN] DefinitionFileNode '" ) << nodename << "' not handled by this worker." << gendl ;
        return false ;
    }

    std::string techname = node -> getDefinitionWord( 1 );

    if ( techname.empty() )
    {
        GreDebug( "[WARN] DefinitionFileNode '" ) << nodename << "' does not have a valid Technique's name." << gendl ;
        return false ;
    }

    auto pm = ResourceManager::Get() -> getHardwareProgramManager() ;
    auto tm = ResourceManager::Get() -> getTechniqueManager() ;
    auto fm = ResourceManager::Get() -> getFramebufferManager() ;

    TechniqueHolder technique = tm -> loadBlank( techname );

    for ( auto subnode : node -> getChildren() )
    {
        if ( !subnode )
        continue ;

        if ( subnode -> getDefinitionWord( 0 ) == "Attribute" &&
            subnode -> countWords() >= 3 )
        {
            std::string alias = subnode -> getDefinitionWord( 1 );
            std::string attname = subnode -> getDefinitionWord( 2 );
            technique -> setAttribName( VertexAttribFromString(alias) , attname );
        }

        else if ( subnode -> getDefinitionWord( 0 ) == "Alias" &&
            subnode -> countWords() >= 3 )
        {
            std::string alias = subnode -> getDefinitionWord( 1 );
            std::string alname = subnode -> getDefinitionWord( 2 );
            technique -> setAlias( TechniqueParamFromString(alias) , alname );
        }

        else if ( subnode -> getDefinitionWord( 0 ) == "LightingMode" &&
            subnode -> countWords() >= 2 )
        {
            std::string mode = subnode -> getDefinitionWord( 1 );
            technique -> setLightingMode( TechniqueLightingModeFromString(mode) );
        }

        else if ( subnode -> getDefinitionWord( 0 ) == "Program" &&
            subnode -> countWords() >= 2 )
        {
            std::string progname = subnode -> getDefinitionWord( 1 );
            auto program = pm -> getProgram( progname );

            if ( program.isInvalid() )
            GreDebug( "[WARN] HardwareProgram '" ) << progname << "' does not exists." << gendl ;

            else
            technique -> setHardwareProgram( program );
        }

        else if ( subnode -> getDefinitionWord( 0 ) == "Framebuffer" &&
            subnode -> countWords() >= 2 )
        {
            std::string fbname = subnode -> getDefinitionWord( 1 );
            auto fb = fm -> get( fbname );

            if ( fb.isInvalid() )
            GreDebug( "[WARN] Framebuffer '" ) << fbname << "' does not exists." << gendl ;

            else
            technique -> setFramebuffer( fb );
        }

        else if ( subnode -> getName() == "Self-Rendered" )
        {
            technique -> setSelfRendered( true );
        }

        else if ( subnode -> getName() == "GlobSet" &&
            subnode -> countWords() >= 3 )
        {
            std::string globname = subnode -> getDefinitionWord( 1 );
            std::string paramname = subnode -> getDefinitionWord( 2 );
            technique -> addGlobSet( globname , TechniqueParamFromString(paramname) );
        }

        else if ( subnode -> getName() == "GlobAlias" &&
            subnode -> countWords() >= 3 )
        {
            std::string globname = subnode -> getDefinitionWord( 1 );
            std::string paramname = subnode -> getDefinitionWord( 2 );
            technique -> addGlobAlias( globname , paramname );
        }
    }

    tm -> loadFromHolder( technique );
    GreDebug( "[INFO] Loaded Technique '" ) << techname << "'." << gendl ;
    return true ;
}

const std::vector< std::string > TechniqueWorker::getDependentDefinitions() const
{
    return
    {
        "Framebuffer" , "GRE:Framebuffer" ,
        "Program" , "GRE:Program" ,
        "Global" , "GRE:Global"
    };
}

const std::vector< std::string > TechniqueWorker::definitions() const
{
    return
    {
        "Technique" , "GRE:Technique"
    };
}
