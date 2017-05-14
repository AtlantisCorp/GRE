//////////////////////////////////////////////////////////////////////
//
//  BoundingBox.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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

BoundingBox::BoundingBox ( const Vector3 & volume )
{
    Vector3 absvol = glm::abs ( volume ) ;

    iMin.x = - (absvol.x / 2.0f) ;
    iMin.y = - (absvol.y / 2.0f) ;
    iMin.z = - (absvol.z / 2.0f) ;

    iMax.x = absvol.x / 2.0f ;
    iMax.y = absvol.y / 2.0f ;
    iMax.z = absvol.z / 2.0f ;

    iIsSet = true ;
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

void BoundingBox::translateTo ( const Vector3 & point )
{
    iMin = iMin + point ;
    iMax = iMax + point ;
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

IntersectionResult BoundingBox::intersect(const Gre::BoundingBox &rhs) const
{
    //////////////////////////////////////////////////////////////////////
    // Computes, for each axis , the result. The result is : this bounding
    // box intersect with rhs ? Yes , this bbox is inside , outside or between
    // rhs.

    if (iMin.x > rhs.iMin.x && iMin.y > rhs.iMin.y && iMin.z > rhs.iMin.z &&
        iMax.x < rhs.iMax.x && iMax.y < rhs.iMax.y && iMax.z < rhs.iMax.z)
    return IntersectionResult::Inside ;

    if (iMin.x < rhs.iMax.x && iMax.x > rhs.iMin.x &&
        iMin.y < rhs.iMax.y && iMax.y > rhs.iMin.y &&
        iMin.z < rhs.iMax.z && iMax.z > rhs.iMin.z)
    return IntersectionResult::Between ;

    return IntersectionResult::Outside ;
}

float BoundingBox::diameterlen () const
{
    return glm::length ( diameter() ) ;
}

Vector3 BoundingBox::diameter () const
{
    return iMax - iMin ;
}

Vector3 BoundingBox::center () const
{
    Vector3 diam = diameter () ;
    return iMin + Vector3 ( diam.x / 2.0f , diam.y / 2.0f , diam.z / 2.0f ) ;
}

GreEndNamespace
