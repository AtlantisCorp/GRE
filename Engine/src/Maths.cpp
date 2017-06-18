//////////////////////////////////////////////////////////////////////
//
//  Maths.cpp
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

#include "Maths.h"

GreBeginNamespace

/// @brief Forward basic direction. (0, 0, -1)
const glm::vec3 VectorForward = glm::vec3 ( 0.0f , 0.0f , - 1.0f ) ;

/// @brief Upward basic direction. (0, 1, 0)
const glm::vec3 VectorUpward = glm::vec3 ( 0.0f , 1.0f , 0.0f ) ;

/// @brief Rightward basic direction. (1, 0, 0)
const glm::vec3 VectorRightward = glm::vec3 ( 1.0f , 0.0f , 0.0f ) ;

const float FloatPrecision = 0.001f ;

glm::quat RotationBetweenVectors ( glm::vec3 start , glm::vec3 dest )
{
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = glm::dot (start, dest) ;
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f)
    {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start

        rotationAxis = glm::cross ( VectorForward , start ) ;
        if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
        rotationAxis = glm::cross ( VectorUpward , start ) ;

        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float s = glm::sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return glm::quat(
                s * 0.5f,
                rotationAxis.x * invs,
                rotationAxis.y * invs,
                rotationAxis.z * invs
            );
}

glm::quat QuaternionLookAt ( glm::vec3 direction , glm::vec3 desiredUp )
{

}

std::ostream & operator << ( std::ostream & stream , const glm::mat4 & mat )
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        stream << "\t [" << i << "] " ;

        for ( int j = 0 ; j < 4 ; ++j )
        stream << mat[j][i] << " | " ;

        stream << std::endl ;
    }

    return stream ;
}

GreEndNamespace
