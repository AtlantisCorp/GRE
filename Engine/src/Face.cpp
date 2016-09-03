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
        GreDebugPretty() << "Invalid 'IndexType' given !" << std::endl;
#endif
        return 0;
    }
}

// ---------------------------------------------------------------------------------------------------

IndexDescriptor::IndexDescriptor()
: iType(IndexType::Null), iMaterial(Material::Null)
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

const Material& IndexDescriptor::getMaterial() const
{
    return iMaterial;
}

void IndexDescriptor::setMaterial(const Gre::Material &material)
{
    iMaterial = material;
}

IndexDescriptor IndexDescriptor::Default = IndexDescriptor();

// ---------------------------------------------------------------------------------------------------

IndexBatch::IndexBatch()
: iDescriptor()
, iSize (0)
, iData (nullptr)
{
    
}

IndexBatch::IndexBatch(const IndexBatch& rhs)
: iDescriptor(rhs.iDescriptor)
, iSize(0)
, iData(nullptr)
{
    setData(rhs.getData(), rhs.getSize());
}

IndexBatch::~IndexBatch()
{
    clear();
}

const IndexDescriptor& IndexBatch::getDescriptor() const
{
    return iDescriptor;
}

void IndexBatch::setDescriptor(const Gre::IndexDescriptor &desc)
{
    iDescriptor = desc;
}

const char* IndexBatch::getData() const
{
    return iData;
}

size_t IndexBatch::getSize() const
{
    return iSize;
}

void IndexBatch::addData(const char* data, size_t sz)
{
    if ( iData )
    {
        char* tmp = iData;
        iData = (char*) malloc(sizeof(char) * (sz + iSize));
        memcpy(iData, tmp, iSize);
        memcpy(iData + iSize, data, sz);
        iSize = iSize + sz;
    }
    
    else
    {
        setData(data, sz);
    }
}

void IndexBatch::setData(const char *data, size_t sz)
{
    if ( iData )
    {
        clear();
    }
    
    iData = (char*) malloc (sz * sizeof(char));
    memcpy(iData, data, sz);
    iSize = sz;
}

void IndexBatch::clear()
{
    if ( iData && iSize )
    {
        free(iData);
        iData = nullptr;
        iSize = 0;
    }
    
    if ( iData )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Freeing 'IndexBatch::iData' but 'iSize' is invalid." << std::endl;
#endif
        free(iData);
        iData = nullptr;
        iSize = 0;
    }
}

IndexBatch& IndexBatch::operator=(const Gre::IndexBatch &rhs)
{
    setDescriptor(rhs.getDescriptor());
    setData(rhs.getData(), rhs.getSize());
    return *this;
}

GreEndNamespace
