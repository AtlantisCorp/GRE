//////////////////////////////////////////////////////////////////////
//
//  Face.cpp
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

#include "Face.h"

GreBeginNamespace

size_t IndexTypeGetSize(const IndexType& itype)
{
    if ( itype == IndexType::Null )
    {
        return 0;
    }
    
    else if ( itype == IndexType::UnsignedByte )
    {
        return sizeof ( unsigned char );
    }
    
    else if ( itype == IndexType::UnsignedShort )
    {
        return sizeof ( unsigned short );
    }
    
    else if ( itype == IndexType::UnsignedInteger )
    {
        return sizeof ( unsigned int );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Invalid 'IndexType' given !" << Gre::gendl;
#endif
        return 0;
    }
}

// ---------------------------------------------------------------------------------------------------

IndexDescriptor::IndexDescriptor()
: iType(IndexType::UnsignedInteger), iMode(IndexDrawmode::Triangles)
{
    
}

IndexDescriptor::~IndexDescriptor()
{
    
}

const IndexType& IndexDescriptor::getType() const
{
    return iType;
}

void IndexDescriptor::setType(const Gre::IndexType &type)
{
    iType = type;
}

const IndexDrawmode& IndexDescriptor::getMode () const
{
    return iMode ;
}

void IndexDescriptor::setMode ( const IndexDrawmode& mode )
{
    iMode = mode ;
}

IndexDescriptor IndexDescriptor::Default = IndexDescriptor();

GreEndNamespace
