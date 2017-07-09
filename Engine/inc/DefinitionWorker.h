//////////////////////////////////////////////////////////////////////
//
//  DefinitionWorker.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 22/06/2017.
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

#ifndef GRE_DEFINITIONWORKER_H
#define GRE_DEFINITIONWORKER_H

#include "Platform.h"
#include "DefinitionContext.h"
#include "Resource.h"
#include "SpecializedResourceManager.h"

GreBeginNamespace

class DefinitionWorker ;
class DefinitionParser ;

GRE_MAKE_HOLDER( DefinitionWorker );

/// @brief Handles each definition with its associated Worker.
typedef std::map < std::string , DefinitionWorkerHolder > DefinitionWorkerHandlingMap ;

//////////////////////////////////////////////////////////////////////
/// @brief Represents an abstract object that works on the DefinitionContext
/// node tree.
///
/// A definition worker will receive definitions from the context tree ,
/// according on which definition the worker wants to work on.
///
/// If two workers accept the same definition , the first reached in the
/// worker's list is used to work on that definition.
///
/// Worker has a name and a UUID defining it. A worker can wait another worker
/// to finish its job, specifying its UUID. A worker should retain a dependent worker
/// UUID before beginning its job, and check those workers presence. If the desired
/// worker are not present , the worker should stop its function and return an error.
///
/// Working stage is a multiprocess parsing of a Definition Context object , that lets
/// multiple objects create GRE objects. Though , when a worker works on a Definition ,
/// it should always remove it from the Definition Context. If the object could not
/// be created after working on the definition , it should be reinserted into the
/// Definition Context with an error code.
///
/// At the end of the Working stage , if the Definition Context is not empty , this
/// means that Definitions were not processed. Thus , if verbose ( or in debug mode ) ,
/// the Parser must list every Definitions that were not processed , with its error
/// code meaning either the Definition had no worker to process , either the Definition
/// could not be loaded for an internal Worker reason.
///
/// Any user may construct Workers. But they should not process Definitions that are
/// already taken by any other Worker. As an example , a dummy Worker cannot process
/// the 'Technique' definition because the 'Gre:TechniqueWorker' already process this
/// definition. In order to ensure that no worker process the definition , the Worker may
/// use DefinitionContext::findWorkerForDefinition() to make sure the desired definition
/// is not already processed , when the Parser registers Workers to process the Definition
/// Context. Note also that a user can use a list of Workers to process the Definition
/// Context. No checking is done using this alternative.
///
//////////////////////////////////////////////////////////////////////
class DefinitionWorker : public Resource
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefinitionWorker ( const std::string & name );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DefinitionWorker () noexcept ( false );

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the given definition is processed.
    //////////////////////////////////////////////////////////////////////
    virtual bool handles ( const std::string & definition ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Worker has overwriting mode enabled.
    /// Notes any user can enable/disable this mode , but default mode should
    /// not have always true or false value.
    //////////////////////////////////////////////////////////////////////
    virtual bool overwrites () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Forces overwriting mode.
    //////////////////////////////////////////////////////////////////////
    virtual void setOverwrites ( bool value );

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the given UUID can be overwritten.
    //////////////////////////////////////////////////////////////////////
    virtual bool isOverwritten ( const Uuid & uuid ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the worker's UUID.
    //////////////////////////////////////////////////////////////////////
    virtual const Uuid uuid () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Processes the given definition node. Should return true on
    /// success , false otherwise.
    //////////////////////////////////////////////////////////////////////
    virtual bool process ( const DefinitionFileNode * node ,
                           DefinitionContext * ctxt ,
                           const DefinitionWorkerHandlingMap & defs ,
                           const DefinitionParser* parser ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Waits while every threads launched with the given worker are
    /// joinable. If at least one of them is joinable , the function blocks.
    //////////////////////////////////////////////////////////////////////
    virtual void wait ( const DefinitionParser* parser , const DefinitionWorkerHolder & worker ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of Definitions this Worker depends on.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector< std::string > getDependentDefinitions() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of Definitions this Worker assume to process.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector< std::string > definitions () const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief For each definitions this Worker depends on , wait for the worker
    /// to finish its job.
    //////////////////////////////////////////////////////////////////////
    virtual void waitDependentDefinitions( const DefinitionParser* parser ,
                                           const DefinitionWorkerHandlingMap & defs ) const ;

protected:

    /// @brief Flag enabling overwriting.
    bool iOverwrite ;

    /// @brief UUID for this Worker. Notes that a creator should always set the same
    /// UUID for its Worker when creating it.
    Uuid iUUID ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages Workers added by other users through plugins.
///
/// Workers are added only if their UUID is not encountered in any other
/// worker already added to the manager.
///
//////////////////////////////////////////////////////////////////////
class DefinitionWorkerManager : public ResourceManagerBase < DefinitionWorker >
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefinitionWorkerManager ( const std::string & name );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DefinitionWorkerManager () noexcept ( false );

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Worker to the worker's list. Checks before that this
    /// worker is not already added.
    //////////////////////////////////////////////////////////////////////
    virtual void add ( const DefinitionWorkerHolder & worker );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Computes a list of every supported definitions with its
    /// associated worker.
    /// When two workers process the same definition , the rule of the
    /// overwriting permission is applied.
    //////////////////////////////////////////////////////////////////////
    virtual DefinitionWorkerHandlingMap computeDefinitions() ;
};

GRE_MAKE_HOLDER( DefinitionWorkerManager );

GreEndNamespace

#endif // GRE_DEFINITIONWORKER_H
