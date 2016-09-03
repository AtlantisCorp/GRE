//////////////////////////////////////////////////////////////////////
//
//  BoundingBox.cpp
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

#include "BoundingBox.h"

GreBeginNamespace

BoundingBox::BoundingBox()
: iMin(0.0f, 0.0f, 0.0f)
, iMax(0.0f, 0.0f, 0.0f)
, iIsSet(false)
{
    
}

BoundingBox::BoundingBox(const Vector3& minpoint, const Vector3& maxpoint)
: iMin(0.0f, 0.0f, 0.0f)
, iMax(0.0f, 0.0f, 0.0f)
, iIsSet(false)
{
    iMin.x = glm::min(minpoint.x, maxpoint.x);
    iMin.y = glm::min(minpoint.y, maxpoint.y);
    iMin.z = glm::min(minpoint.z, maxpoint.z);
    
    iMax.x = glm::max(minpoint.x, maxpoint.x);
    iMax.y = glm::max(minpoint.y, maxpoint.y);
    iMax.z = glm::max(minpoint.z, maxpoint.z);
    
    iIsSet = true;
}

BoundingBox::BoundingBox(const BoundingBox& rhs)
: iMin(rhs.iMin)
, iMax(rhs.iMax)
, iIsSet(rhs.iIsSet)
{
    
}

BoundingBox::~BoundingBox()
{
    
}

const Vector3& BoundingBox::getMax() const
{
    return iMax;
}

const Vector3& BoundingBox::getMin() const
{
    return iMin;
}

void BoundingBox::add(const Vector3 &point)
{
    if ( iIsSet )
    {
        iMin.x = glm::min(iMin.x, point.x);
        iMin.y = glm::min(iMin.y, point.y);
        iMin.z = glm::min(iMin.z, point.z);
        
        iMax.x = glm::max(iMax.x, point.x);
        iMax.y = glm::max(iMax.y, point.y);
        iMax.z = glm::max(iMax.z, point.z);
    }
    
    else
    {
        iMin = point;
        iMax = point;
        iIsSet = true;
    }
}

void BoundingBox::add(const Gre::BoundingBox &rhs)
{
    add(rhs.iMin);
    add(rhs.iMax);
}

bool BoundingBox::contains(const Vector3 &point) const
{
    if ( iIsSet )
    {
        if ( iMin.x < point.x && iMin.y < point.y && iMin.z < point.z &&
             iMax.x > point.x && iMax.y > point.y && iMax.z > point.z )
        {
            return true;
        }
        
        else
        {
            return false;
        }
    }
    
    else
    {
        return false;
    }
}

bool BoundingBox::contains(const Gre::BoundingBox rhs) const
{
    return contains(rhs.iMin) && contains(rhs.iMax);
}

void BoundingBox::clear()
{
    iMin = Vector3(0.0f, 0.0f, 0.0f);
    iMax = Vector3(0.0f, 0.0f, 0.0f);
    iIsSet = false;
}

void BoundingBox::apply(const Gre::Transformation &transf)
{
    if ( iIsSet )
    {
        Vector3 translation = transf.getTranslation();
        
        iMin = iMin + translation;
        iMax = iMax + translation;
        
        Vector3 scale = transf.getScale();
        
        if ( scale.x != 1.0f )
        {
            float diff = iMax.x - iMin.x;
            float newdiff = scale.x * diff;
            
            diff = newdiff - diff;
            diff = diff / 2;
            
            iMin.x = iMin.x - diff;
            iMax.x = iMax.x + diff;
        }
        
        if ( scale.y != 1.0f )
        {
            float diff = iMax.y - iMin.y;
            float newdiff = scale.y * diff;
            
            diff = newdiff - diff;
            diff = diff / 2;
            
            iMin.y = iMin.y - diff;
            iMax.y = iMax.y + diff;
        }
        
        if ( scale.z != 1.0f )
        {
            float diff = iMax.z - iMin.z;
            float newdiff = scale.z * diff;
            
            diff = newdiff - diff;
            diff = diff / 2;
            
            iMin.z = iMin.z - diff;
            iMax.z = iMax.z + diff;
        }
    }
}

bool BoundingBox::isInvalid() const
{
    return !iIsSet;
}

BoundingBox& BoundingBox::operator=(const Gre::BoundingBox &rhs)
{
    iMin = rhs.iMin;
    iMax = rhs.iMax;
    iIsSet = rhs.iIsSet;
    return *this;
}

GreEndNamespace
