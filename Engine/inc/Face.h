//////////////////////////////////////////////////////////////////////
//
//  Face.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/11/2015.
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

#ifndef GRE_Face_h
#define GRE_Face_h

#include "Material.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
enum class IndexType
{
    /// @brief Invalid
    Null ,
    
    /// @brief unsigned int 's type.
    UnsignedInteger ,
    
    /// @brief short's type.
    UnsignedShort ,
    
    /// @brief unsigned byte's type.
    UnsignedByte
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
enum class IndexDrawmode
{
    Points , Lines , Triangles , Patches
};

/// @brief Returns the Size for the given type, in bytes.
size_t IndexTypeGetSize(const IndexType& itype);

//////////////////////////////////////////////////////////////////////
/// @brief Holds informations about a batch of indexes.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC IndexDescriptor
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    IndexDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~IndexDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Index's type.
    //////////////////////////////////////////////////////////////////////
    const IndexType& getType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the index's type.
    //////////////////////////////////////////////////////////////////////
    void setType(const IndexType& type);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iMode'
    //////////////////////////////////////////////////////////////////////
    const IndexDrawmode& getMode () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iMode'.
    //////////////////////////////////////////////////////////////////////
    void setMode ( const IndexDrawmode& mode ) ;
    
    /// @brief A default IndexDesciptor.
    static IndexDescriptor Default;
    
protected:
    
    /// @brief Holds the indexe's type (like int , unsigned int, ...)
    IndexType iType;
    
    /// @brief Holds index drawing mode.
    IndexDrawmode iMode ;
};

GreEndNamespace

#endif
