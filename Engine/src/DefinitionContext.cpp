//////////////////////////////////////////////////////////////////////
//
//  DefinitionContext.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 20/06/2017.
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

#include "DefinitionContext.h"

GreBeginNamespace

DefinitionContext::DefinitionContext ()
{
    iRootNode = new DefinitionFileNode() ;
}

DefinitionContext::~DefinitionContext ()
{
    delete iRootNode ;
}

void DefinitionContext::pushError ( const DefinitionContextError & error )
{
    GreAutolock ; iErrors.push_back( error ) ;
}

DefinitionFileNode* DefinitionContext::getRootNode()
{
    GreAutolock ; return iRootNode ;
}

void DefinitionContext::setCurrentFilepath ( const std::string & filepath )
{
    GreAutolock ; iCurrentFilepath = filepath ;
}

const std::string DefinitionContext::getCurrentFilepath() const
{
    GreAutolock ; return iCurrentFilepath ;
}

bool DefinitionContext::hasCriticalError() const
{
    GreAutolock ;

    for ( auto error : iErrors )
    if ( error.critical ) return true ;

    return false ;
}

const DefinitionContextErrors DefinitionContext::getErrors() const
{
    GreAutolock ; return iErrors ;
}

const std::vector < std::string > DefinitionContext::getFirstLevelDefinitionNames () const
{
    GreAutolock ;

    std::vector < std::string > result ;

    for ( auto child : iRootNode->getChildren() )
    {
        if ( !child )
        continue ;

        if ( child->hasName() )
        result.push_back( child->getName() );
    }

    return result ;
}

GreEndNamespace
