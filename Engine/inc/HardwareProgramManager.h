//
//  HardwareProgramManager.h
//  GRE
//
//  Created by Jacques Tronconi on 10/01/2016.
//
//

#ifndef GRE_HardwareProgramManager_h
#define GRE_HardwareProgramManager_h

#include "Pools.h"
#include "HardwareProgram.h"
#include "Resource.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief A Manager to collect and organize every HardwareProgram
/// objects, and every HardwareShader Objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManagerPrivate : public Resource
{
public:
    
    POOLED(Pools::Manager)
    
    HardwareProgramManagerPrivate(const std::string& name);
    virtual ~HardwareProgramManagerPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Variable in the Sdk for every programs.
    /// This variable will be assigned to every vertex shaders currently
    /// loaded.
    //////////////////////////////////////////////////////////////////////
    virtual void setSdkUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareShader loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload the given HardwareShader object from the Shader
    /// database.
    //////////////////////////////////////////////////////////////////////
    void unloadShader(const HardwareShader& ref);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader);
    
protected:
    
    typedef std::map<std::string, std::shared_ptr<HardwareProgramPrivate> > ProgramsByNameShared;
    typedef std::vector<HardwareShader> ShadersListWeak;
    
    /// @brief The Programs, organized by name and managed by std::shared_ptr.
    ProgramsByNameShared _mPrograms;
    
    /// @brief A list of shaders. Those Shaders are collected by the ResourceManager, so
    /// the HardwareProgramManager has no owning of these resources.
    ShadersListWeak _mShaders;
};

//////////////////////////////////////////////////////////////////////
/// @brief A quick HardwareProgramManagerPrivate user.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramManager : public ResourceUser
{
public:
    
    POOLED(Pools::Manager)
    
    HardwareProgramManager();
    HardwareProgramManager(const HardwareProgramManager& rhs);
    HardwareProgramManager(HardwareProgramManager&& rhs);
    explicit HardwareProgramManager(const ResourceUser& rhs);
    HardwareProgramManager& operator = (const HardwareProgramManager& rhs);
    bool operator == (const HardwareProgramManager& rhs) const;
    bool operator != (const HardwareProgramManager& rhs) const;
    
    ~HardwareProgramManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Variable in the Sdk for every programs.
    /// This variable will be assigned to every Programs loaded in this
    /// Program Manager.
    //////////////////////////////////////////////////////////////////////
    void setSdkUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    //////////////////////////////////////////////////////////////////////
    HardwareShader loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload the given HardwareShader object from the Shader
    /// database.
    //////////////////////////////////////////////////////////////////////
    void unloadShader(const HardwareShader& ref);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader);
    
private:
    
    /// @brief A weak ptr to the direct type.
    std::weak_ptr<HardwareProgramManagerPrivate> _mHwdManager;
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

GRE_END_NAMESPACE

#endif
