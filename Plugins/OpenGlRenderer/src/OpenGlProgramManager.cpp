//////////////////////////////////////////////////////////////////////
//
//  OpenGlProgramManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/02/2017.
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

#include "OpenGlRenderer.h"

OpenGlProgramManager::OpenGlProgramManager ( const OpenGlRenderer* renderer , const std::string & name )
: Gre::HardwareProgramManager(name) , iRenderer(renderer)
{
    
}

OpenGlProgramManager::~OpenGlProgramManager() noexcept ( false )
{
    
}

void OpenGlProgramManager::_loadDefaultProgram ()
{
    // Default program is a default Vertex/Fragment shader.
    
    std::string vertexpath = "Programs/GLSL/Default.vertex" ;
    std::string fragmentpath = "Programs/GLSL/Default.fragment" ;
    
    Gre::HardwareShaderHolder vertexshader = loadShaderHolder(Gre::ShaderType::Vertex, "DefaultVertex", vertexpath) ;
    Gre::HardwareShaderHolder fragmentshader = loadShaderHolder(Gre::ShaderType::Fragment, "DefaultFragment", fragmentpath) ;
    
    if ( vertexshader.isInvalid() || fragmentshader.isInvalid() ) {
        GreDebug("[WARN] Can't load default shaders.") << Gre::gendl ;
        return ;
    }
    
    Gre::HardwareProgramUser defaultprog = createHardwareProgram("Default",
                                                                 vertexshader,
                                                                 fragmentshader);
    
    if ( defaultprog.isInvalid() ) {
        GreDebug("[WARN] Can't load default program.") << Gre::gendl ;
    }
}

Gre::HardwareShaderHolder OpenGlProgramManager::iCreateHardwareShader(const Gre::ShaderType &stype, const std::string &name, const std::string &source) const
{
    GreAutolock ;
    
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer->getRenderContext()->bind();
        return Gre::HardwareShaderHolder( new OpenGlShader(name, stype, source) ) ;
    }
    
    return Gre::HardwareShaderHolder ( nullptr ) ;
}

Gre::HardwareProgramHolder OpenGlProgramManager::iCreateHardwareProgram(const std::string &name, const Gre::HardwareShaderHolder &vshader, const Gre::HardwareShaderHolder &fshader) const
{
    GreAutolock ;
    
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer->getRenderContext()->bind();
        return Gre::HardwareProgramHolder ( new OpenGlProgram(name, vshader, fshader) ) ;
    }
    
    return Gre::HardwareProgramHolder ( nullptr ) ;
}
