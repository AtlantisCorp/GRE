//
//  HardwareProgram.h
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#ifndef GRE_HardwareProgram_h
#define GRE_HardwareProgram_h

#include "Pools.h"
#include "Resource.h"
#include "HardwareShader.h"
#include "HardwareProgramVariables.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Shader Program.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    HardwareProgramPrivate(const std::string& name,
                           const HardwareShader& vertexShader = HardwareShader::Null,
                           const HardwareShader& fragmentShader = HardwareShader::Null);
    
    virtual ~HardwareProgramPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Bind' the Program, make it usable.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Unbind' the Program, or detaches it from the pipeline.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach the given shader to this Program.
    /// Remember that only one shader of each type can be attached.
    //////////////////////////////////////////////////////////////////////
    virtual void attachShader(const HardwareShader& hwdShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    /// Call this function when every shaders is attached. 
    //////////////////////////////////////////////////////////////////////
    virtual void finalize();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Program is ready to be used (i.e. not
    /// binded but correctly compiled).
    //////////////////////////////////////////////////////////////////////
    virtual bool isReady() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Dictionnary of every Variables that use this
    /// Program.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariables getVariables();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a uniform Matrix4 variable in this Program.
    //////////////////////////////////////////////////////////////////////
    virtual void setUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    virtual int getAttribLocation(const std::string& name) const;
    
protected:
    
    /// @brief The VertexShader object.
    HardwareShader _mVertexShader;
    
    /// @brief The FragmentShader object.
    HardwareShader _mFragmentShader;
    
    /// @brief Holds a table to HardwareProgramVariableReference Objects.
    /// Those reference are entry in the vertex shader.
    HardwareProgramVariables _mVariables;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Shader Program object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgram : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    HardwareProgram();
    HardwareProgram(const HardwareProgram& rhs);
    HardwareProgram(HardwareProgram&& rhs);
    explicit HardwareProgram(const ResourceUser& rhs);
    HardwareProgram& operator = (const HardwareProgram& rhs);
    bool operator == (const HardwareProgram& rhs) const;
    bool operator != (const HardwareProgram& rhs) const;
    
    ~HardwareProgram();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Bind' the Program, make it usable.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Unbind' the Program, or detaches it from the pipeline.
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    /// Call this function when every shaders is attached.
    //////////////////////////////////////////////////////////////////////
    void finalize();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Program is ready to be used (i.e. not
    /// binded but correctly compiled).
    //////////////////////////////////////////////////////////////////////
    bool isReady() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Dictionnary of every Variables that use this
    /// Program.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariables getVariables();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a uniform Matrix4 variable in this Program.
    //////////////////////////////////////////////////////////////////////
    void setUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    int getAttribLocation(const std::string& name) const;
    
    static HardwareProgram Null;
    
private:
    
    /// @brief Holds a weaked HardwareProgramPrivate pointer.
    std::weak_ptr<HardwareProgramPrivate> _mHwdProgram;
};

GreEndNamespace

#endif
