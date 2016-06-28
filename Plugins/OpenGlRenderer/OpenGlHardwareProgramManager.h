//
//  OpenGlHardwareProgramManager.h
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#ifndef GRE_OpenGlHardwareProgramManager_h
#define GRE_OpenGlHardwareProgramManager_h

#include "OpenGlIncludes.h"

#include "HardwareProgramManager.h"

GreBeginNamespace

class OpenGlHardwareProgramManager : public HardwareProgramManagerPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    OpenGlHardwareProgramManager(const std::string& name);
    ~OpenGlHardwareProgramManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a new Shader using the correct HardwareShaderLoader
    /// generally provided by the Renderer object.
    //////////////////////////////////////////////////////////////////////
    HardwareShader loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program using given Vertex and Fragment Shaders.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader);
};

//////////////////////////////////////////////////////////////////////
/// @brief A OpenGlHardwareProgramManager Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC OpenGlHardwareProgramManagerLoader : public HardwareProgramManagerLoader
{
public:
    
    POOLED(Pools::Loader)
    
    
    OpenGlHardwareProgramManagerLoader();
    virtual ~OpenGlHardwareProgramManagerLoader();
    ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareProgramManager.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    Resource* load(Resource::Type type, const std::string& name) const;
    
};

GreEndNamespace

#endif
