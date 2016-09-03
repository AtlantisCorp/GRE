//////////////////////////////////////////////////////////////////////
//
//  BoundingBox.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/08/2016.
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

#ifndef BoundingBox_h
#define BoundingBox_h

#include "Pools.h"
#include "Transformation.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief An Axis-Aligned Bounding Box.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC BoundingBox
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    BoundingBox();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    BoundingBox ( const Vector3& minpoint , const Vector3& maxpoint );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    BoundingBox ( const BoundingBox& rhs );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~BoundingBox();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Maximum point of the BoundingBox.
    //////////////////////////////////////////////////////////////////////
    const Vector3& getMax() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Minimum point of the Bounding Box.
    //////////////////////////////////////////////////////////////////////
    const Vector3& getMin() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Modifies the BoundingBox in order to englobate the given
    /// point.
    //////////////////////////////////////////////////////////////////////
    void add ( const Vector3& point );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Fusion this BoundingBox and given one.
    //////////////////////////////////////////////////////////////////////
    void add ( const BoundingBox& rhs );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this BoundingBox contains given point.
    //////////////////////////////////////////////////////////////////////
    bool contains ( const Vector3& point ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this BoundingBox contains given BoundingBox.
    //////////////////////////////////////////////////////////////////////
    bool contains ( const BoundingBox rhs ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the BoundingBox.
    //////////////////////////////////////////////////////////////////////
    void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Apply given Transformation to this BoundingBox.
    /// @note This is an Axis-Aligned BoundingBox. Only the 'scale' and
    /// 'translation' property of the Transformation will be taken.
    //////////////////////////////////////////////////////////////////////
    void apply ( const Transformation& transf );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'true' if 'iIsSet' is false.
    //////////////////////////////////////////////////////////////////////
    bool isInvalid() const;
    
    /// @brief Setter operator.
    BoundingBox& operator = ( const BoundingBox& rhs );
    
protected:
    
    /// @brief Minimum Point of the BoundingBox.
    Vector3 iMin;
    
    /// @brief Maximum Point of the BoundingBox.
    Vector3 iMax;
    
    /// @brief Tells if the BoundingBox has been set once.
    bool iIsSet;
};

GreEndNamespace

#endif /* BoundingBox_h */
