//////////////////////////////////////////////////////////////////////
//
//  DefinitionFileNode.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 21/06/2017.
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

#include "DefinitionFileNode.h"

GreBeginNamespace

DefinitionFileNode::DefinitionFileNode ()
{

}

DefinitionFileNode::~DefinitionFileNode ()
{
    for ( DefinitionFileNode* child : iChildren )
    if ( child ) delete child ;
}

void DefinitionFileNode::setDefinition ( const internal::Definition & definition )
{
    GreAutolock ; iDefinition = definition ;
}

const internal::Definition DefinitionFileNode::getDefinition () const
{
    GreAutolock ; return iDefinition ;
}

void DefinitionFileNode::setDefinitionBlock ( const internal::DefinitionBlock & block )
{
    GreAutolock ; iBlock = block ;
}

const internal::DefinitionBlock DefinitionFileNode::getDefinitionBlock () const
{
    GreAutolock ; return iBlock ;
}

void DefinitionFileNode::addChild ( DefinitionFileNode* node )
{
    GreAutolock ; iChildren.push_back( node );
}

void DefinitionFileNode::setFilepath ( const std::string & filepath )
{
    GreAutolock ; iFilepath = filepath ;
}

const std::vector < DefinitionFileNode* > DefinitionFileNode::getChildren () const
{
    GreAutolock ; return iChildren ;
}

bool DefinitionFileNode::hasName () const
{
    GreAutolock ; return iDefinition.words.size() >= 1 ;
}

const std::string DefinitionFileNode::getName () const
{
    GreAutolock ; return hasName() ? iDefinition.words.at(0) : std::string() ;
}

const std::string DefinitionFileNode::getDefinitionWord( uint32_t i ) const
{
    GreAutolock ; return iDefinition.words.size() >= (i+1) ? iDefinition.words.at(i) : std::string() ;
}

const uint32_t DefinitionFileNode::countWords() const
{
    GreAutolock ; return iDefinition.words.size() ;
}

GreEndNamespace
