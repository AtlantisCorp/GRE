//////////////////////////////////////////////////////////////////////
//
//  DefinitionWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 23/06/2017.
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

#include "DefinitionWorker.h"
#include "DefinitionParser.h"

GreBeginNamespace

// -----------------------------------------------------------------------------
// DefinitionWorker implementation.

DefinitionWorker::DefinitionWorker( const std::string & name ) : Gre::Resource( name )
{

}

DefinitionWorker::~DefinitionWorker() noexcept( false )
{

}

bool DefinitionWorker::overwrites() const
{
    GreAutolock ; return iOverwrite ;
}

void DefinitionWorker::setOverwrites( bool value )
{
    GreAutolock ; iOverwrite = value ;
}

const Uuid DefinitionWorker::uuid() const
{
    GreAutolock ; return iUUID ;
}

void DefinitionWorker::wait( const DefinitionParser * parser , const DefinitionWorkerHolder & worker ) const
{
    if ( worker.isInvalid() || !parser )
    return ;

    GreDebug( "[INFO] " ) << getName() << " : Waiting for worker : " << worker->getName() << "." << gendl ;

    while( parser -> checkCurrentWorkerStatus(worker) != DefinitionWorkerState::Finished );

    // GreDebug( "[INFO] " ) << getName() << " : Waiting finished for worker : " << worker->getName() << "." << gendl ;
}

const std::vector< std::string > DefinitionWorker::getDependentDefinitions() const
{
    return std::vector< std::string > () ;
}

void DefinitionWorker::waitDependentDefinitions( const DefinitionParser* parser ,
                                                 const DefinitionWorkerHandlingMap & defs ) const
{
    if ( !parser ) return ;
    const std::vector< std::string > deps = getDependentDefinitions() ;

    // GreDebug( "[INFO]" ) << getName() << " : Waiting dependencies." << gendl ;

    for ( auto dep : deps )
    {
        // GreDebug( "[INFO] " ) << getName() << " : Waiting for definition '" << dep << "'." << gendl ;
        auto workerit = defs.find(dep);

        if ( workerit != defs.end() )
        wait( parser , workerit->second );

        // else
        // GreDebug( "[INFO] " ) << getName() << " : No definition '" <<  dep << "'." << gendl ;
    }

    // GreDebug( "[INFO]" ) << getName() << " : Dependencies waited." << gendl ;
}

void DefinitionWorker::waitDefinitions(const std::vector<std::string> &definitions, const DefinitionWorkerHandlingMap &defs, const Gre::DefinitionParser *parser) const
{
    if ( !parser )
    return ;
    
    for ( auto definition : definitions )
    {
        auto workerit = defs.find( definition );
        
        if ( workerit != defs.end() )
        wait( parser , workerit->second );
    }
}

// -----------------------------------------------------------------------------
// DefinitionWorkerManager implementation.

DefinitionWorkerManager::DefinitionWorkerManager( const std::string & name )
: Gre::ResourceManagerBase < DefinitionWorker > ( name )
{

}

DefinitionWorkerManager::~DefinitionWorkerManager() noexcept( false )
{

}

void DefinitionWorkerManager::add( const DefinitionWorkerHolder & worker )
{
    if ( worker.isInvalid() )
    return ;

    GreAutolock ;

    auto workerit = std::find( iHolders.begin() , iHolders.end() , worker );

    if ( workerit == iHolders.end() )
    {
        iHolders.add( worker );
        GreDebug( "[INFO] Added DefinitionWorker '" ) << worker->getName() << "'." << gendl ;
    }
}

DefinitionWorkerHandlingMap DefinitionWorkerManager::computeDefinitions()
{
    GreAutolock ;

    DefinitionWorkerHandlingMap result ;

    for ( auto worker : iHolders )
    {
        if ( worker.isInvalid() )
        continue ;

        auto defs = worker -> definitions() ;

        for ( const std::string & def : defs )
        {
            auto it = result.find(def);

            if ( it == result.end() )
            {
                result[def] = worker ;
            }

            else if ( !it->second.isInvalid() )
            {
                if ( worker->overwrites() && worker->isOverwritten(it->second->uuid()) )
                {
                    result[def] = worker ;
                }
            }
        }
    }

    return result ;
}

GreEndNamespace
