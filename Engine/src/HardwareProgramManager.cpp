//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 10/01/2016.
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

#include "HardwareProgramManager.h"
#include "ResourceManager.h"

GreBeginNamespace

HardwareProgramManagerPrivate::HardwareProgramManagerPrivate(const std::string& name)
: Resource(name)
{
    
}

HardwareProgramManagerPrivate::~HardwareProgramManagerPrivate()
{
    
}

HardwareShader HardwareProgramManagerPrivate::loadShader(const ShaderType& stype, const std::string &name, const std::string &filepath)
{
    return HardwareShader::Null;
}

HardwareShader HardwareProgramManagerPrivate::getShaderByName(const std::string &name)
{
    auto it = iShaders.find(name);
    if(it == iShaders.end())
    {
        return HardwareShader::Null;
    }
    
    else
    {
        return *it;
    }
}

const HardwareShader HardwareProgramManagerPrivate::getShaderByName(const std::string &name) const
{
    auto it = iShaders.find(name);
    if(it == iShaders.end())
    {
        return HardwareShader::Null;
    }
    
    else
    {
        return *it;
    }
}

void HardwareProgramManagerPrivate::unloadShaderByName(const std::string &name)
{
    auto it = iShaders.find(name);
    if(it == iShaders.end())
        return;
    
    iShaders.erase(it);
}

void HardwareProgramManagerPrivate::clearShaders()
{
    iShaders.clear();
}

HardwareProgram HardwareProgramManagerPrivate::createHardwareProgram(const std::string &name)
{
    return HardwareProgram::Null;
}

HardwareProgram HardwareProgramManagerPrivate::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    return HardwareProgram::Null;
}

HardwareProgram HardwareProgramManagerPrivate::getProgram(const std::string &name)
{
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgram::Null;
    else
        return (*it);
}

const HardwareProgram HardwareProgramManagerPrivate::getProgram(const std::string &name) const
{
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgram::Null;
    else
        return (*it);
}

void HardwareProgramManagerPrivate::destroyProgram(const std::string &name)
{
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return;
    
    iPrograms.erase(it);
}

void HardwareProgramManagerPrivate::clearPrograms()
{
    iPrograms.clear();
}

void HardwareProgramManagerPrivate::clear()
{
    clearShaders();
    clearPrograms();
    clearGlobals();
}

HardwareProgramVariable& HardwareProgramManagerPrivate::setGlobalVariableMat4(const std::string &name, const Matrix4 &mat4)
{
    HardwareProgramVariable nVar;
    nVar.name = name;
    nVar.type = HdwProgVarType::Mat4;
    nVar.value.mat4 = mat4;
    iGlobals.add(nVar);
    return iGlobals.get(name);
}

HardwareProgramVariable& HardwareProgramManagerPrivate::setGlobalVariable(const HardwareProgramVariable& var)
{
    iGlobals.add(var);
    return iGlobals.get(var.name);
}

void HardwareProgramManagerPrivate::unsetGlobalVariable(const std::string &name)
{
    iGlobals.remove(name);
}

void HardwareProgramManagerPrivate::clearGlobals()
{
    iGlobals.clear();
}

void HardwareProgramManagerPrivate::onUpdateEvent(const Gre::UpdateEvent &e)
{
    for(auto prog : iPrograms)
    {
        for(auto var : iGlobals)
        {
            prog->setUniform(var);
        }
    }
}

// ---------------------------------------------------------------------------------------------------

HardwareProgramManager::HardwareProgramManager(const HardwareProgramManagerPrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser<Gre::HardwareProgramManagerPrivate>(pointer)
{
    
}

HardwareProgramManager::HardwareProgramManager(const HardwareProgramManagerHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser<Gre::HardwareProgramManagerPrivate>(holder)
{
    
}

HardwareProgramManager::HardwareProgramManager(const HardwareProgramManager& user)
: ResourceUser(user)
, SpecializedResourceUser<Gre::HardwareProgramManagerPrivate>(user)
{
    
}

HardwareProgramManager::~HardwareProgramManager()
{
    
}

HardwareShader HardwareProgramManager::loadShader(const Gre::ShaderType &stype, const std::string &name, const std::string &filepath)
{
    auto ptr = lock();
    if(ptr)
        return ptr->loadShader(stype, name, filepath);
    return HardwareShader::Null;
}

HardwareShader HardwareProgramManager::getShaderByName(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        return ptr->getShaderByName(name);
    return HardwareShader::Null;
}

const HardwareShader HardwareProgramManager::getShaderByName(const std::string &name) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getShaderByName(name);
    return HardwareShader::Null;
}

void HardwareProgramManager::unloadShaderByName(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        ptr->unloadShaderByName(name);
}

void HardwareProgramManager::clearShaders()
{
    auto ptr = lock();
    if(ptr)
        ptr->clearShaders();
}

HardwareProgram HardwareProgramManager::createHardwareProgram(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        return ptr->createHardwareProgram(name);
    return HardwareProgram::Null;
}

HardwareProgram HardwareProgramManager::createHardwareProgram(const std::string &name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    auto ptr = lock();
    if(ptr)
        return ptr->createHardwareProgram(name, vertexShader, fragmentShader);
    return HardwareProgram::Null;
}

HardwareProgram HardwareProgramManager::getProgram(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        return ptr->getProgram(name);
    return HardwareProgram::Null;
}

const HardwareProgram HardwareProgramManager::getProgram(const std::string &name) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getProgram(name);
    return HardwareProgram::Null;
}

void HardwareProgramManager::destroyProgram(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        ptr->destroyProgram(name);
}

void HardwareProgramManager::clearPrograms()
{
    auto ptr = lock();
    if(ptr)
        ptr->clearPrograms();
}

void HardwareProgramManager::clear()
{
    auto ptr = lock();
    if(ptr)
        ptr->clear();
}

HardwareProgramVariable& HardwareProgramManager::setGlobalVariableMat4(const std::string &name, const Matrix4 &mat4)
{
    auto ptr = lock();
    if(ptr)
        return ptr->setGlobalVariableMat4(name, mat4);
    return HardwareProgramVariable::Null;
}

HardwareProgramVariable& HardwareProgramManager::setGlobalVariable(const Gre::HardwareProgramVariable &var)
{
    auto ptr = lock();
    if(ptr)
        return ptr->setGlobalVariable(var);
    return HardwareProgramVariable::Null;
}

void HardwareProgramManager::unsetGlobalVariable(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        ptr->unsetGlobalVariable(name);
}

void HardwareProgramManager::clearGlobals()
{
    auto ptr = lock();
    if(ptr)
        ptr->clearGlobals();
}

HardwareProgramManager HardwareProgramManager::Null = HardwareProgramManager(nullptr);

// ---------------------------------------------------------------------------------------------------

HardwareProgramManagerLoader::HardwareProgramManagerLoader()
{
    
}

HardwareProgramManagerLoader::~HardwareProgramManagerLoader()
{
    
}

bool HardwareProgramManagerLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::HwdProgManager;
}

Resource* HardwareProgramManagerLoader::load(Resource::Type type, const std::string &name) const
{
    return nullptr;
}

GreEndNamespace
