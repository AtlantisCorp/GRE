//////////////////////////////////////////////////////////////////////
//
//  PipelineWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 10/07/2017.
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

#include "PipelineWorker.h"
#include <RenderPipeline.h>
#include <ResourceManager.h>

// -----------------------------------------------------------------------------
// PipelineWorker implementation.

PipelineWorker::PipelineWorker( const std::string & name )
: Gre::DefinitionWorker( name )
{

}

PipelineWorker::~PipelineWorker() noexcept( false )
{

}

bool PipelineWorker::handles( const std::string & definition ) const
{
    return ( definition == "Pipeline" ) || ( definition == "GRE:Pipeline" );
}

bool PipelineWorker::isOverwritten( const Uuid & uuid ) const
{
    return false ;
}

bool PipelineWorker::process( const DefinitionFileNode * node ,
                      DefinitionContext * ctxt ,
                      const DefinitionWorkerHandlingMap & defs ,
                      const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    RenderPipelineManagerHolder pipelines = ResourceManager::Get() -> getPipelineManager() ;
    std::string name = node -> getDefinitionWord( 1 );

    if ( name.empty() )
    {
        ctxt -> pushError({
            123 , "Pipeline definition has no name." ,
            std::string() , 0 , false
        });

        return false ;
    }

    RenderPipelineHolder pipeline = pipelines -> loadBlank( name );

    if ( pipeline.isInvalid() )
    {
        ctxt -> pushError({
            123 , name + " couldn't be created." ,
            std::string() , 0 , false
        });

        return false ;
    }

    for ( auto child : node -> getChildren() )
    {
        if ( child -> getName() == "Technique" )
        {
            waitDefinitions({ "Technique" , "GRE:Technique" } , defs , parser );

            std::string techname = child -> getDefinitionWord( 1 );
            TechniqueHolder technique = ResourceManager::Get() -> getTechniqueManager() -> get( techname );

            if ( technique.isInvalid() )
            {
                ctxt -> pushError({
                    123 , techname + " could not be loaded for pipeline " + name ,
                    std::string() , 0 , false
                });
            }

            else
            {
                if ( child -> getDefinitionWord( 2 ) == "INDEX" )
                pipeline -> setPass( static_cast<uint8_t>(strtol(child->getDefinitionWord(3).c_str() , NULL , 10)) ,
                    std::string() , technique );

                // else
                // pipeline -> addPass( std::string() , technique );
            }
        }
    }

    return true ;
}

const std::vector < std::string > PipelineWorker::getDependentDefinitions() const
{
    return
    {

    };
}

const std::vector < std::string > PipelineWorker::definitions() const
{
    return
    {
        "Pipeline" , "GRE:Pipeline"
    };
}
