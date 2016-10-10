//////////////////////////////////////////////////////////////////////
//
//  DarwinGlHardwareProgramManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/10/2016.
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

#include "DarwinGlHardwareProgramManager.h"
#include "DarwinGlHardwareShader.h"
#include "DarwinGlHardwareProgram.h"

namespace DarwinGl
{
    // Here , we have the textes for Default OpenGl Fragment and Vertex Shader .
    
    const char * GlDefaultVertexShaderPath = "Resources/Shaders/GLSL/DefaultVertex.vert" ;
    const char * GlDefaultFragmentShaderPath = "Resources/Shaders/GLSL/DefaultFragment.frag" ;
    const char * GlDefaultMultitexturingFragmentPath = "Resources/Shaders/GLSL/DefaultMultitexturing2D.frag" ;
    
    DarwinGlHardwareProgramManager::DarwinGlHardwareProgramManager ( const std::string& name )
    : Gre::HardwareProgramManagerPrivate(name)
    {
        // We will try here to load default shaders.
        
        HardwareShader GlDefaultVertexShader = loadShader(ShaderType::Vertex, "DefaultVertex", GlDefaultVertexShaderPath);
        
        if ( GlDefaultVertexShader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No 'GlDefaultVertexShader' could be created." << std::endl;
#endif
        }
        else
        {
            GlDefaultVertexShader.compile();
            
            HardwareShader GlDefaultFragmentShader = loadShader(ShaderType::Fragment, "DefaultFragment", GlDefaultFragmentShaderPath);
            
            if ( GlDefaultFragmentShader.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "No 'GlDefaultFragmentShader' could be created." << std::endl;
#endif
            }
            else
            {
                GlDefaultFragmentShader.compile();
                
                if ( !GlDefaultVertexShader.isInvalid() && !GlDefaultFragmentShader.isInvalid() )
                {
                    HardwareProgram GlDefaultProgram = createHardwareProgram("Default", GlDefaultVertexShader, GlDefaultFragmentShader);
                    
                    if ( GlDefaultProgram.isInvalid() )
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "No 'GlDefaultProgram' could be created." << std::endl;
#endif
                    }
                    
                    else
                    {
                        GlDefaultProgram.finalize();
#ifdef GreIsDebugMode
                        GreDebugPretty() << "'GlDefaultProgram' correctly created." << std::endl;
#endif
                    }
                }
            }
        }
        
        // Try to load other HardwareProgram .
        
        HardwareProgram GlDefaultMultitexturing = createHardwareProgramFromFiles("DefaultMultitexturing2D",
                                                                                 GlDefaultVertexShaderPath,
                                                                                 GlDefaultMultitexturingFragmentPath);
    }
    
    DarwinGlHardwareProgramManager::~DarwinGlHardwareProgramManager()
    {
        
    }
    
    HardwareShaderHolder DarwinGlHardwareProgramManager::iCreateHardwareShader(const Gre::ShaderType &stype, const std::string &name, const std::string &source) const
    {
        return HardwareShaderHolder ( new DarwinGlHardwareShader(name, stype, source) );
    }
    
    HardwareProgramHolder DarwinGlHardwareProgramManager::iCreateHardwareProgram(const std::string &name, const HardwareShaderHolder &vshader, const HardwareShaderHolder &fshader) const
    {
        return HardwareProgramHolder ( new DarwinGlHardwareProgram(name, vshader, fshader) );
    }
}


