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
    return HardwareShader ( loadShaderHolder(stype, name, filepath) );
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
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Here we want to create a HardwareProgram but without HardwareShader. By this we mean that the
        // HardwareProgram will not be linked untill HardwareProgramPrivate::link() has been called. Property
        // iProgramIsLinked is so false.
        // Notes that not every implementation should support this creation's way.
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, HardwareShaderHolder(nullptr), HardwareShaderHolder(nullptr));
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgram ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return HardwareProgram ( nullptr );
    }
}

HardwareProgram HardwareProgramManagerPrivate::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Check Vertex shader.
        
        if ( vertexShader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Vertex shader is invalid." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Check Fragment shader.
        
        if ( fragmentShader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Fragment shader is invalid." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Holds shaders and create HardwareProgram.
        
        HardwareShaderHolder vholder = vertexShader.lock();
        HardwareShaderHolder fholder = fragmentShader.lock();
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, vholder, fholder);
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgram ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return HardwareProgram ( nullptr );
    }
}

HardwareProgram HardwareProgramManagerPrivate::createHardwareProgramFromFiles(const std::string &name, const std::string &vertexshaderpath, const std::string &fragmentshaderpath)
{
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        // Try to load the first Shader.
        
        HardwareShaderHolder vshader = loadShaderHolder(ShaderType::Vertex, name + "/vertexshader", vertexshaderpath);
        
        if ( vshader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid Vertex Shader path '" << vertexshaderpath << "'." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        vshader->compile();
        
        // Try to load Fragment shader.
        
        HardwareShaderHolder fshader = loadShaderHolder(ShaderType::Fragment, name + "/fragmentshader", fragmentshaderpath);
        
        if ( fshader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid Fragment Shader path '" << fragmentshaderpath << "'." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        fshader->compile();
        
        // Create the HardwareProgram.
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, vshader, fshader);
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << std::endl;
#endif
            return HardwareProgram ( nullptr );
        }
        
        program->finalize();
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgram ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return HardwareProgram ( nullptr );
    }
}

HardwareShaderHolder HardwareProgramManagerPrivate::loadShaderHolder(const Gre::ShaderType &stype, const std::string &name, const std::string &filepath)
{
    if ( !name.empty() )
    {
        auto it = iShaders.find(name);
        
        if ( it != iShaders.end() )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getFilepath() == filepath )
                {
                    return (*it);
                }
                
                else
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "HardwareShader '" << (*it)->getName() << "' has different filepath." << std::endl;
#endif
                    return HardwareShaderHolder ( nullptr );
                }
            }
            else
            {
                iShaders.erase(it);
                return loadShaderHolder(stype, name, filepath);
            }
        }
        
        // Loading the source from file.
        // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
        
        std::ifstream srcstream ( filepath );
        
        if ( !srcstream )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Can't open file '" << filepath << "'." << std::endl;
#endif
            return HardwareShaderHolder ( nullptr );
        }
        
        std::string src;
        
        srcstream.seekg(0, std::ios::end);
        src.reserve(srcstream.tellg());
        srcstream.seekg(0, std::ios::beg);
        
        src.assign( (std::istreambuf_iterator<char>(srcstream)), std::istreambuf_iterator<char>() );
        
        // Create the shader from source.
        
        HardwareShaderHolder shader = iCreateHardwareShader(stype , name , src);
        
        if ( shader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareShader '" << name << "' can't be created." << std::endl;
#endif
            return HardwareShaderHolder ( nullptr );
        }
        
        shader->setFilepath(filepath);
        iShaders.add(shader);
        return shader;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return HardwareShaderHolder ( nullptr );
    }
}

const HardwareProgramHolder HardwareProgramManagerPrivate::getDefaultProgram() const
{
    return getProgram("Default").lock();
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
