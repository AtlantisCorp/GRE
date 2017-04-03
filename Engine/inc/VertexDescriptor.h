//////////////////////////////////////////////////////////////////////
//
//  VertexDescriptor.h
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

#ifndef VertexDescriptor_h
#define VertexDescriptor_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Enumerates Alias to bind Vertex Attribute.
enum class VertexAttribAlias : int
{
    Position ,
    Color ,
    Normal ,
    Texture ,
    Tangents ,
    Binormals
};

//////////////////////////////////////////////////////////////////////
/// @brief Returns the VertexAttribAlias from its string.
VertexAttribAlias VertexAttribFromString ( const std::string & attrib ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Enumerates possible types for a Vertex Attribute.
enum class VertexAttribType : int
{
    Byte , UnsignedByte ,
    Short , UnsignedShort ,
    Int , UnsignedInt ,
    Float , Double
};

//////////////////////////////////////////////////////////////////////
/// @brief Describes a Vertex Attribute .
struct VertexAttribComponent
{
    /// @brief Alias used to bind this component. The real component
    /// name will be found by the technique , and the real name location
    /// will be found by the HardwareProgram.
    VertexAttribAlias alias ;

    /// @brief Number of elements for this attribute. Must be 1, 2, 3 or 4.
    /// Default is 4.
    size_t elements ;

    /// @brief Basic type this attribute is. Can be Byte , UnsignedByte ,
    /// Short , UnsignedShort , Int , UnsignedInt , Float and Double.
    VertexAttribType type ;

    /// @brief True if the attribute should be normalized.
    bool normalize ;

    /// @brief Total size of this attribute. This is needed when calculating
    /// offsets positions , and stride between attributes in interleaved arrays.
    size_t size ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Describes a set of attributes , generally stored by one
/// vertex buffer and describing its structure.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC VertexDescriptor
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    VertexDescriptor () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~VertexDescriptor () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const std::vector < VertexAttribComponent > & getComponents () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addComponent ( const VertexAttribComponent & component ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs an Attribute Component and adds it to the
    /// descriptor.
    //////////////////////////////////////////////////////////////////////
    void addComponent (const VertexAttribAlias & alias ,
                       size_t elements ,
                       const VertexAttribType & type ,
                       bool normalize ,
                       size_t size);

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the stride between two same components in this
    /// attribute structure.
    //////////////////////////////////////////////////////////////////////
    size_t getStride ( const VertexAttribComponent & component ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first offset of the given component. Notes you
    /// must add this offset to the Vertex's Buffer 'getData()' return value
    /// to get a valid pointer to the component.
    //////////////////////////////////////////////////////////////////////
    size_t getOffset ( const VertexAttribComponent & component ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void clear () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    size_t getSize () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the component with given Attribute Alias.
    //////////////////////////////////////////////////////////////////////
    const VertexAttribComponent findComponent ( const VertexAttribAlias & alias ) const ;

private:

    /// @brief Arrays of Components that defines the attributes structure.
    std::vector < VertexAttribComponent > iComponents ;

    /// @brief Cached total size for this Vertex Descriptor. This lets us calculate
    /// more effectively strides.
    size_t iSize ;
};

GreEndNamespace

#endif /* VertexDescriptor_h */
