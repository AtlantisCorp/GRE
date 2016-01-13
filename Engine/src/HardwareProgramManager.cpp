//
//  HardwareProgramManager.cpp
//  GRE
//
//  Created by Jacques Tronconi on 10/01/2016.
//
//

#include "HardwareProgramManager.h"
#include "ResourceManager.h"

GRE_BEGIN_NAMESPACE

HardwareProgramManagerPrivate::HardwareProgramManagerPrivate(const std::string& name)
: Resource(name)
{
    
}

HardwareProgramManagerPrivate::~HardwareProgramManagerPrivate()
{
    
}

void HardwareProgramManagerPrivate::setSdkUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    // When setting a sdk variable, we just set it to every vertex shaders.
    for(auto program : _mPrograms)
    {
        program.second->setUniformMat4(name, mat4);
    }
}

HardwareShader HardwareProgramManagerPrivate::loadShader(const ShaderType& stype, const std::string &name, const std::string &filepath)
{
    GreDebugFunctionNotImplemented();
    return HardwareShader::Null;
}

void HardwareProgramManagerPrivate::unloadShader(const Gre::HardwareShader &ref)
{
    if(!ref.expired())
    {
        // For performance reason, we check the result of std::find only
        // with GreIsDebugMode enabled. If not, we jump every steps and let
        // the compiler a maximum of liberty to optimize everything.
        
#ifndef GreIsDebugMode
        _mShaders.erase(std::find(_mShaders.begin(), _mShaders.end(), ref));
        ResourceManager::Get().unloadResource(ref.getName());
#endif
        
#ifdef GreIsDebugMode
        ShadersListWeak::iterator it = std::find(_mShaders.begin(), _mShaders.end(), ref);
        if(it == _mShaders.end())
        {
            GreDebugPretty() << "Not found any pointer named '" << ref.getName() << "'." << std::endl;
        }
        else
        {
            _mShaders.erase(it);
            ResourceManager::Get().unloadResource(ref.getName());
        }
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Given HardwareShader reference was Null." << std::endl;
    }
#endif
}

HardwareProgram HardwareProgramManagerPrivate::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    GreDebugFunctionNotImplemented();
    return HardwareProgram::Null;
}

// ---------------------------------------------------------------------------------------------------

HardwareProgramManager::HardwareProgramManager()
: ResourceUser(), _mHwdManager()
{
    
}

HardwareProgramManager::HardwareProgramManager(const HardwareProgramManager& rhs)
: ResourceUser(rhs), _mHwdManager(rhs._mHwdManager)
{
    
}

HardwareProgramManager::HardwareProgramManager(HardwareProgramManager&& rhs)
: ResourceUser(rhs), _mHwdManager(std::move(rhs._mHwdManager))
{
    
}

HardwareProgramManager::HardwareProgramManager(const ResourceUser& rhs)
: ResourceUser(rhs), _mHwdManager(std::dynamic_pointer_cast<HardwareProgramManagerPrivate>(rhs.lock()))
{
    
}

HardwareProgramManager& HardwareProgramManager::operator=(const Gre::HardwareProgramManager &rhs)
{
    ResourceUser::operator=(rhs);
    _mHwdManager = rhs._mHwdManager;
    return *this;
}

bool HardwareProgramManager::operator==(const Gre::HardwareProgramManager &rhs) const
{
    return _mHwdManager.lock() == rhs._mHwdManager.lock();
}

bool HardwareProgramManager::operator!=(const Gre::HardwareProgramManager &rhs) const
{
    return _mHwdManager.lock() != rhs._mHwdManager.lock();
}

HardwareProgramManager::~HardwareProgramManager()
{
    
}

void HardwareProgramManager::setSdkUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    auto ptr = _mHwdManager.lock();
    if(ptr)
        ptr->setSdkUniformMat4(name, mat4);
}

HardwareShader HardwareProgramManager::loadShader(const Gre::ShaderType &stype, const std::string &name, const std::string &filepath)
{
    auto ptr = _mHwdManager.lock();
    if(ptr)
        return ptr->loadShader(stype, name, filepath);
    return HardwareShader::Null;
}

void HardwareProgramManager::unloadShader(const Gre::HardwareShader &ref)
{
    auto ptr = _mHwdManager.lock();
    if(ptr)
        ptr->unloadShader(ref);
}

HardwareProgram HardwareProgramManager::createHardwareProgram(const std::string &name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    auto ptr = _mHwdManager.lock();
    if(ptr)
        return ptr->createHardwareProgram(name, vertexShader, fragmentShader);
    return HardwareProgram::Null;
}

// ---------------------------------------------------------------------------------------------------

HardwareProgramManagerLoader::HardwareProgramManagerLoader()
{
    
}

HardwareProgramManagerLoader::~HardwareProgramManagerLoader()
{
    
}

bool HardwareProgramManagerLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::HwdProgManager;
}

Resource* HardwareProgramManagerLoader::load(Resource::Type type, const std::string &name) const
{
    return nullptr;
}

GRE_END_NAMESPACE
