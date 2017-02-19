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

HardwareProgramManager::HardwareProgramManager(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{
    
}

HardwareProgramManager::~HardwareProgramManager() noexcept ( false )
{
    
}

HardwareShaderUser HardwareProgramManager::loadShader(const ShaderType& stype, const std::string &name, const std::string &filepath)
{
    GreAutolock ;
    return HardwareShaderUser ( loadShaderHolder(stype, name, filepath) );
}

HardwareShaderUser HardwareProgramManager::getShaderByName(const std::string &name)
{
    GreAutolock ;
    
    auto it = iShaders.find(name);
    if(it == iShaders.end())
    {
        return HardwareShaderUser (nullptr);
    }
    
    else
    {
        return *it;
    }
}

const HardwareShaderUser HardwareProgramManager::getShaderByName(const std::string &name) const
{
    GreAutolock ;
    
    auto it = iShaders.find(name);
    if(it == iShaders.end())
    {
        return HardwareShaderUser (nullptr);
    }
    
    else
    {
        return *it;
    }
}

void HardwareProgramManager::unloadShaderByName(const std::string &name)
{
    GreAutolock ;
    
    auto it = iShaders.find(name);
    if(it == iShaders.end())
        return;
    
    iShaders.erase(it);
}

void HardwareProgramManager::clearShaders()
{
    GreAutolock ;
    iShaders.clear();
}

HardwareProgramUser HardwareProgramManager::createHardwareProgram(const std::string &name)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Here we want to create a HardwareProgram but without HardwareShader. By this we mean that the
        // HardwareProgram will not be linked untill HardwareProgramPrivate::link() has been called. Property
        // iProgramIsLinked is so false.
        // Notes that not every implementation should support this creation's way.
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, HardwareShaderHolder(nullptr), HardwareShaderHolder(nullptr));
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgramUser ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << Gre::gendl;
#endif
        return HardwareProgramUser ( nullptr );
    }
}

HardwareProgramUser HardwareProgramManager::createHardwareProgram(const std::string& name, const HardwareShaderUser &vertexShader, const HardwareShaderUser &fragmentShader)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Check Vertex shader.
        
        if ( vertexShader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Vertex shader is invalid." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Check Fragment shader.
        
        if ( fragmentShader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Fragment shader is invalid." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Holds shaders and create HardwareProgram.
        
        HardwareShaderHolder vholder = vertexShader.lock();
        HardwareShaderHolder fholder = fragmentShader.lock();
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, vholder, fholder);
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgramUser ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << Gre::gendl;
#endif
        return HardwareProgramUser ( nullptr );
    }
}

HardwareProgramUser HardwareProgramManager::createHardwareProgramFromFiles(const std::string &name, const std::string &vertexshaderpath, const std::string &fragmentshaderpath)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        // Check if the HardwareProgram currently exists.
        
        auto it = iPrograms.find(name);
        
        if ( it != iPrograms.end() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "HardwareProgram '" << name << "' already loaded." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Try to load the first Shader.
        
        HardwareShaderHolder vshader = loadShaderHolder(ShaderType::Vertex, name + "/vertexshader", vertexshaderpath);
        
        if ( vshader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid Vertex Shader path '" << vertexshaderpath << "'." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Try to load Fragment shader.
        
        HardwareShaderHolder fshader = loadShaderHolder(ShaderType::Fragment, name + "/fragmentshader", fragmentshaderpath);
        
        if ( fshader.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid Fragment Shader path '" << fragmentshaderpath << "'." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Create the HardwareProgram.
        
        HardwareProgramHolder program = iCreateHardwareProgram(name, vshader, fshader);
        
        if ( program.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Invalid HardwareProgram '" << name << "' creation." << Gre::gendl;
#endif
            return HardwareProgramUser ( nullptr );
        }
        
        // Load the HardwareProgram to the database.
        
        iPrograms.add(program);
        return HardwareProgramUser ( program );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << Gre::gendl;
#endif
        return HardwareProgramUser ( nullptr );
    }
}

HardwareShaderHolder HardwareProgramManager::loadShaderHolder(const Gre::ShaderType &stype, const std::string &name, const std::string &filepath)
{
    GreAutolock ;
    
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
                    GreDebugPretty() << "HardwareShader '" << (*it)->getName() << "' has different filepath." << Gre::gendl;
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
            GreDebugPretty() << "Can't open file '" << filepath << "'." << Gre::gendl;
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
            GreDebugPretty() << "HardwareShader '" << name << "' can't be created." << Gre::gendl;
#endif
            return HardwareShaderHolder ( nullptr );
        }
        
        iShaders.add(shader);
        return shader;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << Gre::gendl;
#endif
        return HardwareShaderHolder ( nullptr );
    }
}

const HardwareProgramHolder HardwareProgramManager::getDefaultProgram() const
{
    GreAutolock ;
    return getProgram("Default").lock();
}

HardwareProgramUser HardwareProgramManager::getProgram(const std::string &name)
{
    GreAutolock ;
    
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgramUser (nullptr);
    else
        return (*it);
}

const HardwareProgramUser HardwareProgramManager::getProgram(const std::string &name) const
{
    GreAutolock ;
    
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgramUser (nullptr);
    else
        return (*it);
}

void HardwareProgramManager::destroyProgram(const std::string &name)
{
    GreAutolock ;
    
    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return;
    
    iPrograms.erase(it);
}

void HardwareProgramManager::clearPrograms()
{
    GreAutolock ;
    iPrograms.clear();
}

void HardwareProgramManager::clear()
{
    GreAutolock ;
    
    clearShaders();
    clearPrograms();
    clearGlobals();
}

HardwareProgramVariable& HardwareProgramManager::setGlobalVariableMat4(const std::string &name, const Matrix4 &mat4)
{
    GreAutolock ;
    
    HardwareProgramVariable nVar;
    nVar.name = name;
    nVar.type = HdwProgVarType::Matrix4;
    nVar.value.m4 = mat4;
    iGlobals.add(nVar);
    return iGlobals.get(name);
}

HardwareProgramVariable& HardwareProgramManager::setGlobalVariable(const HardwareProgramVariable& var)
{
    GreAutolock ;
    iGlobals.add(var);
    return iGlobals.get(var.name);
}

void HardwareProgramManager::unsetGlobalVariable(const std::string &name)
{
    GreAutolock ;
    iGlobals.remove(name);
}

void HardwareProgramManager::clearGlobals()
{
    GreAutolock ;
    iGlobals.clear();
}

void HardwareProgramManager::onUpdateEvent(const Gre::UpdateEvent &e)
{
    GreAutolock ;
    
    for(auto prog : iPrograms)
    {
        for(auto var : iGlobals)
        {
            prog->setVariable(var);
        }
    }
}

// ---------------------------------------------------------------------------------------------------

HardwareProgramManagerLoader::HardwareProgramManagerLoader()
{
    
}

HardwareProgramManagerLoader::~HardwareProgramManagerLoader()
{
    
}

HardwareProgramManagerHolder HardwareProgramManagerLoader::load( const std::string &name ) const
{
    return HardwareProgramManagerHolder(nullptr) ;
}

GreEndNamespace
