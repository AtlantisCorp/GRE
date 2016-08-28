//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramManager.h
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

#ifndef GRE_HardwareProgramManager_h
#define GRE_HardwareProgramManager_h

#include "Pools.h"
#include "HardwareProgram.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Manages every HardwareProgram and HardwareShader objects.
/// Those Object are owned by this class.
///
/// Creating HardwareShader
/// This is simply done by using HardwareProgramManager::loadShader()
/// function, setting the type of the Shader, the name and the filepath.
/// The manager own the Shader in iShaders property.
///
/// Creating HardwareProgram
/// You should create HardwareProgram using the function
/// HardwareProgramManager::createHardwareProgram(), giving the name of the Program,
/// and if you already loaded it, you can also pass the HardwareShader
/// objects you will use.
///
/// Using a HardwareProgram
/// A HardwareProgram can be used by Pass Objects in order to make
/// multi-pass render. @see Pass.
///
/// Destroying those HardwareProgram and HardwareShader.
/// Normally, everything is destroyed when the Manager is destructed. But,
/// you can use unloadShader() and destroyProgram(), or even clearShaders(),
/// clearPrograms() and clear() functions to do whatever you want.
///
/// Keep in mind that : if you use a HardwareShader. Then you use
/// unloadShader(). Then you try to use it : HardwareShader will lock a
/// null HardwareShaderHolder, then use passthrough functions. The
/// Holder will also try to get a counter information from the pointer,
/// but this one is invalid. So there are good chances that your program
/// will crash.
///
/// Updating the Shaders and Programs.
/// This is done by calling ::onUpdateEvent(). But, this is generally done when
/// receiving an Event of type EventType::Update.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManagerPrivate : public Resource
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManagerPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManagerPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    /// @note You should overwrite this function in a subclass in order to
    /// correctly load Shaders.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShader loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShader getShaderByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareShader getShaderByName(const std::string& name) const;
    
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
    /// @brief Creates a new blank HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new HardwareProgram using the Shaders loaded from
    /// given files.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgramFromFiles ( const std::string& name , const std::string& vertexshaderpath , const std::string& fragmentshaderpath );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram getProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgram getProgram(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the given program from its name.
    //////////////////////////////////////////////////////////////////////
    virtual void destroyProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload every Program.
    //////////////////////////////////////////////////////////////////////
    virtual void clearPrograms();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Manager.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Global Variable for every programs.
    /// This variable will be assigned to every Programs during update.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariable& setGlobalVariableMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Global variable.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariable& setGlobalVariable(const HardwareProgramVariable& var);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unsets a given HardwareVariable Global by its name.
    //////////////////////////////////////////////////////////////////////
    virtual void unsetGlobalVariable(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unset every Global Variables.
    //////////////////////////////////////////////////////////////////////
    virtual void clearGlobals();
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// The UpdateEvent can be emitted when Window objects themself, using
    /// ResourceManager::loop() or Window::update().
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
    
protected:
    
    /// @brief HardwareShader Holder list.
    HardwareShaderHolderList iShaders;
    
    /// @brief HardwareProgram Holder list.
    HardwareProgramHolderList iPrograms;
    
    /// @brief HardwareProgramVariables, for Global Constants.
    HardwareProgramVariables iGlobals;
};

/// @brief SpecializedResourceHolder for HardwareProgramManagerPrivate.
typedef SpecializedResourceHolder<HardwareProgramManagerPrivate> HardwareProgramManagerHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramManagerPrivate list.
typedef SpecializedResourceHolderList<HardwareProgramManagerPrivate> HardwareProgramManagerHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for HardwareProgramManagerPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManager : public SpecializedResourceUser<HardwareProgramManagerPrivate>
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManager(const HardwareProgramManagerPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManager(const HardwareProgramManagerHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManager(const HardwareProgramManager& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    /// @note You should overwrite this function in a subclass in order to
    /// correctly load Shaders.
    //////////////////////////////////////////////////////////////////////
    HardwareShader loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    HardwareShader getShaderByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    const HardwareShader getShaderByName(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload the given HardwareShader, retrieving it from its
    /// name.
    //////////////////////////////////////////////////////////////////////
    void unloadShaderByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unloads every Shader.
    //////////////////////////////////////////////////////////////////////
    void clearShaders();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new blank HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram getProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    const HardwareProgram getProgram(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the given program from its name.
    //////////////////////////////////////////////////////////////////////
    void destroyProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload every Program.
    //////////////////////////////////////////////////////////////////////
    void clearPrograms();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Manager.
    //////////////////////////////////////////////////////////////////////
    void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Global Variable for every programs.
    /// This variable will be assigned to every Programs during update.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariable& setGlobalVariableMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Global variable.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariable& setGlobalVariable(const HardwareProgramVariable& var);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unsets a given HardwareVariable Global by its name.
    //////////////////////////////////////////////////////////////////////
    void unsetGlobalVariable(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unset every Global Variables.
    //////////////////////////////////////////////////////////////////////
    void clearGlobals();
    
    /// @brief The Null HdwProgramManager.
    static HardwareProgramManager Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief A HardwareProgramManager Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManagerLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    HardwareProgramManagerLoader();
    virtual ~HardwareProgramManagerLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::HwdProgramManager.
    //////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareProgramManager.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual Resource* load(Resource::Type type, const std::string& name) const;
};

typedef ResourceLoaderFactory<HardwareProgramManagerLoader> HardwareProgramManagerLoaderFactory;

GreEndNamespace

#endif
