//
//  AxisAlignedBoundingBox.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//

#include "AxisAlignedBoundingBox.h"

GreBeginNamespace

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
    setNull();
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& center, glm::float_t radius)
{
    setNull();
    extend(center, radius);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& p1, const glm::vec3& p2)
{
    setNull();
    extend(p1);
    extend(p2);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& aabb)
{
    setNull();
    extend(aabb);
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

void AxisAlignedBoundingBox::extend(glm::float_t val)
{
    if (!isNull())
    {
        mMin -= glm::vec3(val);
        mMax += glm::vec3(val);
    }
}

void AxisAlignedBoundingBox::extend(const glm::vec3& p)
{
    if (!isNull())
    {
        mMin = glm::min(p, mMin);
        mMax = glm::max(p, mMax);
    }
    else
    {
        mMin = p;
        mMax = p;
    }
}

void AxisAlignedBoundingBox::extend(const glm::vec3& p, glm::float_t radius)
{
    glm::vec3 r(radius);
    if (!isNull())
    {
        mMin = glm::min(p - r, mMin);
        mMax = glm::max(p + r, mMax);
    }
    else
    {
        mMin = p - r;
        mMax = p + r;
    }
}

void AxisAlignedBoundingBox::extend(const AxisAlignedBoundingBox& aabb)
{
    if (!aabb.isNull())
    {
        extend(aabb.mMin);
        extend(aabb.mMax);
    }
}

void AxisAlignedBoundingBox::extendDisk(const glm::vec3& c, const glm::vec3& n, glm::float_t r)
{
    if (glm::length(n) < 1.e-12) { extend(c); return; }
    glm::vec3 norm = glm::normalize(n);
    glm::float_t x = sqrt(1 - norm.x) * r;
    glm::float_t y = sqrt(1 - norm.y) * r;
    glm::float_t z = sqrt(1 - norm.z) * r;
    extend(c + glm::vec3(x,y,z));
    extend(c - glm::vec3(x,y,z));
}

glm::vec3 AxisAlignedBoundingBox::getDiagonal() const
{
    if (!isNull())
        return mMax - mMin;
    else
        return glm::vec3(0);
}

glm::float_t AxisAlignedBoundingBox::getLongestEdge() const
{
    return glm::compMax(getDiagonal());
}

glm::float_t AxisAlignedBoundingBox::getShortestEdge() const
{
    return glm::compMin(getDiagonal());
}

glm::vec3 AxisAlignedBoundingBox::getCenter() const
{
    if (!isNull())
    {
        glm::vec3 d = getDiagonal();
        return mMin + (d * glm::float_t(0.5));
    }
    else
    {
        return glm::vec3(0.0);
    }
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::translate(const glm::vec3& v)
{
    if (!isNull())
    {
        mMin += v;
        mMax += v;
    }
    
    return *this;
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::scale(const glm::vec3& s, const glm::vec3& o)
{
    if (!isNull())
    {
        mMin -= o;
        mMax -= o;
        
        mMin *= s;
        mMax *= s;
        
        mMin += o;
        mMax += o;
    }
    
    return *this;
}

bool AxisAlignedBoundingBox::overlaps(const AxisAlignedBoundingBox& bb) const
{
    if (isNull() || bb.isNull())
        return false;
    
    if( bb.mMin.x > mMax.x || bb.mMax.x < mMin.x)
        return false;
    else if( bb.mMin.y > mMax.y || bb.mMax.y < mMin.y)
        return false;
    else if( bb.mMin.z > mMax.z || bb.mMax.z < mMin.z)
        return false;
    
    return true;
}

AxisAlignedBoundingBox::INTERSECTION_TYPE AxisAlignedBoundingBox::intersect(const AxisAlignedBoundingBox& b) const
{
    if (isNull() || b.isNull())
        return OUTSIDE;
    
    if ((mMax.x < b.mMin.x) || (mMin.x > b.mMax.x) ||
        (mMax.y < b.mMin.y) || (mMin.y > b.mMax.y) ||
        (mMax.z < b.mMin.z) || (mMin.z > b.mMax.z))
    {
        return OUTSIDE;
    }
    
    if ((mMin.x <= b.mMin.x) && (mMax.x >= b.mMax.x) &&
        (mMin.y <= b.mMin.y) && (mMax.y >= b.mMax.y) &&
        (mMin.z <= b.mMin.z) && (mMax.z >= b.mMax.z))
    {
        return INSIDE;
    }
    
    return INTERSECT;
}


bool AxisAlignedBoundingBox::isSimilarTo(const AxisAlignedBoundingBox& b, glm::float_t diff) const
{
    if (isNull() || b.isNull()) return false;
    
    glm::vec3 acceptable_diff=( (getDiagonal()+b.getDiagonal()) / glm::float_t(2.0))*diff;
    glm::vec3 min_diff(mMin-b.mMin);
    min_diff = glm::vec3(fabs(min_diff.x),fabs(min_diff.y),fabs(min_diff.z));
    if (min_diff.x > acceptable_diff.x) return false;
    if (min_diff.y > acceptable_diff.y) return false;
    if (min_diff.z > acceptable_diff.z) return false;
    glm::vec3 max_diff(mMax-b.mMax);
    max_diff = glm::vec3(fabs(max_diff.x),fabs(max_diff.y),fabs(max_diff.z));
    if (max_diff.x > acceptable_diff.x) return false;
    if (max_diff.y > acceptable_diff.y) return false;
    if (max_diff.z > acceptable_diff.z) return false;
    return true;
}

bool AxisAlignedBoundingBox::hasVolume() const
{
    return getDiagonal() != Vector3(0,0,0);
}

bool AxisAlignedBoundingBox::contains(const glm::vec3& p) const
{
    AxisAlignedBoundingBox tmp;
    tmp.extend(p);
    INTERSECTION_TYPE inters = intersect(tmp);
    return inters == INSIDE;
}

GreEndNamespace
