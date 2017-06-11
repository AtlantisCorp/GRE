//////////////////////////////////////////////////////////////////////
//
//  Maths.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/05/2017.
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

#ifndef GRE_MATHS_H
#define GRE_MATHS_H

#include "Version.h"

// The glm library should always be present when compiling GRE. On linux distributions, we could note that
// 'glm::tvec3' classes where 'glm::details::tvec3' and so on for every templated classes.

// #define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

GreBeginNamespace

template<typename T> using TVector2 = glm::tvec2<T, glm::highp>;
template<typename T> using TVector3 = glm::tvec3<T, glm::highp>;
template<typename T> using TVector4 = glm::tvec4<T, glm::highp>;

typedef TVector2<float> Vector2;
typedef TVector3<float> Vector3;
typedef TVector4<float> Vector4;

typedef TVector2 < int > IVector2 ;
typedef TVector3 < int > IVector3 ;
typedef TVector4 < int > IVector4 ;

typedef glm::mat2 Matrix2 ;
typedef glm::mat3 Matrix3 ;
typedef glm::mat4 Matrix4 ;
typedef glm::quat Quaternion;

/// @brief Holds a volume in 3D.
struct Volume
{
    union
    {
        struct
        {
            float width;
            float height;
            float depth;
        };

        struct
        {
            float x;
            float y;
            float z;
        };

        Vector3 vec3;
    };
};

/// @brief Types that define a Radian angle.
typedef float Radian;
typedef float Degree;

/// @brief Simply, a plane is a Vector4 component.
typedef Vector4 Plane;

/// @brief Forward basic direction. (0, 0, 1)
extern const glm::vec3 VectorForward ;

/// @brief Upward basic direction. (0, 1, 0)
extern const glm::vec3 VectorUpward ;

/// @brief Rightward basic direction. (1, 0, 0)
extern const glm::vec3 VectorRightward ;

//////////////////////////////////////////////////////////////////////
/// @brief Finds the rotation quaternion between two vectors.
//////////////////////////////////////////////////////////////////////
glm::quat RotationBetweenVectors ( glm::vec3 start , glm::vec3 dest ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Prints a glm::mat4 description to given ostream.
//////////////////////////////////////////////////////////////////////
std::ostream & operator << ( std::ostream & stream , const glm::mat4 & mat ) ;

GreEndNamespace

#endif /* GRE_MATHS_H */
