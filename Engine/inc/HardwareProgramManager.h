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
#include "Resource.h"
#include "ResourceLoader.h"
#include "HardwareProgram.h"

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
class DLL_PUBLIC HardwareProgramManager : public Resource
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManager(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManager() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    /// @note You should overwrite this function in a subclass in order to
    /// correctly load Shaders.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderUser loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderUser getShaderByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieves a HardwareShader by its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareShaderUser getShaderByName(const std::string& name) const;
    
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
    virtual HardwareProgramUser createHardwareProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramUser createHardwareProgram(const std::string& name, const HardwareShaderUser &vertexShader, const HardwareShaderUser &fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new HardwareProgram using the Shaders loaded from
    /// given files.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramUser createHardwareProgramFromFiles ( const std::string& name , const std::string& vertexshaderpath , const std::string& fragmentshaderpath );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareShader from a file and returns a holder.
    /// If 'name' is already registered, it returns this Shader only if
    /// 'filepath' is corresponding.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder loadShaderHolder ( const ShaderType& stype , const std::string& name , const std::string& filepath );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Default Hardware Program , if has one .
    /// This HardwareProgram is loaded under the name 'Default' .
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramHolder getDefaultProgram () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramUser getProgram(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Retrieve a HardwareProgram from its name.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramUser getProgram(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the default program.
    //////////////////////////////////////////////////////////////////////
    virtual void _loadDefaultProgram () = 0 ;
    
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
    /// @brief Internal method to create HardwareShader.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShaderHolder iCreateHardwareShader ( const ShaderType& stype , const std::string& name , const std::string& source ) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method to create HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramHolder iCreateHardwareProgram ( const std::string& name , const HardwareShaderHolder& vshader , const HardwareShaderHolder& fshader ) const = 0;
    
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

/// @brief SpecializedCountedObjectHolder for HardwareProgramManagerPrivate.
typedef SpecializedCountedObjectHolder<HardwareProgramManager> HardwareProgramManagerHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramManagerPrivate list.
typedef SpecializedResourceHolderList<HardwareProgramManager> HardwareProgramManagerHolderList;

/// @brief SpecializedCountedObjectUser for HardwareProgramManager.
typedef SpecializedCountedObjectUser<HardwareProgramManager> HardwareProgramManagerUser;

//////////////////////////////////////////////////////////////////////
/// @brief A HardwareProgramManager Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManagerLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManagerLoader();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramManagerLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareProgramManager.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramManagerHolder load( const std::string& name ) const;
};

typedef ResourceLoaderFactory<HardwareProgramManagerLoader> HardwareProgramManagerLoaderFactory;

GreEndNamespace

#endif
