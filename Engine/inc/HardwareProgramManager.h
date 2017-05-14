//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramManager.h
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

#ifndef GRE_HardwareProgramManager_h
#define GRE_HardwareProgramManager_h

#include "Pools.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include "HardwareProgram.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Interface to implement the HardwareProgram Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManagerInternalCreator
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManagerInternalCreator () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManagerInternalCreator () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Shader file with name , type and source.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShader (const ShaderType & type ,
                                             const std::string & name ,
                                             const std::string & source) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Program with every shaders that may be part of it.
    /// Notes that at least a Vertex Shader and a Fragment Shader should
    /// be present.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramHolder loadProgram (const std::string & name ,
                                               const HardwareShaderHolderList & shaders) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Compiler String the bundle will use to get a valid
    /// path to the shader file. For example , OpenGl 'GLSL' Compiler has
    /// String Value "GLSL" . Shaders files will so be in folder 'GLSL/'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string getCompiler () const = 0 ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages every HardwareProgram and HardwareShader objects.
///
/// ### Creating Compiler's API Specific Objects
///
/// As there isn't one GPU's program compiler , anyone can create a
/// HardwareProgramManagerInternalCreator object and overwrite the already
/// made one in order to implement a Compiler - specific manager.
///
/// ### Management of HardwareProgram's
///
/// They do not rely on any names conventions. They only analyses the
/// shader code to get valid data. The link between actual Engine data
/// and shader input is done by the Technique object.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManager : public Resource
{
public:

    POOLED(Pools::Manager)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManager(const std::string& name = "DefaultProgramManager");

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManager() noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder getShaderByName(const std::string& name);

    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareShaderHolder getShaderByName(const std::string& name) const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unload the given HardwareShader, retrieving it from its
    /// name.
    //////////////////////////////////////////////////////////////////////
    virtual void unloadShaderByName(const std::string& name);

    //////////////////////////////////////////////////////////////////////
    /// @brief Unloads every Shader.
    //////////////////////////////////////////////////////////////////////
    virtual void clearShaders();

    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramHolder getProgram(const std::string& name);

    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramHolder getProgram(const std::string& name) const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the given program from its name.
    //////////////////////////////////////////////////////////////////////
    virtual void destroyProgram(const std::string& name);

    //////////////////////////////////////////////////////////////////////
    /// @brief Unloads the given program holder from this manager.
    //////////////////////////////////////////////////////////////////////
    virtual void unloadProgram ( const HardwareProgramHolder & program ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unload every Program.
    //////////////////////////////////////////////////////////////////////
    virtual void clearPrograms();

    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Manager.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setInternalCreator ( HardwareProgramManagerInternalCreator * creator ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramManagerInternalCreator * getInternalCreator () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads shaders from the given ShaderTableList.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolderList loadShaderTables ( const ShaderPathTableList & tables ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the given shader table.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderTable ( const ShaderPathTable & table ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a shader from the given file. It is assumed this file
    /// may be in one of the bundles , or the path may be a direct path from
    /// the working directory.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderFromFile ( const ShaderType & type , const std::string & path ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a shader that assumes the path is in one of the bundles.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderFromBundled ( const ShaderType & type , const std::string & path ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a shader directly from its source.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderFromSource ( const ShaderType & type , const std::string & source ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a shader from its holder , if it is not already
    /// loaded.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderFromHolder ( const HardwareShaderHolder & shader ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a program , or returns it if it was already loaded.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramHolder loadProgram ( const std::string & name ) ;

protected:

    /// @brief HardwareShader Holder list.
    HardwareShaderHolderList iShaders;

    /// @brief HardwareProgram Holder list.
    HardwareProgramHolderList iPrograms;

    /// @brief Internal Creator to implement shader/program creation.
    HardwareProgramManagerInternalCreator * iInternalCreator ;
};

/// @brief Holder for HardwareProgramManagerPrivate.
typedef Holder<HardwareProgramManager> HardwareProgramManagerHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramManagerPrivate list.
typedef SpecializedResourceHolderList<HardwareProgramManager> HardwareProgramManagerHolderList;

GreEndNamespace

#endif
