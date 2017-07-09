//////////////////////////////////////////////////////////////////////
//
//  DefinitionParser.h
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

#ifndef GRE_DEFINITIONPARSER_H
#define GRE_DEFINITIONPARSER_H

#include "Resource.h"
#include "ResourceBundle.h"
#include "DefinitionContextError.h"
#include "DefinitionContext.h"
#include "DefinitionWorker.h"

#define GRE_DEFINITION_PARSER_VERSION 1

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief DefinitionParser's states.
enum class DefinitionParserState
{
    /// @brief Parser has not yet started processing the files.
    Idling ,

    /// @brief Parser launched preprocessor on files.
    Preprocessing ,

    /// @brief Parser is parsing the files.
    Parsing ,

    /// @brief Parser is working on the files.
    Working ,

    /// @brief Parser has finished files' processing.
    Finished
};

//////////////////////////////////////////////////////////////////////
/// @brief Parses a Definition File to create a DefinitionContext.
///
/// The DefinitionParser is the object one should use to parse and process
/// Definition Files. The 'parse()' function is done in two stage , a
/// 'parsing' stage which phisically parse the files given to the function ,
/// creating a DefinitionContext tree. A second stage , 'Working Stage' ,
/// launch the Workers checked for this context in order for them to
/// work on the different Definitions found in the files.
///
/// If a DefinitionParser is always available to the user by the
/// ResourceManager ('getDefinitionParser()') , one can use a derived
/// object to parse the files the way he wants.
///
/// @notes During Working Stage , the DefinitionContext is not shared
/// between different Parsing Stages. Thus , multiple related files should
/// be processed together , or files should be handled in a good order
/// to prevent a situation where parsing separately two files with one
/// depending on resources of the other one but is parsed first.
///
/// @notes 'parse()' is a non-blocking function. This means that a user
/// may want to wait the parser has finished processing the files ('wait()') ,
/// or may want to do other actions while the parser is working ('working()').
/// The current parser's state is given by 'status()'. Notes also that
/// using 'reset()' will reset the parser's state to 'Idling'.
///
//////////////////////////////////////////////////////////////////////
class DefinitionParser : public Resource
{
public:

    /// @brief Working thread informations.
    struct WorkingThread
    {
        std::string worker ;
        std::future<bool> thread ;
    } ;

public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefinitionParser ( const std::string & name = "definitionparser.default" ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DefinitionParser () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parses every bundles found in the ResourceManager.
    //////////////////////////////////////////////////////////////////////
    virtual void parseBundles () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parses bundles given.
    //////////////////////////////////////////////////////////////////////
    virtual void parseBundlesList ( const ResourceBundleHolderList & bundles ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parses the given files.
    /// @notes Given filepathes should not be relatives to bundles , but from
    /// working directory.
    //////////////////////////////////////////////////////////////////////
    virtual void parseFiles ( const std::vector< std::string > & filepathes ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parses the given file.
    /// @notes Given filepath is treated as a list of one filepath.
    //////////////////////////////////////////////////////////////////////
    virtual void parseFile ( const std::string & filepath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Wait while the given state is not the current state , or
    /// the timeout is exceeded , and returns the status of the waiting
    /// operation ( std::future_status::ready if state is okay ,
    /// std::future_status::timeout if timed out ).
    //////////////////////////////////////////////////////////////////////
    template< class Rep , class Period >
    std::future_status wait ( const DefinitionParserState & state , const std::chrono::duration<Rep,Period>& timeout ) const
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now() ;
        std::chrono::steady_clock::duration stimeout = std::chrono::duration_cast<std::chrono::steady_clock::duration>(timeout) ;

        do
        {
            if ( status() == state )
            return std::future_status::ready ;

            std::chrono::steady_clock::duration elapsed = std::chrono::steady_clock::now() - start ;

            if ( elapsed > stimeout )
            return std::future_status::timeout ;

        } while(1) ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Wait while the given state is different from the current
    /// state. Notes as this is a blocking function , if the state is never
    /// reached , the function will block forever.
    //////////////////////////////////////////////////////////////////////
    void wait ( const DefinitionParserState & state ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the current state.
    //////////////////////////////////////////////////////////////////////
    DefinitionParserState status () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the current state is 'parsing'.
    //////////////////////////////////////////////////////////////////////
    bool parsing () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the current state is 'working'.
    //////////////////////////////////////////////////////////////////////
    bool working () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the current state is 'finished'.
    //////////////////////////////////////////////////////////////////////
    bool finished () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Resets the current state and stop every processing.
    //////////////////////////////////////////////////////////////////////
    void reset () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief After a parsing process is finished , returns the
    /// Errors that may have occured during this process.
    //////////////////////////////////////////////////////////////////////
    DefinitionContextErrors getLastResult () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the last result after parsing and working process.
    //////////////////////////////////////////////////////////////////////
    void setLastResult ( const DefinitionContextErrors & errors ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'DefinitionParserState::Working' if some threads
    /// executed by workers are joinable , or 'DefinitionParserState::Finished'
    /// if none is encountered.
    //////////////////////////////////////////////////////////////////////
    DefinitionParserState checkCurrentWorkerStatus ( const DefinitionWorkerHolder & worker ) const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Makes Parsing Stage for given filepath and add found definitions
    /// to given DefinitionContext.
    //////////////////////////////////////////////////////////////////////
    virtual void parsing ( DefinitionContext* ctxt , const std::string & filepath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the next '[' character from position , including this
    /// position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findNextDefinition ( const std::string & source , uint32_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parses the given block for definitions and blocks.
    //////////////////////////////////////////////////////////////////////
    virtual void parseBlockContent ( const std::string & source , DefinitionFileNode* node , DefinitionContext* ctxt ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the next ']' character from position , including this
    /// position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findDefinitionEnd ( const std::string & source , uint32_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Computes the words used in a definition.
    /// Those words are : letters ( ex : abcde ) , or strings ( ex : "abcd" ) .
    //////////////////////////////////////////////////////////////////////
    virtual std::vector < std::string > computeDefinitionWords ( const std::string & source ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds any character next position that is not a space.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findNextCharacter ( const std::string & source , uint32_t pos , char * nextc ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the block's end '}'. Also counts opened bracket since
    /// given position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findBlockEnd ( const std::string & source , uint32_t pos ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'Checking' Stage.
    ///
    /// Processes the given DefinitionWorkerHandlingMap selecting only the
    /// definitions present in the file. Also , definitions not present in
    /// the given map but present in the file are notified as non-critical
    /// errors in the context errors.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual DefinitionWorkerHandlingMap checking ( DefinitionContext* ctxt , const DefinitionWorkerHandlingMap & map );

    //////////////////////////////////////////////////////////////////////
    /// @brief Handles 'Working' Stage.
    ///
    /// Launches asynchronously threads for each Worker , giving them definitions
    /// they can handle , and wait for all of them to finish. Notes that this
    /// function should be launched from another thread in 'parsing()'. Workers
    /// launched may wait for other Workers to finish their process to begin
    /// theirs , because they may depend on their results.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void working ( DefinitionContext* ctxt , const DefinitionWorkerHandlingMap & map );

    //////////////////////////////////////////////////////////////////////
    /// @brief Handles iteration for a node in the 'Working' Stage.
    //////////////////////////////////////////////////////////////////////
    virtual void working_iterate ( DefinitionFileNode* node ,
                                   DefinitionContext* ctxt ,
                                   std::map < DefinitionWorkerHolder , std::vector<std::string> > defsbyworkers ,
                                   std::vector < WorkingThread > & threads ,
                                   const DefinitionWorkerHandlingMap & map );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the parser's current state.
    //////////////////////////////////////////////////////////////////////
    virtual void setCurrentState ( const DefinitionParserState & state );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief For each Worker , wait for parser to launch working threads ,
    /// wait dependencies to finish their jobs , and then create threads to
    /// process each definitions and waits for them.
    //////////////////////////////////////////////////////////////////////
    virtual void working_per_workers (DefinitionContext* ctxt ,
                                      std::map < DefinitionWorkerHolder , std::vector < DefinitionFileNode* > > nodesbyworkers ,
                                      std::vector < WorkingThread > & threads ,
                                      const DefinitionWorkerHandlingMap & map );

protected:

    /// @brief Current Parser's state.
    DefinitionParserState iCurrentState ;

    /// @brief Last parsing result errors.
    DefinitionContextErrors iFutureResult ;

    /// @brief Internal flag true if Workers currently working should stop their
    /// process and end as either the destructor or the reset() is called.
    bool iWorkersShouldStop ;

    /// @brief Parsing thread.
    std::thread iParsingThread ;

    /// @brief Holds threads created by the parsing stage. When a Working thread
    /// is spawned , it is pushed into the list. The thread status is usually given
    /// with the assumption of getting std::future status. Notes that if that future
    /// gets a false value , this means it had errors while processing the given
    /// definition.
    std::vector < WorkingThread > iCurrentWorkingThreads ;
    
    /// @brief Holds the resulting session data. Notes when resetting the parser , this
    /// data is not saved.
    struct
    {
        /// @brief Workers that already finished their threads are stored here.
        std::vector < std::string > finishedWorkers ;
        
    } iSessionData ;
};

GRE_MAKE_HOLDER( DefinitionParser );

GreEndNamespace

#endif // GRE_DEFINITIONPARSER_H
