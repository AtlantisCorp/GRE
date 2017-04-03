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

OpenGlProgramManagerCreator::OpenGlProgramManagerCreator ( const OpenGlRenderer* renderer )
: Gre::HardwareProgramManagerInternalCreator()
, iRenderer(renderer)
{

}

OpenGlProgramManagerCreator::~OpenGlProgramManagerCreator()
{

}

Gre::HardwareShaderHolder OpenGlProgramManagerCreator::loadShader (const Gre::ShaderType & type ,
                                                                   const std::string & name ,
                                                                   const std::string & source) const
{
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer -> getRenderContext () -> bind () ;
        return Gre::HardwareShaderHolder ( new OpenGlShader(name, type, source) ) ;
    }

    return Gre::HardwareShaderHolder ( nullptr ) ;
}

Gre::HardwareProgramHolder OpenGlProgramManagerCreator::loadProgram (const std::string & name ,
                                                                     const Gre::HardwareShaderHolderList & shaders) const
{
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer -> getRenderContext() -> bind () ;

        Gre::HardwareProgramHolder program ( new OpenGlProgram(name) ) ;

        if ( program.isInvalid() )
        return Gre::HardwareProgramHolder ( nullptr ) ;

        //////////////////////////////////////////////////////////////////////
        // The user may or may not want to load shaders directly here , so let's
        // do it only if the shaders' list is not empty.

        if ( shaders.empty() )
        return program ;

        program -> attachShaders ( shaders ) ;
        program -> finalize () ;

        if ( program -> isFinalized() ) {
            return program ;
        }

#ifdef GreIsDebugMode
        GreDebug ( "[WARN] OpenGlProgram '" ) << name << "' not loaded." << Gre::gendl ;
#endif
    }

    return Gre::HardwareProgramHolder ( nullptr ) ;
}

const std::string OpenGlProgramManagerCreator::getCompiler () const
{
    return "GLSL" ;
}
