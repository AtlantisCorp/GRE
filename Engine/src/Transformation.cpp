//////////////////////////////////////////////////////////////////////
//
//  Transformation.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 15/06/2016.
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

#include "Transformation.h"

GreBeginNamespace

Transformation::Transformation()
: iTranslation(0,0,0), iRotation(), iScale(1,1,1)
{
    
}

Transformation::Transformation(const Vector3& translation)
: iTranslation(translation), iRotation(), iScale(1,1,1)
{
    
}

Transformation::Transformation(const Vector3& translation, const Matrix4& rotation)
: iTranslation(translation), iRotation(rotation), iScale(1,1,1)
{
    
}

Transformation::Transformation(const Vector3& translation, const Matrix4& rotation, const Vector3& scale)
: iTranslation(translation), iRotation(rotation), iScale(scale)
{
    
}

Transformation::Transformation(const Transformation& other)
: iTranslation(other.iTranslation), iRotation(other.iRotation), iScale(other.iScale)
{
    
}

Transformation::~Transformation()
{
    
}

void Transformation::translate(const Vector3 &translation)
{
    iTranslation += translation;
}

void Transformation::setTranslation(const Vector3 &translation)
{
    iTranslation = translation;
}

const Vector3& Transformation::getTranslation() const
{
    return iTranslation;
}

void Transformation::rotate(float angle, const Vector3 &axis)
{
    iRotation = glm::rotate(iRotation, angle, axis);
}

void Transformation::setRotation(const Quaternion &rotation)
{
    iRotation = rotation;
}

const Quaternion& Transformation::getRotation() const
{
    return iRotation;
}

void Transformation::scale(const Vector3 &scalev)
{
    iScale *= scalev;
}

void Transformation::setScale(const Vector3 &scalev)
{
    iScale = scalev;
}

const Vector3& Transformation::getScale() const
{
    return iScale;
}

Matrix4 Transformation::get() const
{
    return glm::translate(glm::scale(glm::toMat4(iRotation), iScale), iTranslation);
}

void Transformation::apply(const Gre::Transformation &transformation)
{
    translate(transformation.iTranslation);
    setRotation(transformation.iRotation * iRotation);
    scale(transformation.iScale);
}

Transformation Transformation::Default = Transformation();

GreEndNamespace


