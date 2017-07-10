//////////////////////////////////////////////////////////////////////
//
//  RenderPipeline.cpp
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

#include "RenderPipeline.h"
#include "Renderer.h"
#include "ResourceManager.h"

GreBeginNamespace

// -----------------------------------------------------------------------------
// RenderPipeline implementation.

RenderPipeline::RenderPipeline ( const std::string & name ) : Gre::Resource ( name )
{

}

RenderPipeline::~RenderPipeline () noexcept ( false )
{

}

void RenderPipeline::setPass ( uint8_t idx , const RenderPassHolder & pass )
{
    GreAutolock ; iPasses[idx] = pass ;
}

void RenderPipeline::setPass ( uint8_t idx , const std::string & name , const std::string & techname )
{
    GreAutolock ;

    auto tm = ResourceManager::Get() -> getTechniqueManager() ;

    if ( tm.isInvalid() )
    {
        GreDebug( "[ERRO] No Technique Manager found to set pass : " ) << idx << " ( " << name << " )." << gendl ;
        return ;
    }

    auto tech = tm -> get( techname ) ;

    if ( tech.isInvalid() )
    GreDebug( "[ERRO] Technique '" ) << techname << "' not found." << gendl ;

    else
    setPass( idx , name , tech ) ;
}

void RenderPipeline::setPass ( uint8_t idx , const std::string & name , const TechniqueHolder & technique )
{
    GreAutolock ;

    if ( technique.isInvalid() )
    {
        GreDebug( "[ERRO] Given technique is invalid." ) << gendl ;
        return ;
    }

    RenderPassHolder & pass = iPasses[idx] ;

    if ( !pass.isInvalid() )
    GreDebug( "[WARN] Overwriting pass '" ) << pass->getName() << "'." << gendl ;

    pass = new RenderPass( name ) ;

    if ( pass.isInvalid() )
    {
        GreDebug( "[ERRO] Can't allocate RenderPass '" ) << name << "'." << gendl ;
        return ;
    }

    pass -> setTechnique( technique ) ;
    GreDebug( "[INFO] Set index " ) << idx << " with RenderPass '" << name << "'." << gendl ;
}

RenderPassHolder RenderPipeline::getPass ( uint8_t idx )
{
    GreAutolock ;

    auto passit = iPasses.find( idx ) ;

    if ( passit == iPasses.end() )
    return RenderPassHolder( nullptr ) ;

    return passit -> second ;
}

const RenderPassHolder RenderPipeline::getPass ( uint8_t idx ) const
{
    GreAutolock ;

    auto passit = iPasses.find( idx ) ;

    if ( passit == iPasses.end() )
    return RenderPassHolder( nullptr ) ;

    return passit -> second ;
}

void RenderPipeline::reset ( uint8_t idx )
{
    GreAutolock ;

    auto passit = iPasses.find( idx ) ;

    if ( passit != iPasses.end() )
    passit -> second.clear() ;
}

void RenderPipeline::clear ()
{
    GreAutolock ; iPasses.clear() ;
}

void RenderPipeline::render ( const Renderer* renderer ) const
{
    GreAutolock ;

    if ( !renderer )
    {
        GreDebug( "[ERRO] Invalid Renderer." ) << gendl ;
        return ;
    }

    for ( auto it : iPasses )
    {
        if ( !it.second.isInvalid() )
        it.second -> render( renderer ) ;
    }
}

void RenderPipeline::renderReversed ( const Renderer* renderer ) const
{
    GreAutolock ;

    if ( !renderer )
    {
        GreDebug( "[ERRO] Invalid Renderer." ) << gendl ;
        return ;
    }

    for ( std::map < uint8_t , RenderPassHolder >::const_reverse_iterator it = iPasses.rbegin() ; it != iPasses.rend() ; it++ )
    {
        if ( !it->second.isInvalid() )
        it->second -> render( renderer ) ;
    }
}

// -----------------------------------------------------------------------------
// RenderPipelineManager implementation.

RenderPipelineManager::RenderPipelineManager( const std::string & name )
: Gre::ResourceManagerBase < RenderPipeline > ( name )
{

}

RenderPipelineManager::~RenderPipelineManager() noexcept( false )
{

}

RenderPipelineHolder RenderPipelineManager::loadBlank( const std::string & pipelinename )
{
    GreAutolock ;

    for ( auto pipeline : iHolders )
    if ( pipeline->getName() == pipelinename )
    return nullptr ;

    RenderPipelineHolder pipeline = new RenderPipeline( pipelinename );

    if ( pipeline.isInvalid() )
    {
        GreDebug( "Can't create RenderPipeline '" ) << pipelinename << "'." << gendl ;
        return nullptr ;
    }

    iHolders.push_back( pipeline );
    return pipeline ;
}

GreEndNamespace
