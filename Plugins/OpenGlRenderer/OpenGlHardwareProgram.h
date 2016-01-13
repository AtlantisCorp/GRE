//
//  OpenGlHardwareProgram.h
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#ifndef GRE_OpenGlHardwareProgram_h
#define GRE_OpenGlHardwareProgram_h

#include "OpenGlIncludes.h"
#include "HardwareProgram.h"

GRE_BEGIN_NAMESPACE

class OpenGlHardwareProgram : public HardwareProgramPrivate
{
public:
    
    OpenGlHardwareProgram(const std::string& name, const HardwareShader& vs, const HardwareShader& fs);
    ~OpenGlHardwareProgram();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Bind' the Program, make it usable.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Unbind' the Program, or detaches it from the pipeline.
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach the given shader to this Program.
    /// Remember that only one shader of each type can be attached.
    //////////////////////////////////////////////////////////////////////
    void attachShader(const HardwareShader& hdwShader);
    
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
    /// @brief Sets a uniform Matrix4 variable in this Program.
    //////////////////////////////////////////////////////////////////////
    void setUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    int getAttribLocation(const std::string& name) const;
    
private:
    
    GLuint _mProgramId;
    bool   _mLinked;
};

GRE_END_NAMESPACE

#endif
