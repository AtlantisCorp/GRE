//
//  HardwareShader.cpp
//  GRE
//
//  Created by Jacques Tronconi on 09/01/2016.
//
//

#include "HardwareShader.h"

GreBeginNamespace

HardwareShaderPrivate::HardwareShaderPrivate(const std::string& name, const ShaderType& type, const std::string& text)
: Resource(name), _mType(type)
{

}

HardwareShaderPrivate::~HardwareShaderPrivate()
{
    
}

ShaderType HardwareShaderPrivate::getType() const
{
    return _mType;
}

// ---------------------------------------------------------------------------------------------------

HardwareShader::HardwareShader()
: ResourceUser(), _mShader()
{
    
}

HardwareShader::HardwareShader(const HardwareShader& rhs)
: ResourceUser(rhs), _mShader(rhs._mShader)
{
    
}

HardwareShader::HardwareShader(HardwareShader&& rhs)
: ResourceUser(rhs), _mShader(std::move(rhs._mShader))
{
    
}

HardwareShader::HardwareShader(const ResourceUser& rhs)
: ResourceUser(rhs), _mShader(std::dynamic_pointer_cast<HardwareShaderPrivate>(rhs.lock()))
{
    
}

HardwareShader& HardwareShader::operator=(const HardwareShader& rhs)
{
    ResourceUser::operator=(rhs);
    _mShader = rhs._mShader;
    return *this;
}

bool HardwareShader::operator==(const Gre::HardwareShader &rhs) const
{
    return _mShader.lock() == rhs._mShader.lock();
}

bool HardwareShader::operator!=(const Gre::HardwareShader &rhs) const
{
    return !(*this == rhs);
}

HardwareShader::~HardwareShader()
{
    
}

ShaderType HardwareShader::getType() const
{
    auto ptr = _mShader.lock();
    if(ptr)
        return ptr->getType();
    return ShaderType::Null;
}

// ---------------------------------------------------------------------------------------------------

HardwareShaderLoader::HardwareShaderLoader()
{
    
}

HardwareShaderLoader::~HardwareShaderLoader()
{
    
}

bool HardwareShaderLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::HdwShader;
}

Resource* HardwareShaderLoader::load(Resource::Type type, const std::string &name, const std::string& filepath, const Gre::ShaderType &stype) const
{
    GreDebugFunctionNotImplemented();
    return nullptr;
}

HardwareShader HardwareShader::Null = HardwareShader();
HardwareShader HardwareShader::VertexPassThrough = HardwareShader();
HardwareShader HardwareShader::FragmentPassThrough = HardwareShader();

GreEndNamespace
