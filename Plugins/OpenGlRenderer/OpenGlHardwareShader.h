//
//  OpenGlHardwareShader.h
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#ifndef GRE_OpenGlHardwareShader_h
#define GRE_OpenGlHardwareShader_h

#include "OpenGlIncludes.h"

GRE_BEGIN_NAMESPACE

class OpenGlHardwareShader : public HardwareShaderPrivate
{
public:
    
    POOLED(Pools::Resource)
    
    OpenGlHardwareShader(const std::string& name, const ShaderType& type, const std::string& text);
    ~OpenGlHardwareShader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data for user.
    //////////////////////////////////////////////////////////////////////
    const void* getCustomData(const std::string& dataname) const;
    
private:
    
    GLuint _mShaderId;
    bool _mIsCompiled;
};

//////////////////////////////////////////////////////////////////////
/// @brief A OpenGlHardwareShader Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC OpenGlHardwareShaderLoader : public HardwareShaderLoader
{
public:
    
    POOLED(Pools::Loader)
    
    
    OpenGlHardwareShaderLoader();
    virtual ~OpenGlHardwareShaderLoader();
    ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareShader.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    Resource* load(Resource::Type type, const std::string& name, const std::string& shaderSource, const ShaderType& stype) const;
    
};

GRE_END_NAMESPACE

#endif
