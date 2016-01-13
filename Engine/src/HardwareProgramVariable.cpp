//
//  HardwareProgramVariable.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "HardwareProgramVariable.h"

GRE_BEGIN_NAMESPACE

HardwareProgramVariable::HardwareProgramVariable(const std::string& name, const VariableUsage& usage, const Matrix4& mat4)
: _mName(name), _mType(VariableType::Mat4), _mMat4(mat4), _mUsage(usage), _mIsDirty(true)
{
    
}

HardwareProgramVariable::~HardwareProgramVariable()
{
    
}

bool HardwareProgramVariable::isDirty() const
{
    return _mIsDirty;
}

void HardwareProgramVariable::set(const Matrix4 &mat4)
{
    if(_mType == VariableType::Mat4 && mat4 != _mMat4) {
        _mMat4 = mat4;
        _mIsDirty = true;
    }
}

const std::string& HardwareProgramVariable::getName() const
{
    return _mName;
}

GRE_END_NAMESPACE
