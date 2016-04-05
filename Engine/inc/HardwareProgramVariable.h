//
//  HardwareProgramVariable.h
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#ifndef GRE_HardwareProgramVariable_h
#define GRE_HardwareProgramVariable_h

#include "Pools.h"

GreBeginNamespace

enum class VariableType
{
    Mat4
};

enum class VariableUsage
{
    Attrib,
    Uniform
};

//////////////////////////////////////////////////////////////////////
/// @brief Holds a simple Program Variable.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramVariable
{
public:
    
    HardwareProgramVariable(const std::string& name, const VariableUsage& usage, const Matrix4& mat4);
    virtual ~HardwareProgramVariable();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Variable must be (re)binded.
    //////////////////////////////////////////////////////////////////////
    bool isDirty() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the current Variable to the currently used Program.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the value of this Variable. Type must be correct.
    //////////////////////////////////////////////////////////////////////
    void set(const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the name of the Variable.
    //////////////////////////////////////////////////////////////////////
    const std::string& getName() const;
    
protected:
    
    /// @brief Holds the name of this Variable.
    std::string _mName;
    
    /// @brief Holds the type of this Variable.
    VariableType _mType;
    
    /// @brief Holds the usage of this Variable.
    VariableUsage _mUsage;
    
    /// @brief True if the variable must be (re)binded.
    mutable bool _mIsDirty;
    
    union {
        Matrix4 _mMat4;
    };
};

GreEndNamespace

#endif
