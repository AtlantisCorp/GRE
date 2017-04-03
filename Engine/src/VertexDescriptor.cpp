//////////////////////////////////////////////////////////////////////
//
//  VertexDescriptor.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 06/07/2016.
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

#include "VertexDescriptor.h"

GreBeginNamespace

// -----------------------------------------------------------------------------

VertexAttribAlias VertexAttribFromString ( const std::string & attrib )
{
    if ( attrib == "Position" ) return VertexAttribAlias::Position ;
    if ( attrib == "Normal" ) return VertexAttribAlias::Normal ;
    if ( attrib == "Texture" ) return VertexAttribAlias::Texture ;
    if ( attrib == "Tangents" ) return VertexAttribAlias::Tangents ;
    if ( attrib == "Binormals" ) return VertexAttribAlias::Binormals ;
    return VertexAttribAlias::Position ;
}

// -----------------------------------------------------------------------------

VertexDescriptor::VertexDescriptor ()
{
    iSize = 0 ;
}

VertexDescriptor::~VertexDescriptor ()
{

}

const std::vector < VertexAttribComponent > & VertexDescriptor::getComponents () const
{
    return iComponents ;
}

void VertexDescriptor::addComponent(const Gre::VertexAttribComponent &component)
{
    iComponents.push_back ( component ) ;
    iSize += component.size ;
}

void VertexDescriptor::addComponent(const Gre::VertexAttribAlias &alias, size_t elements, const Gre::VertexAttribType &type, bool normalize, size_t size)
{
    VertexAttribComponent comp ;
    comp.alias = alias ;
    comp.elements = elements ;
    comp.type = type ;
    comp.normalize = normalize ;
    comp.size = size ;
    addComponent(comp) ;
}

size_t VertexDescriptor::getStride(const Gre::VertexAttribComponent &component) const
{
    return iSize ;
}

size_t VertexDescriptor::getOffset(const Gre::VertexAttribComponent &component) const
{
    size_t ret = 0 ;

    for ( auto comp : iComponents )
    {
        if ( comp.alias == component.alias ) {
            return ret ;
        }
        else {
            ret += comp.size ;
        }
    }

    return 0 ;
}

void VertexDescriptor::clear()
{
    iComponents.clear() ;
    iSize = 0 ;
}

size_t VertexDescriptor::getSize() const
{
    return iSize ;
}

const VertexAttribComponent VertexDescriptor::findComponent(const Gre::VertexAttribAlias &alias) const
{
    for ( auto c : iComponents )
    {
        if ( c.alias == alias )
            return c ;
    }

    return VertexAttribComponent () ;
}

GreEndNamespace
