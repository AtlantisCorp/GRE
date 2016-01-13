//
//  HardwareShader.h
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#ifndef GRE_HardwareShader_h
#define GRE_HardwareShader_h

#include "Pools.h"
#include "Resource.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief Defines the Shader type that can be constructed.
//////////////////////////////////////////////////////////////////////
enum class ShaderType
{
    Null,
    Vertex,
    Fragment
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareShaderPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    HardwareShaderPrivate(const std::string& name, const ShaderType& type, const std::string& text);
    virtual ~HardwareShaderPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Type of this Shader.
    //////////////////////////////////////////////////////////////////////
    ShaderType getType() const;
    
private:
    
    /// @brief Holds the Shader's type.
    ShaderType _mType;
};

//////////////////////////////////////////////////////////////////////
/// @brief User object to manipulate HardwareShaderPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareShader : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    HardwareShader();
    HardwareShader(const HardwareShader& rhs);
    HardwareShader(HardwareShader&& rhs);
    explicit HardwareShader(const ResourceUser& rhs);
    HardwareShader& operator = (const HardwareShader& rhs);
    bool operator == (const HardwareShader& rhs) const;
    bool operator != (const HardwareShader& rhs) const;
    
    ~HardwareShader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Type of this Shader.
    //////////////////////////////////////////////////////////////////////
    ShaderType getType() const;
    
    /// @brief A Null HardwareShader object.
    static HardwareShader Null;
    
    /// @brief The HardwareShader Passthrough.
    static HardwareShader VertexPassThrough;
    static HardwareShader FragmentPassThrough;
    
private:
    
    /// @brief Holds the Weak pointer.
    std::weak_ptr<HardwareShaderPrivate> _mShader;
};

//////////////////////////////////////////////////////////////////////
/// @brief A HardwareShader Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareShaderLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    HardwareShaderLoader();
    virtual ~HardwareShaderLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::HdwShader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a HardwareShader.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual Resource* load(Resource::Type type, const std::string& name, const std::string& filepath, const ShaderType& stype) const;
};

typedef ResourceLoaderFactory<HardwareShaderLoader> HardwareShaderLoaderFactory;

GRE_END_NAMESPACE

#endif
