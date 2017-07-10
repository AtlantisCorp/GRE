//////////////////////////////////////////////////////////////////////
//
//  ProgramWorker.cpp
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

#include "ProgramWorker.h"
#include <HardwareShader.h>
#include <ResourceManager.h>

// -----------------------------------------------------------------------------
// ProgramWorker implementation.

ProgramWorker::ProgramWorker( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "97367c32-e700-45eb-9f35-92c0526c996a" );
}

ProgramWorker::~ProgramWorker() noexcept( false )
{
    
}

bool ProgramWorker::handles(const std::string &definition) const
{
    return ( definition == "Program" ) || ( definition == "GRE:Program" ) ;
}

bool ProgramWorker::isOverwritten(const Gre::Uuid &uuid) const
{
    return false ;
}

bool ProgramWorker::process(const Gre::DefinitionFileNode *node, Gre::DefinitionContext *ctxt, const DefinitionWorkerHandlingMap &defs, const Gre::DefinitionParser *parser) const
{
    if ( !node || !ctxt )
    return false ;
    
    //////////////////////////////////////////////////////////////////////
    // Finds name.
    
    const std::string progname = node -> getDefinitionWord( 1 );
    
    if ( progname.empty() )
    {
        ctxt -> pushError({
            123 , std::string( "Definition does not have any name" ) ,
            std::string() , 0 , false
        });
        
        return false ;
    }
    
    //////////////////////////////////////////////////////////////////////
    // Computes shader tables.
    
    ShaderPathTableList tables ;
    
    for ( auto child : node -> getChildren() )
    {
        if ( !child )
        continue ;
        
        if ( child->getChildren().empty() )
        continue ;
        
        std::map< std::string , std::string > pathes ;
        
        for ( auto pathnode : child -> getChildren() )
        {
            if ( !pathnode ) continue ;
            if ( pathnode->countWords() < 2 ) continue ;
            
            std::string comp = pathnode -> getDefinitionWord( 0 );
            std::string path = pathnode -> getDefinitionWord( 1 );
            pathes[comp] = path ;
        }
        
        if ( child -> getName() == "Vertex" ) tables.add( ShaderType::Vertex, pathes );
        if ( child -> getName() == "Fragment" ) tables.add( ShaderType::Fragment , pathes );
    }
    
    //////////////////////////////////////////////////////////////////////
    // Loads the tables with manager.
    
    auto pm = ResourceManager::Get() -> getHardwareProgramManager() ;
    
    HardwareShaderHolderList shaders = pm -> loadShaderTables( tables );
    
    if ( shaders.empty() )
    {
        ctxt -> pushError({
            123 , std::string( "Definition '" ) + progname + "' has invalid ShaderTables." ,
            std::string() , 0 , false
        });
        
        return false ;
    }
    
    HardwareProgramHolder program = pm -> loadProgram( progname );
    
    if ( program.isInvalid() )
    {
        ctxt -> pushError({
            123 , std::string( "Definition '" ) + progname + "' has invalid Program." ,
            std::string() , 0 , false
        });
        
        return false ;
    }
    
    program -> attachShaders( shaders );
    program -> finalize();
    
    GreDebug( "[INFO] Processed HardwareProgram : " ) << progname << gendl ;
    return true ;
}

const std::vector< std::string > ProgramWorker::getDependentDefinitions() const
{
    return { } ;
}

const std::vector< std::string > ProgramWorker::definitions() const
{
    return
    {
        "Program" , "GRE:Program"
    };
}

