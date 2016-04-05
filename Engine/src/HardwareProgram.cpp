//
//  HardwareProgram.cpp
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#include "HardwareProgram.h"

GreBeginNamespace

HardwareProgramPrivate::HardwareProgramPrivate(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader)
: Resource(name), _mVertexShader(vertexShader), _mFragmentShader(fragmentShader)
{
    
}

HardwareProgramPrivate::~HardwareProgramPrivate()
{
    
}

void HardwareProgramPrivate::bind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareProgramPrivate::unbind() const
{
    GreDebugFunctionNotImplemented();
}

void HardwareProgramPrivate::attachShader(const HardwareShader& hwdShader)
{
    GreDebugFunctionNotImplemented();
}

void HardwareProgramPrivate::finalize()
{
    GreDebugFunctionNotImplemented();
}

bool HardwareProgramPrivate::isReady() const
{
    GreDebugFunctionNotImplemented();
    return false;
}

HardwareProgramVariables HardwareProgramPrivate::getVariables()
{
    return _mVariables;
}

void HardwareProgramPrivate::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    GreDebugFunctionNotImplemented();
}

int HardwareProgramPrivate::getAttribLocation(const std::string& name) const
{
    GreDebugFunctionNotImplemented();
    return -1;
}

// ---------------------------------------------------------------------------------------------------

HardwareProgram::HardwareProgram()
: ResourceUser(), _mHwdProgram()
{
    
}

HardwareProgram::HardwareProgram(const HardwareProgram& rhs)
: ResourceUser(rhs), _mHwdProgram(rhs._mHwdProgram)
{
    
}

HardwareProgram::HardwareProgram(HardwareProgram&& rhs)
: ResourceUser(rhs), _mHwdProgram(std::move(rhs._mHwdProgram))
{
    
}

HardwareProgram::HardwareProgram(const ResourceUser& rhs)
: ResourceUser(rhs), _mHwdProgram(std::dynamic_pointer_cast<HardwareProgramPrivate>(rhs.lock()))
{
    
}

HardwareProgram& HardwareProgram::operator = (const HardwareProgram& rhs)
{
    ResourceUser::operator=(rhs);
    _mHwdProgram = rhs._mHwdProgram;
    return *this;
}

bool HardwareProgram::operator==(const HardwareProgram &rhs) const
{
    return _mHwdProgram.lock().get() == rhs._mHwdProgram.lock().get();
}

bool HardwareProgram::operator!=(const HardwareProgram &rhs) const
{
    return _mHwdProgram.lock().get() != rhs._mHwdProgram.lock().get();
}

HardwareProgram::~HardwareProgram()
{
    
}

void HardwareProgram::bind() const
{
    auto ptr = _mHwdProgram.lock();
    
    if(ptr)
        ptr->bind();
}

void HardwareProgram::unbind() const
{
    auto ptr = _mHwdProgram.lock();
    
    if(ptr)
        ptr->unbind();
}

void HardwareProgram::finalize()
{
    auto ptr = _mHwdProgram.lock();
    if(ptr)
        ptr->finalize();
}

bool HardwareProgram::isReady() const
{
    auto ptr = _mHwdProgram.lock();
    if(ptr)
        return ptr->isReady();
    return false;
}

HardwareProgramVariables HardwareProgram::getVariables()
{
    auto ptr = _mHwdProgram.lock();
    if(ptr)
        return ptr->getVariables();
    return HardwareProgramVariables::Empty;
}

void HardwareProgram::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    auto ptr = _mHwdProgram.lock();
    if(ptr)
        ptr->setUniformMat4(name, mat4);
}

int HardwareProgram::getAttribLocation(const std::string& name) const
{
    auto ptr = _mHwdProgram.lock();
    if(ptr)
        return ptr->getAttribLocation(name);
    return -1;
}

HardwareProgram HardwareProgram::Null = HardwareProgram();

GreEndNamespace
