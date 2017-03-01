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
    iComponentLocations [VertexComponentType::Position]  = 0 ;
    iComponentLocations [VertexComponentType::Color]     = 1 ;
    iComponentLocations [VertexComponentType::Normal]    = 2 ;
    iComponentLocations [VertexComponentType::Texture]   = 3 ;
    iComponentLocations [VertexComponentType::Tangents]  = 4 ;
    iComponentLocations [VertexComponentType::Binormals] = 5 ;
    
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
    iComponentLocations [VertexComponentType::Position]  = 0 ;
    iComponentLocations [VertexComponentType::Color]     = 1 ;
    iComponentLocations [VertexComponentType::Normal]    = 2 ;
    iComponentLocations [VertexComponentType::Texture]   = 3 ;
    iComponentLocations [VertexComponentType::Tangents]  = 4 ;
    iComponentLocations [VertexComponentType::Binormals] = 5 ;
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

void HardwareProgram::setLights(const std::vector<Light> &lights) const
{
    for ( int i = 0 ; i < lights.size() ; ++i )
    {
        std::string lightname = std::string("lights[") + std::to_string(i) + "]." ;
        
        if ( lights[i].isEnabled() )
        {
            HardwareProgramVariable lighttype ;
            lighttype.name = lightname + "type" ;
            lighttype.type = HdwProgVarType::Int1 ;
            lighttype.value.i1 = (int) lights[i].getType() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "enabled" ;
            lighttype.type = HdwProgVarType::Int1 ;
            lighttype.value.i1 = 1 ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "position" ;
            lighttype.type = HdwProgVarType::Float3 ;
            lighttype.value.f3 = lights[i].getPosition() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "direction" ;
            lighttype.type = HdwProgVarType::Float3 ;
            lighttype.value.f3 = lights[i].getDirection() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "ambient" ;
            lighttype.type = HdwProgVarType::Float4 ;
            lighttype.value.f4 = lights[i].getAmbient().toFloat4() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "diffuse" ;
            lighttype.type = HdwProgVarType::Float4 ;
            lighttype.value.f4 = lights[i].getDiffuse().toFloat4() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "specular" ;
            lighttype.type = HdwProgVarType::Float4 ;
            lighttype.value.f4 = lights[i].getSpecular().toFloat4() ;
            setVariable(lighttype) ;
            
            lighttype.name = lightname + "shininess" ;
            lighttype.type = HdwProgVarType::Float1 ;
            lighttype.value.f1 = lights[i].getShininess() ;
            setVariable(lighttype) ;
            
            if ( lights[i].getType() != LightType::Directionnal )
            {
                lighttype.name = lightname + "attenuationConstant" ;
                lighttype.type = HdwProgVarType::Float1 ;
                lighttype.value.f1 = lights[i].getAttenuationCst() ;
                setVariable(lighttype) ;
                
                lighttype.name = lightname + "attenuationLinear" ;
                lighttype.type = HdwProgVarType::Float1 ;
                lighttype.value.f1 = lights[i].getAttenuationLinear() ;
                setVariable(lighttype) ;
                
                lighttype.name = lightname + "attenuationQuadratique" ;
                lighttype.type = HdwProgVarType::Float1 ;
                lighttype.value.f1 = lights[i].getAttenuationQuad() ;
                setVariable(lighttype) ;
            }
            
            if ( lights[i].getType() == LightType::Spot )
            {
                lighttype.name = lightname + "angle" ;
                lighttype.type = HdwProgVarType::Float1 ;
                lighttype.value.f1 = lights[i].getAngle() ;
                setVariable(lighttype) ;
                
                lighttype.name = lightname + "exposition" ;
                lighttype.type = HdwProgVarType::Float1 ;
                lighttype.value.f1 = lights[i].getExposition() ;
                setVariable(lighttype) ;
            }
        }
    }
}

GreEndNamespace
