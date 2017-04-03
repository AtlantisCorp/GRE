//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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

// -----------------------------------------------------------------------------

HardwareProgramManagerInternalCreator::HardwareProgramManagerInternalCreator ()
{

}

HardwareProgramManagerInternalCreator::~HardwareProgramManagerInternalCreator ()
{

}

// -----------------------------------------------------------------------------

HardwareProgramManager::HardwareProgramManager(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{
    iInternalCreator = nullptr ;
}

HardwareProgramManager::~HardwareProgramManager() noexcept ( false )
{

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

HardwareProgramHolder HardwareProgramManager::getProgram(const std::string &name)
{
    GreAutolock ;

    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgramHolder (nullptr);
    else
        return (*it);
}

const HardwareProgramHolder HardwareProgramManager::getProgram(const std::string &name) const
{
    GreAutolock ;

    auto it = iPrograms.find(name);
    if(it == iPrograms.end())
        return HardwareProgramHolder (nullptr);
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

void HardwareProgramManager::unloadProgram(const HardwareProgramHolder &program)
{
    GreAutolock ;

    if ( !program.isInvalid() )
    {
        auto it = iPrograms.find(program->getIdentifier()) ;

        if ( it != iPrograms.end() ) {

            removeListener ( EventProceederUser(program) ) ;
            iPrograms.erase(it) ;

        }
    }
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
    iInternalCreator = nullptr ;
}

void HardwareProgramManager::setInternalCreator(Gre::HardwareProgramManagerInternalCreator *creator)
{
    GreAutolock ; iInternalCreator = creator ;
}

const HardwareProgramManagerInternalCreator* HardwareProgramManager::getInternalCreator() const
{
    GreAutolock ; return iInternalCreator ;
}

HardwareShaderHolderList HardwareProgramManager::loadShaderTables ( const ShaderPathTableList & tables )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Iterates over the tables and load it one by one.

    HardwareShaderHolderList shaders ;

    for ( auto table : tables.tables )
    {
        HardwareShaderHolder shader = loadShaderTable ( table ) ;
        if ( !shader.isInvalid() ) shaders.add(shader) ;
    }

    return shaders ;
}

HardwareShaderHolder HardwareProgramManager::loadShaderTable ( const ShaderPathTable & table )
{
    GreAutolock ;

    if ( !iInternalCreator )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Iterates over the pathes to get a valid path for our compiler.

    for ( auto it : table.pathes )
    {
        if ( it.first == iInternalCreator -> getCompiler() )
        return loadShaderFromFile ( table.type , it.second ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // If we gets here this means we had nothing.

    return HardwareShaderHolder ( nullptr ) ;
}

HardwareShaderHolder HardwareProgramManager::loadShaderFromFile ( const ShaderType & type , const std::string & path )
{
    GreAutolock ;

    if ( !iInternalCreator )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Tries to get the file's source directly from working directory.

    std::string src = Platform::GetFileSource ( path ) ;

    if ( src.empty() )
    return loadShaderFromBundled ( type , path ) ;

    //////////////////////////////////////////////////////////////////////
    // If we get here this means we have the correct path.

    return loadShaderFromSource ( type , src ) ;
}

HardwareShaderHolder HardwareProgramManager::loadShaderFromBundled ( const ShaderType & type , const std::string & path )
{
    GreAutolock ;

    if ( !iInternalCreator )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Gets every bundles and tries to find the file in one of the bundles.
    // Notes the path is always appended by the compiler name , as this is a
    // bundle requirement.

    std::string filepath = iInternalCreator -> getCompiler () + Platform::GetSeparator() + path ;
    filepath = ResourceManager::Get() -> findBundledFile ( ResourceType::Program , filepath ) ;

    if ( filepath.empty() )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // If we gets here , this means the file has been found and we have a
    // correct path. Gets the source and load it.

    std::string src = Platform::GetFileSource ( filepath ) ;

    if ( src.empty() )
    return HardwareShaderHolder ( nullptr ) ;

    return loadShaderFromSource ( type , src ) ;
}

HardwareShaderHolder HardwareProgramManager::loadShaderFromSource ( const ShaderType & type , const std::string & source )
{
    GreAutolock ;

    if ( !iInternalCreator )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Uses the internal creator to create the shader.

    HardwareShaderHolder shader = iInternalCreator -> loadShader ( type , "" , source ) ;

    if ( shader.isInvalid () )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Registers and returns it.

    return loadShaderFromHolder ( shader ) ;
}

HardwareShaderHolder HardwareProgramManager::loadShaderFromHolder ( const HardwareShaderHolder & shader )
{
    GreAutolock ;

    if ( shader.isInvalid () )
    return HardwareShaderHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Check this shader is not already loaded.

    if ( iShaders.find(shader->getIdentifier()) == iShaders.end() )
    iShaders.add ( shader ) ;

    return shader ;
}

HardwareProgramHolder HardwareProgramManager::loadProgram ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check if program is not already loaded.

    HardwareProgramHolder chk = getProgram ( name ) ;

    if ( !chk.isInvalid() )
    return chk ;

    //////////////////////////////////////////////////////////////////////
    // We gets here , load it using the creator and registers it.

    if ( !iInternalCreator )
    return HardwareProgramHolder ( nullptr ) ;

    chk = iInternalCreator -> loadProgram ( name , { } ) ;

    if ( chk.isInvalid() )
    return HardwareProgramHolder ( nullptr ) ;

    iPrograms.add ( chk ) ;
    return chk ;
}

GreEndNamespace
