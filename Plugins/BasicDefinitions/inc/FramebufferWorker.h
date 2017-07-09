//////////////////////////////////////////////////////////////////////
//
//  FramebufferWorker.h
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

#ifndef GRE_FRAMEBUFFERWORKER_H
#define GRE_FRAMEBUFFERWORKER_H

#include <DefinitionParser.h>
using namespace Gre ;

//////////////////////////////////////////////////////////////////////
/// @brief Workers used to process 'Framebuffer' definitions.
//////////////////////////////////////////////////////////////////////
class FramebufferWorker : public DefinitionWorker
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    FramebufferWorker ( const std::string & name );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~FramebufferWorker () noexcept( false );

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the given definition is 'Framebuffer'.
    //////////////////////////////////////////////////////////////////////
    virtual bool handles ( const std::string & definition ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns false. Notes that every 'basics' workers cannot
    /// overwrite other workers.
    //////////////////////////////////////////////////////////////////////
    virtual bool isOverwritten ( const Uuid & uuid ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Processes the given definition node.
    //////////////////////////////////////////////////////////////////////
    virtual bool process ( const DefinitionFileNode * node ,
                          DefinitionContext * ctxt ,
                          const DefinitionWorkerHandlingMap & defs ,
                          const DefinitionParser* parser ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of Definitions this Worker depends on.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector< std::string > getDependentDefinitions() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of Definitions this Worker assume to process.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector< std::string > definitions () const ;
};

#endif // GRE_FRAMEBUFFERWORKER_H
