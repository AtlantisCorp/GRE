//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramVariable.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/01/2016.
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

#ifndef GRE_HardwareProgramVariable_h
#define GRE_HardwareProgramVariable_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Describes HardwareProgramVariable Types used in the Engine.
/// Those types are taken from GLSL types.
enum class HdwProgVarType
{
    None, ///< @brief Reserved for invalid Variables.

    Float1, Float2, Float3, Float4,
    Int1, Int2, Int3, Int4,

    Matrix2, Matrix3, Matrix4
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines real types in a union to hold every variable types
/// possibles.
union RealProgramVariable
{

    float f1 ; Vector2 f2 ; Vector3 f3 ; Vector4 f4 ;
    int i1 ; IVector2 i2 ; IVector3 i3 ; IVector4 i4 ;

    Matrix2 m2 ; Matrix3 m3 ; Matrix4 m4 ;

    RealProgramVariable () { memset( this, 0, sizeof(RealProgramVariable) ); }

    RealProgramVariable ( const RealProgramVariable & rhs)
    {
    	memset( this, 0, sizeof(rhs) );
    	memcpy( this, &rhs, sizeof(rhs) );
    }

    RealProgramVariable ( const float& v ) : f1(v) { }
    RealProgramVariable ( const Vector2& v ) : f2(v) { }
    RealProgramVariable ( const Vector3& v ) : f3(v) { }
    RealProgramVariable ( const Vector4& v ) : f4(v) { }

    RealProgramVariable ( const int& v ) : i1(v) { }
    RealProgramVariable ( const IVector2& v ) : i2(v) { }
    RealProgramVariable ( const IVector3& v ) : i3(v) { }
    RealProgramVariable ( const IVector4& v ) : i4(v) { }

    RealProgramVariable ( const Matrix2& v ) : m2(v) { }
    RealProgramVariable ( const Matrix3& v ) : m3(v) { }
    RealProgramVariable ( const Matrix4& v ) : m4(v) { }
};

//////////////////////////////////////////////////////////////////////
/// @brief Holds a simple Program Variable.
//////////////////////////////////////////////////////////////////////
struct HardwareProgramVariable
{
    /// @brief Constructs a default variable with type 'none'.
    HardwareProgramVariable();

    /// @brief Copy Constructor.
    HardwareProgramVariable ( const HardwareProgramVariable& rhs );

    /// @brief Name of the Variable. This should be the same as the one
    /// in the shader file, or you can set a location.
    std::string name;

    /// @brief [Optional] Location of the variable in the shader. This can
    /// be default value ( -1 ) and will be ignored. If this value is set,
    /// this is prioritary on variable's name.
    int location;

    /// @brief Type of the Variable.
    HdwProgVarType type;

    /// @brief If this property is 'true', properties 'elementNumber' is used.
    bool isArrayElement;

    /// @brief Describes the index of this Variable in the Array given by 'name'. This
    /// property is used only if 'isArrayElement' is 'true'.
    int elementNumber;

    /// @brief Should be self-explainatory.
    RealProgramVariable value;

    /// @brief A Null HardwareProgramVariable.
    static HardwareProgramVariable Null;
};

GreEndNamespace

#endif
