//
//  OpenGlHardwareVariable.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "OpenGlHardwareVariable.h"

GRE_BEGIN_NAMESPACE

OpenGlHardwareVariable::OpenGlHardwareVariable(const std::string& name, const VariableUsage& usage, const Matrix4& mat4)
: HardwareProgramVariable(name, usage, mat4), _mCachedAttribLoc(0)
{
    
}

OpenGlHardwareVariable::~OpenGlHardwareVariable()
{
    
}

void OpenGlHardwareVariable::bind() const
{
    if(_mCachedAttribLoc >= 0 && _mUsage == VariableUsage::Uniform) {
        
        if(_mType == VariableType::Mat4) {
            glUniformMatrix4fv(_mCachedAttribLoc, 1, GL_FALSE, &_mMat4[0][0]);
            _mIsDirty = false;
        }
        
    }
}

GRE_END_NAMESPACE
