//////////////////////////////////////////////////////////////////////
//
//  DefinitionFileNode.h
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

#ifndef GRE_DEFINITIONFILENODE_H
#define GRE_DEFINITIONFILENODE_H

#include "Lockable.h"

GreBeginNamespace

namespace internal
{
    //////////////////////////////////////////////////////////////////////
    /// @brief Represents a Definition element.
    typedef struct
    {
        // Brackets position in the relative source. ( '[' and ']' )
        int brackbeg ; int brackend ;

        // Words defining the definition.
        std::vector < std::string > words ;

        // True if the definition holds a block.
        bool hasblock ;

    } Definition ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Holds one or more definitions related to the parent's definition.
    typedef struct
    {
        // Brackets position in the relative source. ( '{' and '}' )
        int blkbeg ; int blkend ;

    } DefinitionBlock ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Represents a node in the Definition's file hierarchy.
///
/// In a definition's file , a node represents a Definition and a Block.
/// A block then contains one or more Definition's file nodes.
///
//////////////////////////////////////////////////////////////////////
class DefinitionFileNode : public Lockable
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefinitionFileNode () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~DefinitionFileNode () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setDefinition ( const internal::Definition & definition );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const internal::Definition getDefinition () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setDefinitionBlock ( const internal::DefinitionBlock & block );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const internal::DefinitionBlock getDefinitionBlock () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addChild ( DefinitionFileNode* node );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setFilepath ( const std::string & filepath );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const std::vector < DefinitionFileNode* > getChildren () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if definition as at least one word.
    //////////////////////////////////////////////////////////////////////
    bool hasName () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns first definition word.
    //////////////////////////////////////////////////////////////////////
    const std::string getName () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the i's definition word if exists , or empty.
    //////////////////////////////////////////////////////////////////////
    const std::string getDefinitionWord ( uint32_t i ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of word in the definition.
    //////////////////////////////////////////////////////////////////////
    const uint32_t countWords() const ;

protected:

    /// @brief Definition structure.
    internal::Definition iDefinition ;

    /// @brief Block structure.
    internal::DefinitionBlock iBlock ;

    /// @brief Children if this node has a block.
    std::vector < DefinitionFileNode* > iChildren ;

    /// @brief Filepath where this node has been discovered.
    std::string iFilepath ;
};

GreEndNamespace

#endif // GRE_DEFINITIONFILENODE_H
