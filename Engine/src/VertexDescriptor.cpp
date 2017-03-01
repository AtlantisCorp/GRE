//////////////////////////////////////////////////////////////////////
//
//  VertexDescriptor.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

size_t VertexComponentTypeGetSize(const VertexComponentType& vtype)
{
    size_t totsz = 0;

    if ( vtype == VertexComponentType::Null )
    {
        
    }
    
    else if ( vtype == VertexComponentType::Null1 )
    {
        totsz += 1 * sizeof(uint32_t);
    }
    
    else if ( vtype == VertexComponentType::Null2 )
    {
        totsz += 2 * sizeof(uint32_t);
    }
    
    else if ( vtype == VertexComponentType::Null3 )
    {
        totsz += 3 * sizeof(uint32_t);
    }
    
    else if ( vtype == VertexComponentType::Null4 )
    {
        totsz += 4 * sizeof(uint32_t);
    }
    
    else if ( vtype == VertexComponentType::Position )
    {
        totsz += 3 * sizeof(float);
    }
    
    else if ( vtype == VertexComponentType::Color )
    {
        totsz += 4 * sizeof(float);
    }
    
    else if ( vtype == VertexComponentType::Normal )
    {
        totsz += 3 * sizeof(float);
    }
    
    else if ( vtype == VertexComponentType::Texture )
    {
        totsz += 2 * sizeof(float);
    }
    
    else if ( vtype == VertexComponentType::Tangents )
        return VertexComponentTypeGetSize(VertexComponentType::Normal) ;
    else if ( vtype == VertexComponentType::Binormals )
        return VertexComponentTypeGetSize(VertexComponentType::Normal) ;
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Bad VertexComponentType given : " << (uint32_t) vtype << "." << Gre::gendl;
    }
#endif
    
    return totsz;
}

std::string VertexComponentTypeToString(const VertexComponentType& component)
{
    if ( component == VertexComponentType::Null )
    {
        return "Null";
    }
    
    else if ( component == VertexComponentType::Null1 )
    {
        return "Null1";
    }
    
    else if ( component == VertexComponentType::Null2 )
    {
        return "Null2";
    }
    
    else if ( component == VertexComponentType::Null3 )
    {
        return "Null3";
    }
    
    else if ( component == VertexComponentType::Null4 )
    {
        return "Null4";
    }
    
    else if ( component == VertexComponentType::Position )
    {
        return "Position";
    }
    
    else if ( component == VertexComponentType::Normal )
    {
        return "Normal";
    }
    
    else if ( component == VertexComponentType::Texture )
    {
        return "Texture";
    }
    
    else if ( component == VertexComponentType::Color )
    {
        return "Color";
    }
    
    else if ( component == VertexComponentType::Tangents )
        return "Tangents" ;
    else if ( component == VertexComponentType::Binormals )
        return "Binormals" ;
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Component given not found ('" << (int) component << "')." << Gre::gendl;
    }
#endif
    
    return std::string();
}

// ---------------------------------------------------------------------------------------------------

VertexDescriptor::VertexDescriptor()
{
    iSize = 0;
}

VertexDescriptor::~VertexDescriptor()
{
    
}

void VertexDescriptor::addComponent(const Gre::VertexComponentType &vtype)
{
    iComponents.push_back(vtype);
    iSize += VertexComponentTypeGetSize(vtype);
}

void VertexDescriptor::addComponentUnique(const Gre::VertexComponentType &vtype)
{
    removeComponent(vtype);
    iComponents.push_back(vtype);
    iSize += VertexComponentTypeGetSize(vtype);
}

void VertexDescriptor::removeComponent(const Gre::VertexComponentType &vtype)
{
    for (auto it = iComponents.begin(); it != iComponents.end(); it++)
    {
        if( (*it) == vtype )
        {
            iComponents.erase(it);
            iSize -= VertexComponentTypeGetSize(vtype);
            break;
        }
    }
}

const VertexComponents& VertexDescriptor::getComponents() const
{
    return iComponents;
}

size_t VertexDescriptor::getSize() const
{
    return iSize;
}

int VertexDescriptor::getComponentLocation(const Gre::VertexComponentType &component) const
{
    size_t loc = 0;
    
    for ( auto comp : iComponents )
    {
        if ( comp == component )
        {
            return (int) loc;
        }
        
        else
        {
            loc += VertexComponentTypeGetSize(comp);
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Component '" << VertexComponentTypeToString(component) << "' not found in Vertex." << Gre::gendl;
#endif
    
    return -1;
}

size_t VertexDescriptor::getStride(const Gre::VertexComponentType &vtype) const
{
    if ( iComponents.size() == 1 )
        return 0 ;
    
    return iSize ;
}

size_t VertexDescriptor::getOffset(const Gre::VertexComponentType &component) const
{
    if (iComponents.size() == 1)
        return 0 ;
    
    size_t value = 0 ;
    
    for ( VertexComponentType comp : iComponents )
    {
        if ( comp != component ) {
            value += VertexComponentTypeGetSize(comp);
        } else {
            break ;
        }
    }
    
    return value ;
}

VertexDescriptor VertexDescriptor::Default = VertexDescriptor();

// ---------------------------------------------------------------------------------------------------

VertexDescriptor& operator << (VertexDescriptor& desc, const VertexComponentType& vtype)
{
    desc.addComponent(vtype);
    return desc;
}

GreEndNamespace
