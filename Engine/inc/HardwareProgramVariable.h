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
/// Those types are taken from GLSL types. Notes that Samplers types
/// are handled as unsigned int types to registers 'sampler' value,
/// or texture unit.
enum class HdwProgVarType
{
    None, ///< @brief Reserved for invalid Variables.

    Float1, Float2, Float3, Float4,
    Int1, Int2, Int3, Int4,
    UnsignedInt1, UnsignedInt2, UnsignedInt3, UnsignedInt4,
    Bool1, Bool2, Bool3, Bool4,
    Matrix2, Matrix3, Matrix4,

    Sampler1D ,
    Sampler2D ,
    Sampler3D ,
    SamplerCube
};

/// @brief Returns the HdwProgVarType from string.
HdwProgVarType HdwProgVarTypeFromString ( const std::string & type );

//////////////////////////////////////////////////////////////////////
/// @brief Defines real types in a union to hold every variable types
/// possibles.
union RealProgramVariable
{

    float f1 ; Vector2 f2 ;
    Vector3 f3 ; Vector4 f4 ;

    int i1 ; IVector2 i2 ;
    IVector3 i3 ; IVector4 i4 ;

    unsigned int u1 ; glm::uvec2 u2 ;
    glm::uvec3 u3 ; glm::uvec4 u4 ;

    bool b1 ; glm::bvec2 b2 ;
    glm::bvec3 b3 ; glm::bvec4 b4 ;

    Matrix2 m2 ; Matrix3 m3 ; Matrix4 m4 ;

    RealProgramVariable () { memset( this, 0, sizeof(RealProgramVariable) ); }

    RealProgramVariable ( const RealProgramVariable & rhs)
    {
    	memset( this, 0, sizeof(rhs) );
    	memcpy( this, &rhs, sizeof(rhs) );
    }

    RealProgramVariable& operator = ( const RealProgramVariable& rhs )
    {
    	memset( this, 0, sizeof(rhs) );
    	memcpy( this, &rhs, sizeof(rhs) );
        return *this ;
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

    RealProgramVariable ( const unsigned int & v ) : u1(v) { }
    RealProgramVariable ( const glm::uvec2 & v ) : u2(v) { }
    RealProgramVariable ( const glm::uvec3 & v ) : u3(v) { }
    RealProgramVariable ( const glm::uvec4 & v ) : u4(v) { }

    RealProgramVariable ( const bool & v ) : b1(v) { }
    RealProgramVariable ( const glm::bvec2 & v ) : b2(v) { }
    RealProgramVariable ( const glm::bvec3 & v ) : b3(v) { }
    RealProgramVariable ( const glm::bvec4 & v ) : b4(v) { }
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
