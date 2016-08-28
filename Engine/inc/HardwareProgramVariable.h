//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramVariable.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

/// @brief Describes HardwareProgramVariable Types used in the Engine.
/// Those types are taken from GLSL types.
enum class HdwProgVarType
{
    None, ///< @brief Reserved for invalid Variables.
    Vec2, ///< @brief vec2 GLSL type.
    Vec3, ///< @brief vec3 GLSL type.
    Vec4, ///< @brief vec4 GLSL type.
    Mat4, ///< @brief mat4 GLSL type.
    Uint32, ///< @brief uint GLSL type.
    Sampler2D, ///< @brief sampler2d GLSL type.
    Sampler3D, ///< @brief sampler3d GLSL type.
    Float1 ///< @brief A Float variable.
};

//////////////////////////////////////////////////////////////////////
/// @brief Holds a simple Program Variable.
//////////////////////////////////////////////////////////////////////
struct HardwareProgramVariable
{
    /// @brief Constructs a default variable with type 'none'.
    HardwareProgramVariable();
    
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
    union _u
    {
        Vector2 vec2;
        Vector3 vec3;
        Vector4 vec4;
        Matrix4 mat4;
        uint32_t uint32;
        int textureunit;
        float float1;
        
        _u() { memset(this, 0, sizeof(_u)); }
        
    } value;
    
    /// @brief A Null HardwareProgramVariable.
    static HardwareProgramVariable Null;
};

GreEndNamespace

#endif
