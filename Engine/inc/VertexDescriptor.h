//////////////////////////////////////////////////////////////////////
//
//  VertexDescriptor.h
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

#ifndef VertexDescriptor_h
#define VertexDescriptor_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
enum class VertexComponentType
{
    Null = 0,
    
    /// @brief Disabled field size 1. ( Size = sizeof(uint32_t) )
    Null1 = 1,
    
    /// @brief Disabled field size 2. ( Size = sizeof(uint32_t) * 2 )
    Null2 = 2,
    
    /// @brief Disabled field size 3. ( Size = sizeof(uint32_t) * 3 )
    Null3 = 3,
    
    /// @brief Disabled field size 4. ( Size = sizeof(uint32_t) * 4 )
    Null4 = 4,
    
    /// @brief A XYZ Position Component. (Size = sizeof(float) * 3)
    Position = 10,
    
    /// @brief A RGBA Color component. (Size = sizeof(uint32_t) * 4)
    Color = 11,
    
    /// @brief A XYZ Normal Component. (Size = sizeof(float) * 3)
    Normal = 12,
    
    /// @brief A UV Texture Component. (Size = sizeof(float) * 2)
    Texture = 13
};

/// @brief A VertexPosition.
typedef TVector3<float> VertexPosition;

/// @brief A VertexColor.
typedef TVector4<uint32_t> VertexColor;

/// @brief A VertexNormal.
typedef TVector3<float> VertexNormal;

/// @brief A VertexTexture.
typedef TVector2<float> VertexTexture;

/// @brief A List of Component.
typedef std::vector<VertexComponentType> VertexComponents;

/// @brief Returns the size of given Component, in bytes.
size_t VertexComponentTypeGetSize(const VertexComponentType& vtype);

/// @brief Returns a Readable name for given Component.
std::string VertexComponentTypeToString ( const VertexComponentType& component );

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC VertexDescriptor
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    VertexDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~VertexDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Push a Component at the end of the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addComponent(const VertexComponentType& vtype);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Pushes a Component at the end of the list. If a component
    /// of the same type is already in the list, removes it before.
    //////////////////////////////////////////////////////////////////////
    virtual void addComponentUnique(const VertexComponentType& vtype);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Remove given Component from the list.
    //////////////////////////////////////////////////////////////////////
    virtual void removeComponent(const VertexComponentType& vtype);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Components from this Descriptor.
    //////////////////////////////////////////////////////////////////////
    virtual const VertexComponents& getComponents() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the total size for this Descriptor. (Size of every
    /// Components)
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Component location in the Vertex structure.
    //////////////////////////////////////////////////////////////////////
    virtual int getComponentLocation ( const VertexComponentType& component ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the stride between two same Components. ( getSize()
    /// - VertexComponentTypeGetSize(vtype) )
    //////////////////////////////////////////////////////////////////////
    virtual size_t getStride(const VertexComponentType& vtype) const;
    
    /// @brief Default Vertex Descriptor (Position + Color + Normal + Texture, in this order).
    static VertexDescriptor Default;
    
protected:
    
    /// @brief List of Components in this Vertex, by order.
    VertexComponents iComponents;
    
    /// @brief Updated size of this Descriptor.
    size_t iSize;
};

/// @brief Common operator to write VertexDescriptor.
VertexDescriptor& operator << (VertexDescriptor& desc, const VertexComponentType& vtype);

GreEndNamespace

#endif /* VertexDescriptor_h */
