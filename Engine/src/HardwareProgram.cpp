//////////////////////////////////////////////////////////////////////
//
//  HardwareProgram.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
, iVertexShader ( nullptr ) , iFragmentShader ( nullptr )
, iCompiled ( false ) , iBinded ( false )
{
    
}

HardwareProgram::HardwareProgram(const std::string& name,
                                 const HardwareShaderUser& vertexShader,
                                 const HardwareShaderUser& fragmentShader,
                                 bool cacheShaders )
: Resource(name)
, iVertexShader ( nullptr ) , iFragmentShader ( nullptr )
, iCompiled ( false ) , iBinded ( false )
{
    iComponentLocations [VertexComponentType::Position] = 0 ;
    iComponentLocations [VertexComponentType::Color]    = 1 ;
    iComponentLocations [VertexComponentType::Normal]   = 2 ;
    
    if ( cacheShaders )
    {
        iVertexShader = vertexShader ;
        iFragmentShader = fragmentShader ;
    }
}

HardwareProgram::~HardwareProgram() noexcept ( false )
{
    
}

void HardwareProgram::use() const
{
    GreAutolock ;
    
    _bind () ;
    
    if ( iBinded )
    {
        // Check if we have some variables to upload.
        
        for ( const HardwareProgramVariable & var : iCachedVariables ) {
            _setVariable (var) ;
        }
        
        iCachedVariables.clear();
    }
}

void HardwareProgram::unuse() const
{
    GreAutolock ; _unbind() ;
}

void HardwareProgram::setAttribLocation(const std::string &name, const Gre::VertexComponentType &component)
{
    GreAutolock ; _setAttribLocation ( name , getAttribLocation(component) ) ;
}

void HardwareProgram::setAttribLocation(const Gre::VertexComponentType &component, int location)
{
    GreAutolock ; iComponentLocations [component] = location ;
}

int HardwareProgram::getAttribLocation(const Gre::VertexComponentType &component) const
{
    GreAutolock ;
    
    auto it = iComponentLocations.find ( component ) ;
    if ( it == iComponentLocations.end() )
        return -1 ;
    else
        return it->second ;
}

bool HardwareProgram::isCompiled() const
{
    GreAutolock ; return iCompiled ;
}

void HardwareProgram::reset()
{
    GreAutolock ;
    
    iVertexShader.clear();
    iFragmentShader.clear();
    
    _deleteProgram() ;
    iCompiled = false ;
    iBinded = false ;
    
    iComponentLocations.clear() ;
    iComponentLocations [VertexComponentType::Position] = 0 ;
    iComponentLocations [VertexComponentType::Color]    = 1 ;
    iComponentLocations [VertexComponentType::Normal]   = 2 ;
}

void HardwareProgram::setVariable(const Gre::HardwareProgramVariable &var) const
{
    GreAutolock ;
    
    if ( iBinded ) {
        // We are binded , so we can upload the variable to the program.
        _setVariable ( var ) ;
    }
    
    else {
        // Cache this Variable for later uploading.
        iCachedVariables.add(var);
    }
}

GreEndNamespace
