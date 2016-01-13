//
//  HardwareProgramVariables.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "HardwareProgramVariables.h"

GRE_BEGIN_NAMESPACE

HardwareProgramVariables::HardwareProgramVariables()
{
    
}

HardwareProgramVariables::~HardwareProgramVariables()
{
    
}

void HardwareProgramVariables::addUniform(HardwareProgramVariable *hdwVariable)
{
    if(hdwVariable)
    {
        _mUniforms.push_back(VariableHolder(hdwVariable));
    }
}

std::vector<HardwareProgramVariables::VariableHolder>& HardwareProgramVariables::getAttribs()
{
    return _mAttribs;
}

const std::vector<HardwareProgramVariables::VariableHolder>& HardwareProgramVariables::getAttribs() const
{
    return _mAttribs;
}

std::vector<HardwareProgramVariables::VariableHolder>& HardwareProgramVariables::getUniforms()
{
    return _mUniforms;
}

const std::vector<HardwareProgramVariables::VariableHolder>& HardwareProgramVariables::getUniforms() const
{
    return _mUniforms;
}

HardwareProgramVariables::VariableHolder HardwareProgramVariables::findUniform(const std::string& name)
{
    for(auto uniform : _mUniforms) {
        if(uniform->getName() == name)
            return uniform;
    }
    
    return VariableHolder();
}

HardwareProgramVariables HardwareProgramVariables::Empty = HardwareProgramVariables();

GRE_END_NAMESPACE
