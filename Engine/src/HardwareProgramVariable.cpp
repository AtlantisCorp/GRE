//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramVariable.cpp
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

#include "HardwareProgramVariable.h"

GreBeginNamespace

// -----------------------------------------------------------------------------
// External functions.

HdwProgVarType HdwProgVarTypeFromString( const std::string & type )
{
    if ( type == "Float1" ) return HdwProgVarType::Float1 ;
    else if ( type == "Float2" ) return HdwProgVarType::Float2 ;
    else if ( type == "Float3" ) return HdwProgVarType::Float3 ;
    else if ( type == "Float4" ) return HdwProgVarType::Float4 ;

    else if ( type == "Int1" ) return HdwProgVarType::Int1 ;
    else if ( type == "Int2" ) return HdwProgVarType::Int2 ;
    else if ( type == "Int3" ) return HdwProgVarType::Int3 ;
    else if ( type == "Int4" ) return HdwProgVarType::Int4 ;

    else if ( type == "UnsignedInt1" ) return HdwProgVarType::UnsignedInt1 ;
    else if ( type == "UnsignedInt2" ) return HdwProgVarType::UnsignedInt2 ;
    else if ( type == "UnsignedInt3" ) return HdwProgVarType::UnsignedInt3 ;
    else if ( type == "UnsignedInt4" ) return HdwProgVarType::UnsignedInt4 ;

    else if ( type == "Bool1" ) return HdwProgVarType::Bool1 ;
    else if ( type == "Bool2" ) return HdwProgVarType::Bool2 ;
    else if ( type == "Bool3" ) return HdwProgVarType::Bool3 ;
    else if ( type == "Bool4" ) return HdwProgVarType::Bool4 ;

    else if ( type == "Matrix2" ) return HdwProgVarType::Matrix2 ;
    else if ( type == "Matrix3" ) return HdwProgVarType::Matrix3 ;
    else if ( type == "Matrix4" ) return HdwProgVarType::Matrix4 ;

    else if ( type == "Sampler1D" ) return HdwProgVarType::Sampler1D ;
    else if ( type == "Sampler2D" ) return HdwProgVarType::Sampler2D ;
    else if ( type == "Sampler3D" ) return HdwProgVarType::Sampler3D ;
    else if ( type == "SamplerCube" ) return HdwProgVarType::SamplerCube ;

    return HdwProgVarType::None ;
}

// -----------------------------------------------------------------------------
// HardwareProgramVariable implementation.

HardwareProgramVariable::HardwareProgramVariable()
{
    type = HdwProgVarType::None;
    location = -1;
}

HardwareProgramVariable::HardwareProgramVariable(const HardwareProgramVariable& rhs)
: name (rhs.name)
, location (rhs.location)
, type (rhs.type)
, isArrayElement (rhs.isArrayElement)
, elementNumber (rhs.elementNumber)
, value (rhs.value)
{

}

HardwareProgramVariable HardwareProgramVariable::Null = HardwareProgramVariable();

GreEndNamespace
