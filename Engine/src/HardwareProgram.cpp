//////////////////////////////////////////////////////////////////////
//
//  HardwareProgram.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 06/01/2016.
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

#include "HardwareProgram.h"
#include "Material.h"

GreBeginNamespace

HardwareProgram::HardwareProgram ( const std::string & name )
: Gre::Resource ( name )
, iLinked ( false ) , iBinded ( false )
{
    iAttachedShaders[ShaderType::Vertex] = HardwareShaderHolder ( nullptr ) ;
    iAttachedShaders[ShaderType::Fragment] = HardwareShaderHolder ( nullptr ) ;
}

HardwareProgram::~HardwareProgram() noexcept ( false )
{

}

void HardwareProgram::attachShaders ( const HardwareShaderHolderList & shaders )
{
    if ( iLinked )
        return ;

    //////////////////////////////////////////////////////////////////////
    // Attaches every shaders to the object. Notes that if a shader type is
    // already attached to another shader, the behaviour is platform-dependent
    // and not guaranteed, but usually it should overwrite the attached shader.

    for ( auto shader : shaders )
    {
        if ( !shader.isInvalid() )
        {
            if ( _attachShader(shader) ) {
                iAttachedShaders[shader->getType()] = shader ;
            }
        }
    }
}

void HardwareProgram::finalize ()
{
    //////////////////////////////////////////////////////////////////////
    // If the object is not already finalized, call the implementation
    // function '_finalize'.

    if ( !iLinked ) {
        iLinked = _finalize () ;
    }
}

void HardwareProgram::use() const
{
    GreAutolock ;

    _bind () ;

    if ( iBinded )
    {
        // Check if we have some variables to upload. Those variables have been set before binding the program
        // so they were saved in order to set them when using the program.

        for ( const HardwareProgramVariable & var : iCachedVariables ) {
            setUniform (var) ;
        }

        iCachedVariables.clear();
    }
}

void HardwareProgram::unuse() const
{
    GreAutolock ; _unbind() ;
}

bool HardwareProgram::isFinalized () const
{
    GreAutolock ; return iLinked ;
}

bool HardwareProgram::isLinked() const
{
    GreAutolock ; return iLinked ;
}

void HardwareProgram::reset()
{
    GreAutolock ;

    iAttachedShaders.clear () ;

    _deleteProgram() ;
    iLinked = false ;
    iBinded = false ;

    iCachedVariables.clear() ;
    iUniforms.clear() ;
    iAttribsLocation.clear() ;
}

bool HardwareProgram::setUniform ( const std::string & name , const HdwProgVarType & type , const RealProgramVariable & value ) const
{
	GreAutolock ;

	if ( isLinked() )
	{
        //////////////////////////////////////////////////////////////////////
        // We must check if the given variable has an existing name, and a
        // correct type.

		auto it = iUniforms.find ( name ) ;

		if ( it != iUniforms.end() )
        {
            if ( it->second.type == type )
			return _setUniform ( it->second.location , type , value ) ;
		}
	}

	return false ;
}

bool HardwareProgram::setUniform ( const HardwareProgramVariable & variable ) const
{
    return setUniform ( variable.name , variable.type , variable.value ) ;
}

bool HardwareProgram::isUniformValid(const std::string &name) const
{
    GreAutolock ; return iUniforms.find(name) != iUniforms.end() ;
}

int HardwareProgram::getUniformLocation(const std::string &name) const
{
    GreAutolock ; auto it = iUniforms.find(name) ;

    if ( it == iUniforms.end() ) return -1 ;
    return it -> second.location ;
}

GreEndNamespace
