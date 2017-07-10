//////////////////////////////////////////////////////////////////////
//
//  DefinitionParser.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 19/06/2017.
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

#include "DefinitionParser.h"
#include "TechniqueFilePreprocessor.h"
#include "ResourceManager.h"

GreBeginNamespace

DefinitionParser::DefinitionParser ( const std::string & name ) : Gre::Resource( name )
{
    iCurrentState = DefinitionParserState::Idling ;
    iWorkersShouldStop = true ;
}

DefinitionParser::~DefinitionParser () noexcept ( false )
{

}

void DefinitionParser::parseBundles ()
{
    GreAutolock ;

    auto bundles = ResourceManager::Get() -> getBundles() ;

    if ( bundles.empty() )
    {
        GreDebug( "[ERRO] No bundles." ) << gendl ;
        return ;
    }

    parseBundlesList( bundles ) ;
}

void DefinitionParser::parseBundlesList ( const ResourceBundleHolderList & bundles )
{
    GreAutolock ;

    if ( bundles.empty() )
    {
        GreDebug( "[ERRO] 'bundles' is empty." ) << gendl ;
        return ;
    }

    GreDebug( "[INFO] Parsing " ) << bundles.size() << " bundles." << gendl ;

    std::vector < std::string > filepathes ;

    for ( auto bundle : bundles )
    {
        if ( bundle.isInvalid() )
        continue ;

        const std::vector < std::string > & directories = bundle -> getDirectories( ResourceType::DefinitionFile ) ;

        if ( directories.empty() )
        continue ;

        for ( auto dir : directories )
        {
            auto pathes = Platform::GetFilesListWithDirectory( dir ) ;
            filepathes.insert( filepathes.begin() , pathes.begin() , pathes.end() ) ;
        }
    }

    if ( filepathes.empty() )
    GreDebug( "[ERRO] No files found in bundle's 'DefinitionFile' directories." ) << gendl ;

    else
    parseFiles( filepathes ) ;
}

void DefinitionParser::parseFiles ( const std::vector< std::string > & filepathes )
{
    GreAutolock ;

    if ( status() != DefinitionParserState::Idling && !finished() )
    return ;

    iWorkersShouldStop = false ;
    iCurrentState = DefinitionParserState::Idling ;

    GreDebug( "[INFO] Parsing " ) << filepathes.size() << " files." << gendl ;

    //////////////////////////////////////////////////////////////////////
    // Start a new async function , where we keep the same context while
    // parsing every files.

    iParsingThread = std::thread( [filepathes] (DefinitionParser* parser) {

                //////////////////////////////////////////////////////////////////////
                // Creates a new context.

                DefinitionContext* ctxt = new DefinitionContext () ;
                auto wm = ResourceManager::Get() -> getDefinitionWorkerManager() ;

                //////////////////////////////////////////////////////////////////////
                // Use the parser to parse the source file using our context.

                for ( auto path : filepathes )
                parser -> parsing ( ctxt , path ) ;

                //////////////////////////////////////////////////////////////////////
                // Now check errors. If our context has at least one critical error ,
                // we should not continue to the working stage.

                if ( ctxt -> hasCriticalError() )
                {
                    DefinitionContextErrors errors = ctxt -> getErrors() ;
                    delete ctxt ;

                    parser -> setLastResult( errors );
                    parser -> setCurrentState( DefinitionParserState::Finished );
                    return ;
                }

                //////////////////////////////////////////////////////////////////////
                // Computes definitions for the checking stage.

                DefinitionWorkerHandlingMap map = wm -> computeDefinitions() ;

                //////////////////////////////////////////////////////////////////////
                // Continue to the checking workers stage.

                DefinitionWorkerHandlingMap workers = parser -> checking ( ctxt , map ) ;

                if ( workers.empty() )
                {
                    DefinitionContextErrors errors = ctxt -> getErrors() ;
                    delete ctxt ;

                    errors.push_back({
                        -1 , "No worker found for definition file." ,
                        std::string() , 0 ,
                        true
                    });

                    parser -> setLastResult( errors );
                    parser -> setCurrentState( DefinitionParserState::Finished );
                    return ;
                }

                //////////////////////////////////////////////////////////////////////
                // Continue to the working stage. ( the parser will block in this thread
                // untill every worker thread is joined. ) When working stage is finished ,
                // the parser will set itself the state.

                parser -> working ( ctxt , workers ) ;

                //////////////////////////////////////////////////////////////////////
                // Hold back errors , delete context and return those errors. Hopping
                // none were emitted !

                DefinitionContextErrors errors = ctxt -> getErrors() ;
                delete ctxt ;

                parser -> setLastResult( errors );

    } , this );
}

void DefinitionParser::parseFile ( const std::string & filepath )
{
    parseFiles({ filepath }) ;
}

void DefinitionParser::wait ( const DefinitionParserState & state ) const
{
    do
    {
        if ( status() == state )
        return ;

    } while( 1 ) ;
}

DefinitionParserState DefinitionParser::status() const
{
    GreAutolock ; return iCurrentState ;
}

bool DefinitionParser::parsing() const
{
    return status() == DefinitionParserState::Parsing ;
}

bool DefinitionParser::working() const
{
    return status() == DefinitionParserState::Working ;
}

bool DefinitionParser::finished() const
{
    return status() == DefinitionParserState::Finished ;
}

void DefinitionParser::reset()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // We do not reset the thread , because destroying it would mean to
    // call 'terminate()'. This would issue memory leaks and other
    // unexpected behaviours. However , the 'iWorkersShouldStop' flag
    // should let them stop their loading.

    iCurrentState = DefinitionParserState::Idling ;
    iFutureResult = DefinitionContextErrors() ;
    iWorkersShouldStop = true ;
    iCurrentWorkingThreads.clear() ;
    iParsingThread.detach() ;
    iSessionData.launchedWorkers.clear() ;
    iSessionData.finishedWorkers.clear() ;
}

DefinitionContextErrors DefinitionParser::getLastResult() const
{
    GreAutolock ; return iFutureResult ;
}

DefinitionWorkerState DefinitionParser::checkCurrentWorkerStatus( const DefinitionWorkerHolder & worker ) const
{
    if ( worker.isInvalid() ) return DefinitionWorkerState::NotLaunched ;
    if ( worker->getName().empty() ) return DefinitionWorkerState::NotLaunched ;

    const std::string workername = worker->getName() ;

    GreAutolock ;
    
    //////////////////////////////////////////////////////////////////////
    // First look for worker in finished thread.
    
    auto workerit = std::find( iSessionData.finishedWorkers.begin() ,
                               iSessionData.finishedWorkers.end() ,
                               workername );
    
    if ( workerit != iSessionData.finishedWorkers.end() )
    return DefinitionWorkerState::Finished ;
    
    //////////////////////////////////////////////////////////////////////
    // Looks in launched workers.
    
    workerit = std::find( iSessionData.launchedWorkers.begin() ,
                          iSessionData.launchedWorkers.end() ,
                          workername );
    
    if ( workerit == iSessionData.launchedWorkers.end() )
    return DefinitionWorkerState::NotLaunched ;
    
    //////////////////////////////////////////////////////////////////////
    // Second look in working thread.

    for ( auto & wt : iCurrentWorkingThreads )
    {
        if ( wt.worker == workername )
        return DefinitionWorkerState::Working ;
    }
    
    //////////////////////////////////////////////////////////////////////
    // If worker is launched but not working , return launching.

    return DefinitionWorkerState::Launching ;
}

void DefinitionParser::parsing ( DefinitionContext* ctxt , const std::string & filepath )
{
    if ( !ctxt )
    return ;

    if ( filepath.empty() )
    {
        ctxt -> pushError({ -2 , "'filepath' is empty." , std::string() , 0 , false }) ;
        return ;
    }

    { GreAutolock ; iCurrentState = DefinitionParserState::Preprocessing ; }

    std::string src = Platform::GetFileSource( filepath ) ;

    TechniqueFilePreprocessor preprocessor ;
    preprocessor.process( filepath , src );
    src = preprocessor.getResult() ;

    if ( src.empty() )
    {
        ctxt -> pushError({ -2 , filepath + " has no source." , std::string() , 0 , false }) ;
        { GreAutolock ; iCurrentState = DefinitionParserState::Idling ; }
        return ;
    }

    { GreAutolock ; iCurrentState = DefinitionParserState::Parsing ; }

    ctxt -> setCurrentFilepath( filepath );
    uint32_t length = src.size() ;
    uint32_t pos = 0 ;

    pos = findNextDefinition( src , pos ) ;

    if( pos < length )
    parseBlockContent( src , ctxt -> getRootNode() , ctxt ) ;

    { GreAutolock ; iCurrentState = DefinitionParserState::Idling ; }
}

uint32_t DefinitionParser::findNextDefinition ( const std::string & source , uint32_t idx )
{
    uint32_t length = source.size() ;

    while ( idx < length )
    {
        if ( source.at(idx) == '[' )
        return idx ;

        idx ++ ;
    }

    return length ;
}

void DefinitionParser::parseBlockContent ( const std::string & source , DefinitionFileNode* node , DefinitionContext* ctxt )
{
    if ( !node || !ctxt )
    return ;

    //////////////////////////////////////////////////////////////////////
    // In order to parse the file , we loop through every characters untill
    // we find the '[' character , which is the definition beginner.

    uint32_t length = source.size() ;
    uint32_t pos = 0 ;

    pos = findNextDefinition ( source , pos ) ;

    while ( pos < length )
    {
        //////////////////////////////////////////////////////////////////////
        // Detected new definition . We should create a new node , and Registers
        // it to the root node.

        DefinitionFileNode* def = new DefinitionFileNode () ;
        def -> setFilepath( ctxt->getCurrentFilepath() );

        //////////////////////////////////////////////////////////////////////
        // Tries to see the definition end.

        uint32_t posend = findDefinitionEnd ( source , pos ) ;

        if ( pos >= length )
        {
            GreDebug ( "[WARN] Definition End ']' not seen." ) << gendl ;
            delete def ;

            ctxt -> pushError ({ -2 , "Definition has no end bracket ']'." ,
                                 ctxt->getCurrentFilepath() , 0 , true });

            return ;
        }

        //////////////////////////////////////////////////////////////////////
        // Tries to compute the words in the definition.

        std::string tmp = source.substr ( pos + 1 , posend - pos ) ;
        std::vector < std::string > words = computeDefinitionWords ( tmp ) ;

        //////////////////////////////////////////////////////////////////////
        // Registers everything in defs.

        internal::Definition definition ;
        definition.words = words ;
        definition.brackbeg = pos ;
        definition.brackend = posend ;

        def -> setDefinition( definition );

        //////////////////////////////////////////////////////////////////////
        // Now place our iterator to the end of the definition , ignores spaces
        // to see if next character is a block beginning.

        pos = posend ;

        char nextc ;
        uint32_t posblk = findNextCharacter ( source , pos , &nextc ) ;

        if ( nextc  == '{' )
        {
            //////////////////////////////////////////////////////////////////////
            // We have a block ! Tries to find the end of it.

            uint32_t posblkend = findBlockEnd ( source , posblk ) ;

            if ( posblkend >= length )
            {
                GreDebug ( "[WARN] Can't find Block end." ) << gendl ;
                delete def ;

                ctxt -> pushError ({ -2 , "Cant' find block end bracket '}'." ,
                                     ctxt->getCurrentFilepath() , 0 , true });

                return ;
            }

            //////////////////////////////////////////////////////////////////////
            // Now we have to register the new block and positions.

            internal::DefinitionBlock blk ;
            blk.blkbeg = posblk ;
            blk.blkend = posblkend ;
            definition.hasblock = true ;

            def -> setDefinition( definition );
            def -> setDefinitionBlock( blk );

            //////////////////////////////////////////////////////////////////////
            // Now we may reiterate the exact same thing on the block content.

            std::string blkcont = source.substr ( posblk + 1 , posblkend - posblk ) ;
            parseBlockContent ( blkcont , def , ctxt ) ;

            //////////////////////////////////////////////////////////////////////
            // Updates the position to the block's end.

            pos = posblkend ;
        }

        //////////////////////////////////////////////////////////////////////
        // Finds the next character to parse and registers the definition.

        node -> addChild( def );
        pos = findNextDefinition ( source , pos ) ;
    }
}

uint32_t DefinitionParser::findDefinitionEnd ( const std::string & source , uint32_t idx )
{
    uint32_t length = source.size() ;

    while ( idx < length )
    {
        if ( source.at(idx) == ']' )
        return idx ;

        idx ++ ;
    }

    return length ;
}

std::vector < std::string > DefinitionParser::computeDefinitionWords ( const std::string & source )
{
    uint32_t pos = 0 ;
    uint32_t length = source.size() ;
    std::vector < std::string > words ;

    //////////////////////////////////////////////////////////////////////
    // Iterates through the characters.

    while ( pos < length )
    {
        char c = source.at(pos) ;

        if ( std::isalpha(c) )
        {
            //////////////////////////////////////////////////////////////////////
            // We have a beginning word. Try to get it untill next space. A word is
            // an alphanumeric set of characters which are began by an alphabetic
            // character.

            std::string word ;

            while ( pos < length )
            {
                c = source.at(pos) ;

                if ( std::isspace(c) || c == ']' || c == '[' || c == '{' || c == '}' || std::iscntrl(c) )
                break ;

                word.push_back(c) ;
                pos ++ ;
            }

            words.push_back ( word ) ;
        }

        else if ( c == ']' )
        {
            //////////////////////////////////////////////////////////////////////
            // End of definition , exit the loop.
            break ;
        }

        else if ( c == '"' )
        {
            //////////////////////////////////////////////////////////////////////
            // String beginning. Try to get the next '"' and substr.

            uint32_t endpos = pos + 1 ;
            while ( endpos < length )
            {
                if ( source.at(endpos) == '"' )
                    break ;
                endpos ++ ;
            }

            if ( endpos < length )
            {
                std::string str = source.substr ( pos + 1 , endpos - pos - 1 ) ;
                words.push_back ( str ) ;
            }

            pos = endpos + 1 ;
        }

        else if ( std::isdigit(c) )
        {
            //////////////////////////////////////////////////////////////////////
            // Number beginning. A number may be of form '0' , '0.0' , '0.0f' , or
            // whatever may be recognized with strtol.

            std::string num ;

            while ( pos < length )
            {
                c = source.at(pos) ;

                if ( std::isspace(c) || c == ']' || c == '[' || c == '{' || c == '}' || std::iscntrl(c) )
                break ;

                num.push_back ( c ) ;
                pos ++ ;
            }

            words.push_back(num);
        }

        else
        {
            //////////////////////////////////////////////////////////////////////
            // Space or any other characters , we can ignore it.
            pos ++ ;
        }
    }

    return words ;
}

uint32_t DefinitionParser::findNextCharacter ( const std::string & source , uint32_t pos , char* nextc )
{
    //////////////////////////////////////////////////////////////////////
    // Here , we just iterate over the source to find the next non-space
    // character.

    uint32_t length = source.size() ;
    uint32_t npos = pos + 1 ;

    while ( npos < length )
    {
        if ( !std::isspace(source.at(npos)) )
        {
            if ( nextc ) *nextc = source.at(npos) ;
            return npos ;
        }

        npos ++ ;
    }

    return length ;
}

uint32_t DefinitionParser::findBlockEnd ( const std::string & source , uint32_t pos )
{
    uint32_t length = source.size() ;
    uint32_t npos = pos + 1 ;
    uint32_t counter = 1 ;

    //////////////////////////////////////////////////////////////////////
    // Here , we iterates over the source. When a '{' is found , we push
    // the bracket holder. When a '}' is found , we pop this holder. When
    // the holder is at 0 , it means it is the right bracket.

    while ( npos < length )
    {
        if ( source.at(npos) == '{' ) {
            counter ++ ;
        }

        else if ( source.at(npos) == '}' ) {
            counter -- ;
            if ( counter == 0 )
            return npos ;
        }

        npos ++ ;
    }

    return length ;
}

void DefinitionParser::setLastResult ( const DefinitionContextErrors & errors )
{
    GreAutolock ; iFutureResult = errors ;
}

DefinitionWorkerHandlingMap DefinitionParser::checking ( DefinitionContext* ctxt , const DefinitionWorkerHandlingMap & map )
{
    GreAutolock ;

    if ( !ctxt )
    return DefinitionWorkerHandlingMap () ;

    setCurrentState( DefinitionParserState::Parsing );

    //////////////////////////////////////////////////////////////////////
    // We must iterate through every definitions. One rule always applies :
    // A definition included in another definition should always be treated
    // by the Worker who process the parent's definition. This means that a
    // Definition of the first level is always considered as an object constructed
    // by an independant Worker and everything inside this definition is
    // managed by the Worker which process it.

    std::vector< std::string > defnames = ctxt -> getFirstLevelDefinitionNames () ;

    if ( defnames.empty() )
    {
        ctxt -> pushError({ -3 , "Definition Context does not hold any definitions." ,
            std::string() , 0 , true
        });

        setCurrentState( DefinitionParserState::Idling );
        return DefinitionWorkerHandlingMap () ;
    }

    DefinitionWorkerManagerHolder wm = ResourceManager::Get() -> getDefinitionWorkerManager() ;

    if ( wm.isInvalid() )
    {
        ctxt -> pushError({ -3 , "Definition Worker Manager is not present." ,
            std::string() , 0 , true
        });

        setCurrentState( DefinitionParserState::Idling );
        return DefinitionWorkerHandlingMap () ;
    }

    DefinitionWorkerHandlingMap result ;

    //////////////////////////////////////////////////////////////////////
    // Tries to find each definition in the given map.

    for ( const std::string & def : defnames )
    {
        auto it = map.find(def);

        if ( it == map.end() )
        {
            ctxt -> pushError({
                -3 , std::string("Definition '") + def + "' is not processed by installed Workers." ,
                std::string() , 0 , false
            });
            continue ;
        }

        else if ( it->second != result[def] )
        {
            result[def] = it->second ;
            GreDebug( "[INFO] Selecting Worker '" ) << it->second->getName() << "' to process Definition '" << def << "'." << gendl ;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Computes dependencies. If a Worker has its dependencies not fullfilled ,
    // just be strict and return an empty map. If we can find the depending
    // worker , just add it to the handling map. As adding a new entree in
    // the map invalids the iterator , rebuild dependencies.

dependency_stage:

    for ( auto it : result )
    {
        auto workerdeps = it.second -> getDependentDefinitions() ;

        for ( const std::string & dep : workerdeps )
        {
            if ( result.find(dep) == result.end() )
            {

                auto it2 = map.find(dep);
                if ( it2 != map.end() )
                {
                    result[dep] = it2->second ;
                    goto dependency_stage ;
                }

                else
                {
                    GreDebug( "[ERRO] Aborting checking stage because Worker '" ) << it.second->getName() << "' needs the following dependency : " << dep << gendl ;
                    setCurrentState( DefinitionParserState::Idling );
                    return DefinitionWorkerHandlingMap () ;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Mark the current status to idling.

    setCurrentState( DefinitionParserState::Idling );
    return result ;
}

void DefinitionParser::working( DefinitionContext* ctxt , const DefinitionWorkerHandlingMap & map )
{
    if ( !ctxt || map.empty() )
    return ;

    { GreAutolock ; iCurrentState = DefinitionParserState::Working ; }

    //////////////////////////////////////////////////////////////////////
    // Working stage : first we set the map to the context. Then , for each
    // entree , we compute a map with definitions by worker.

    std::map < DefinitionWorkerHolder , std::vector<std::string> > defsbyworkers ;

    for ( auto entree : map )
    defsbyworkers[entree.second].push_back( entree.first );
    
    //////////////////////////////////////////////////////////////////////
    // Make a map of every Definition Node for each Workers.
    
    std::map < DefinitionWorkerHolder , std::vector < DefinitionFileNode* > > nodesbyworkers ;
    
    for ( auto child : ctxt -> getRootNode() -> getChildren() )
    {
        auto worker = map.find( child->getName() )->second ;
        nodesbyworkers[worker].push_back(child);
        iSessionData.launchedWorkers.push_back( worker->getName() );
    }

    //////////////////////////////////////////////////////////////////////
    // Next , we begin to iterate through the nodes in the context. Each thread
    // created in the next function will be stored in the current working threads
    // held by the definition parser. As they execute simultaneously , the
    // status() function will return an Idling state if and only if any thread
    // is joinable.

    // DefinitionFileNode* rootnode = ctxt -> getRootNode() ;
    std::vector< WorkingThread > threads ;

    working_per_workers(ctxt, nodesbyworkers, threads, map);
    { GreAutolock ; iCurrentWorkingThreads = std::move(threads) ; }

    //////////////////////////////////////////////////////////////////////
    // Waits for every working thread to finish.

    while( 1 )
    {
        GreAutolock ;

        for ( auto it = iCurrentWorkingThreads.begin() ; it != iCurrentWorkingThreads.end() ; it++ )
        {
            if ( (*it).thread.wait_for(std::chrono::milliseconds(0) ) == std::future_status::ready )
            {
                GreDebug( "[INFO] Worker '" ) << (*it).worker << "' finished processing its definitions." << gendl ;

                iSessionData.finishedWorkers.push_back( (*it).worker );
                iCurrentWorkingThreads.erase(it);
                
                break ;
            }
        }

        if ( iCurrentWorkingThreads.empty() )
        {
            iCurrentState = DefinitionParserState::Finished ;
            break ;
        }
    }
}

void DefinitionParser::setCurrentState(const Gre::DefinitionParserState &state)
{
    GreAutolock ; iCurrentState = state ;
}

void DefinitionParser::working_per_workers(Gre::DefinitionContext *ctxt, std::map<DefinitionWorkerHolder, std::vector<DefinitionFileNode *> > nodesbyworkers, std::vector<WorkingThread> &threads, const DefinitionWorkerHandlingMap &map)
{
    if ( !ctxt )
    return ;
    
    for ( auto it : nodesbyworkers )
    {
        if ( it.first.isInvalid() )
        continue ;
        
        //////////////////////////////////////////////////////////////////////
        // Let's create a thread to do that.
        
        threads.push_back({
            it.first->getName() ,
            std::async( std::launch::async ,
                       
                [] (
                    DefinitionWorkerHolder worker ,
                    const std::vector < DefinitionFileNode* > & nodes ,
                    const DefinitionWorkerHandlingMap & map ,
                    const DefinitionParser* parser ,
                    DefinitionContext* ctxt
                       
                ) -> bool
                       {
                           
                           worker -> waitDependentDefinitions( parser , map );
                           
                           std::vector < std::future<bool> > workerthreads ;
                           
                           for ( auto node : nodes )
                           {
                               workerthreads.push_back( std::async( std::launch::async , [worker , node , ctxt , map , parser]() -> bool {
                                   
                                   return worker -> process( node , ctxt , map , parser );
                                   
                               }) );
                           }
                           
                           bool ret = true ;
                           
                           for ( auto & result : workerthreads )
                           if ( !result.get() ) ret = false ;
                           
                           return ret ;
                           
                           
                       } , it.first , it.second , map , this , ctxt )
        });
    }
}

GreEndNamespace
