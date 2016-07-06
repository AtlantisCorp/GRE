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

GreBeginNamespace

HardwareProgramPrivate::HardwareProgramPrivate(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader)
: Resource(name), iVertexShader(vertexShader), iFragmentShader(fragmentShader), iVariables(), iNeedsCompilation(true), iNeedsVarUpdate(false)
{
    
}

HardwareProgramPrivate::~HardwareProgramPrivate()
{
    
}

void HardwareProgramPrivate::bind() const
{
#ifdef GreIsDebugMode
    if(!isCompiled())
    {
        GreDebugPretty() << "Can't bind HardwareProgram '" << getName() << "' because not compiled." << std::endl;
    }
#endif
}

void HardwareProgramPrivate::unbind() const
{
    
}

void HardwareProgramPrivate::attachShader(const HardwareShader& hwdShader)
{
    if(isCompiled())
    {
        // If program is already ready, we can't change it so we must clear it.
        // We store the Current Shader in order not to loose them.
        
        HardwareShader tmpVertex = iVertexShader;
        HardwareShader tmpFragment = iFragmentShader;
        
        reset();
        
        iVertexShader = tmpVertex;
        iFragmentShader = tmpFragment;
    }
    
    if(hwdShader.getType() == ShaderType::Vertex)
    {
        iVertexShader = hwdShader;
    }
    
    if(hwdShader.getType() == ShaderType::Fragment)
    {
        iFragmentShader = hwdShader;
    }
}

void HardwareProgramPrivate::finalize()
{
    iNeedsCompilation = true;
}

bool HardwareProgramPrivate::isReady() const
{
    return isCompiled();
}

HardwareProgramVariables HardwareProgramPrivate::getVariables()
{
    return iVariables;
}

void HardwareProgramPrivate::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    HardwareProgramVariable var;
    var.name = name;
    var.type = HdwProgVarType::Mat4;
    var.value.mat4 = mat4;
    iVariables.add(var);
    iNeedsVarUpdate = true;
}

void HardwareProgramPrivate::setUniform(const Gre::HardwareProgramVariable &var)
{
    iVariables.add(var);
    iNeedsVarUpdate = true;
}

void HardwareProgramPrivate::unsetUniform(const std::string &name)
{
    iVariables.remove(name);
}

int HardwareProgramPrivate::getAttribLocation(const std::string& name) const
{
    return -1;
}

bool HardwareProgramPrivate::isCompiled() const
{
    return iNeedsCompilation;
}

void HardwareProgramPrivate::reset()
{
    iVertexShader.reset();
    iFragmentShader.reset();
    iVariables.clear();
    iNeedsCompilation = true;
    iNeedsVarUpdate = false;
}

bool HardwareProgramPrivate::hasOutputLocation(uint16_t location) const
{
    return false;
}

void HardwareProgramPrivate::setCompiled(bool flag)
{
    iNeedsCompilation = flag;
}

// ---------------------------------------------------------------------------------------------------

HardwareProgram::HardwareProgram(const HardwareProgramPrivate* pointer)
: SpecializedResourceUser<HardwareProgramPrivate>(pointer)
{
    
}

HardwareProgram::HardwareProgram(const HardwareProgramHolder& holder)
: SpecializedResourceUser<HardwareProgramPrivate>(holder)
{
    
}

HardwareProgram::HardwareProgram(const HardwareProgram& user)
: SpecializedResourceUser<HardwareProgramPrivate>(user)
{
    
}

HardwareProgram::~HardwareProgram()
{
    
}

void HardwareProgram::bind() const
{
    auto ptr = lock();
    
    if(ptr)
        ptr->bind();
}

void HardwareProgram::unbind() const
{
    auto ptr = lock();
    
    if(ptr)
        ptr->unbind();
}

void HardwareProgram::finalize()
{
    auto ptr = lock();
    if(ptr)
        ptr->finalize();
}

bool HardwareProgram::isReady() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isReady();
    return false;
}

HardwareProgramVariables HardwareProgram::getVariables()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getVariables();
    return HardwareProgramVariables::Empty;
}

void HardwareProgram::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    auto ptr = lock();
    if(ptr)
        ptr->setUniformMat4(name, mat4);
}

void HardwareProgram::setUniform(const Gre::HardwareProgramVariable &var)
{
    auto ptr = lock();
    if(ptr)
        ptr->setUniform(var);
}

void HardwareProgram::unsetUniform(const std::string& name)
{
    auto ptr = lock();
    if(ptr)
        ptr->unsetUniform(name);
}

int HardwareProgram::getAttribLocation(const std::string& name) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getAttribLocation(name);
    return -1;
}

bool HardwareProgram::isCompiled() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->isCompiled();
    }
    
    return false;
}

void HardwareProgram::reset()
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->reset();
    }
}

bool HardwareProgram::hasOutputLocation(uint16_t location) const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasOutputLocation(location);
    return false;
}

HardwareProgram HardwareProgram::Null = HardwareProgram(nullptr);

GreEndNamespace
