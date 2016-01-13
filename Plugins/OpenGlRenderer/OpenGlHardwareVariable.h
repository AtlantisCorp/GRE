//
//  OpenGlHardwareVariable.h
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#ifndef GRE_OpenGlHardwareVariable_h
#define GRE_OpenGlHardwareVariable_h

#include "OpenGlIncludes.h"
#include "HardwareProgramVariable.h"

GRE_BEGIN_NAMESPACE

class OpenGlHardwareVariable : public HardwareProgramVariable
{
public:
    
    OpenGlHardwareVariable(const std::string& name, const VariableUsage& usage, const Matrix4& mat4);
    ~OpenGlHardwareVariable();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the current Variable to the currently used Program.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    /// @brief Holds the cached location of the variable.
    /// This is necessary because we can't set the variable without the program
    /// id and only with the variable name, glGetAttrib/UniformLocation require
    /// the program Id.
    GLint _mCachedAttribLoc;
};

GRE_END_NAMESPACE

#endif
