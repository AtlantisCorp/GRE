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

HardwareProgram::HardwareProgram(const std::string& name, const HardwareShaderUser& vertexShader, const HardwareShaderUser& fragmentShader)
: Resource(name), iVertexShader(vertexShader), iFragmentShader(fragmentShader)
, iNeedsCompilation(true), iNeedsVarUpdate(false)
{
    
}

HardwareProgram::~HardwareProgram()
{
    
}

void HardwareProgram::use() const
{
    GreAutolock ; iBind () ;
    iBinded = true ;
    
    // Check if we have some variables to upload.
    
    for ( const HardwareProgramVariable & var : iCachedVariables )
    {
        iUploadVariable(var);
    }
    
    iCachedVariables.clear();
}

void HardwareProgram::unuse() const
{
    GreAutolock ; iUnbind() ; iBinded = false ;
}

void HardwareProgram::attachShader(const HardwareShaderUser& hwdShader)
{
    GreAutolock ;
    
    if( isCompiled() )
    {
        // If program is already ready, we can't change it so we must clear it.
        // We store the Current Shader in order not to loose them.
        
        HardwareShaderUser tmpVertex = iVertexShader;
        HardwareShaderUser tmpFragment = iFragmentShader;
        
        reset();
        
        iVertexShader = tmpVertex;
        iFragmentShader = tmpFragment;
        
#ifdef GreIsDebugMode
        GreDebug("[WARN] HardwareProgram '") << getName() << "' was already compiled." << Gre::gendl ;
#endif
    }
    
    HardwareShaderHolder sholder = hwdShader.lock () ;
    
    if ( sholder.isInvalid() )
    {
        // When attaching a null shader, we resest the whole program.
        iVertexShader = HardwareShaderUser ( nullptr ) ;
        iFragmentShader = HardwareShaderUser ( nullptr ) ;
        
#ifdef GreIsDebugMode
        GreDebug("[WARN] HardwareProgram '") << getName() << "' was reset because passed shader is invalid." << Gre::gendl ;
#endif
    }
    
    else
    {
        if(sholder->getType() == ShaderType::Vertex)
        {
            iVertexShader = hwdShader;
            
#ifdef GreIsDebugMode
            GreDebug("[INFO] HardwareProgram '") << getName() << "' was attached with Vertex Shader '"
            << sholder->getName() << Gre::gendl ;
#endif
        }
        
        else if(sholder->getType() == ShaderType::Fragment)
        {
            iFragmentShader = hwdShader;
            
#ifdef GreIsDebugMode
            GreDebug("[INFO] HardwareProgram '") << getName() << "' was attached with Fragment Shader '"
            << sholder->getName() << Gre::gendl ;
#endif
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebug("[WARN] Unrecognized HardwareShader '") << sholder->getName() << "' type." << Gre::gendl ;
#endif
        }
    }
}

void HardwareProgram::finalize()
{
    GreAutolock ; iNeedsCompilation = true;
    
#ifdef GreIsDebugMode
    GreDebug("[WARN] Function called not implemented.") << Gre::gendl ;
#endif
}

bool HardwareProgram::isReady() const
{
    return isCompiled();
}

int HardwareProgram::getAttribLocation(const std::string& name) const
{
#ifdef GreIsDebugMode
    GreDebug("[WARN] Function called not implemented.") << Gre::gendl ;
#endif
    
    return -1;
}

bool HardwareProgram::isCompiled() const
{
    return iNeedsCompilation;
}

void HardwareProgram::reset()
{
    iVertexShader.clear();
    iFragmentShader.clear();
    iNeedsCompilation = true;
    iNeedsVarUpdate = false;
}

bool HardwareProgram::hasOutputLocation(uint16_t location) const
{
#ifdef GreIsDebugMode
    GreDebug("[WARN] Function called not implemented.") << Gre::gendl ;
#endif
    return false;
}

void HardwareProgram::bindTextureUnit(int) const
{
#ifdef GreIsDebugMode
    GreDebug("[WARN] Function called not implemented.") << Gre::gendl ;
#endif
}

void HardwareProgram::setCompiled(bool flag)
{
    iNeedsCompilation = flag;
}

void HardwareProgram::bindAttribsVertex(const VertexDescriptor &descriptor, const char* data) const
{
    if ( iNeedsCompilation )
    {
#ifdef GreIsDebugMode
        GreDebug("[ERRO] Trying to bind Attributes Vertex Descriptor to HardwareProgram '") << getName()
        << "' but this one is not compiled." << Gre::gendl ;
#endif
        
        GreThrow( "HardwareProgram:notCompiled" );
        return;
    }
    
    if ( descriptor.getSize() )
    {
        VertexComponents components = descriptor.getComponents();
        
        for ( auto comp : components )
        {
            if ( VertexComponentTypeGetSize(comp) )
            {
                int index = getAttribLocation(comp);
                
                if ( index >= 0 )
                {
                    setAttribData(index, descriptor.getStride(comp), data + descriptor.getComponentLocation(comp) );
                }
                
                else
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Attribute '" << VertexComponentTypeToString(comp) << "' not found in HardwareProgram '" << getName() << "'." << Gre::gendl;
#endif
                }
            }
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Descriptor given is empty." << Gre::gendl;
#endif
    }
}

int HardwareProgram::getAttribLocation(const VertexComponentType& component) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
    return -1;
}

void HardwareProgram::setAttribData(int index, size_t stride, const char *data) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
}

void HardwareProgram::bindAttributeLocation(const Gre::VertexComponentType &component, int index)
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
}

int HardwareProgram::getUniformLocation(const std::string &name) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
    return -1;
}

void HardwareProgram::setVariable(const Gre::HardwareProgramVariable &var) const
{
    if ( iBinded )
    {
        // We are binded , so we can upload the variable to the program.
        iUploadVariable ( var ) ;
    }
    
    else
    {
        // Cache this Variable for later uploading.
        iCachedVariables.add(var);
    }
}

GreEndNamespace
