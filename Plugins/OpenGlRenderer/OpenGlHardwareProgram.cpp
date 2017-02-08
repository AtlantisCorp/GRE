//
//  OpenGlHardwareProgram.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "OpenGlHardwareProgram.h"
#include "OpenGlHardwareVariable.h"
#include "OpenGlRenderContext.h"

GreBeginNamespace

OpenGlHardwareProgram::OpenGlHardwareProgram(const std::string& name, const HardwareShader& vs, const HardwareShader& fs)
: HardwareProgramPrivate(name, vs, fs), _mProgramId(0), _mLinked(false)
{
    if(!vs.expired() && !fs.expired())
    {
        GLuint vsid = *((const GLuint*)(vs.getCustomData("OpenGlId")));
        GLuint fsid = *((const GLuint*)(fs.getCustomData("OpenGlId")));
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Linking Program '" << name << "'." << Gre::gendl;
#endif
        
        _mProgramId = glGlobalContext->getGl().CreateProgram();
        glGlobalContext->getGl().AttachShader(_mProgramId, vsid);
        glGlobalContext->getGl().AttachShader(_mProgramId, fsid);
        glGlobalContext->getGl().LinkProgram(_mProgramId);
        
        GLint result = GL_FALSE;        int infoLogLenght;
        
        glGlobalContext->getGl().GetProgramiv(_mProgramId, GL_LINK_STATUS, &result);
        glGlobalContext->getGl().GetProgramiv(_mProgramId, GL_INFO_LOG_LENGTH, &infoLogLenght);
        if(infoLogLenght > 0) {
            std::vector<char> programErrorMessage(infoLogLenght+1);
            glGlobalContext->getGl().GetProgramInfoLog(_mProgramId, infoLogLenght, NULL, &programErrorMessage[0]);
            GreDebugPretty() << "Glsl Linker Error : " << &programErrorMessage[0] << Gre::gendl;
        }
        
        glGlobalContext->getGl().DetachShader(_mProgramId, vsid);
        glGlobalContext->getGl().DetachShader(_mProgramId, fsid);
        if(result == GL_TRUE)
            _mLinked = true;
        else
            _mLinked = false;
    }
  
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Tried to create OpenGlHardwareProgram without vertex/fragment shader." << Gre::gendl;
    }
#endif
}

OpenGlHardwareProgram::~OpenGlHardwareProgram()
{
    if(_mProgramId)
    {
        glGlobalContext->getGl().DeleteProgram(_mProgramId);
    }
}

void OpenGlHardwareProgram::bind() const
{
    if(_mProgramId)
    {
        glGlobalContext->getGl().UseProgram(_mProgramId);
        for(auto variable : _mVariables.getAttribs())
        {
            if(variable->isDirty()) {
                variable->bind();
            }
        }
        
        for(auto variable : _mVariables.getUniforms())
        {
            if(variable->isDirty()) {
                variable->bind();
            }
        }
    }
}

void OpenGlHardwareProgram::unbind() const
{
    glGlobalContext->getGl().UseProgram(0);
}

void OpenGlHardwareProgram::attachShader(const Gre::HardwareShader &hdwShader)
{
    if(!hdwShader.expired() && _mProgramId) {
        glGlobalContext->getGl().AttachShader(_mProgramId, *(GLuint*) hdwShader.getCustomData("OpenGlId"));
        if(hdwShader.getType() == ShaderType::Vertex)
            _mVertexShader = hdwShader;
        else if(hdwShader.getType() == ShaderType::Fragment)
            _mFragmentShader = hdwShader;
    }
}

void OpenGlHardwareProgram::finalize()
{
    if(!_mProgramId)
    {
        _mProgramId = glGlobalContext->getGl().CreateProgram();
    }
    
    if(_mProgramId)
    {
        GLuint vsid = *(GLuint*)(_mVertexShader.getCustomData("OpenGlId"));
        GLuint fsid = *(GLuint*)(_mFragmentShader.getCustomData("OpenGlId"));
        
        glGlobalContext->getGl().AttachShader(_mProgramId, vsid);
        glGlobalContext->getGl().AttachShader(_mProgramId, fsid);
        glGlobalContext->getGl().LinkProgram(_mProgramId);
        
        GLint result = GL_FALSE;
        int infoLogLenght;
        
        glGlobalContext->getGl().GetProgramiv(_mProgramId, GL_LINK_STATUS, &result);
        glGlobalContext->getGl().GetProgramiv(_mProgramId, GL_INFO_LOG_LENGTH, &infoLogLenght);
        if(infoLogLenght > 0) {
            std::vector<char> programErrorMessage(infoLogLenght+1);
            glGlobalContext->getGl().GetProgramInfoLog(_mProgramId, infoLogLenght, NULL, &programErrorMessage[0]);
            GreDebugPretty() << "Glsl Linker Error : " << &programErrorMessage[0] << Gre::gendl;
        }
        
        glGlobalContext->getGl().DetachShader(_mProgramId, vsid);
        glGlobalContext->getGl().DetachShader(_mProgramId, fsid);
        
        if(result == GL_TRUE)
            _mLinked = true;
        else
            _mLinked = false;
    }
}

bool OpenGlHardwareProgram::isReady() const
{
    return _mLinked;
}

void OpenGlHardwareProgram::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    if(_mProgramId)
    {
        HardwareProgramVariables::VariableHolder vholder = _mVariables.findUniform(name);
        if(vholder) {
            vholder->set(mat4);
        } else {
            OpenGlHardwareVariable* newVariable = new OpenGlHardwareVariable(name, VariableUsage::Uniform, mat4);
            newVariable->_mCachedAttribLoc = glGlobalContext->getGl().GetUniformLocation(_mProgramId, name.c_str());
            _mVariables.addUniform(newVariable);
        }
    }
}

int OpenGlHardwareProgram::getAttribLocation(const std::string& name) const
{
    if(_mProgramId)
    {
        return glGlobalContext->getGl().GetAttribLocation(_mProgramId, name.c_str());
    }
    
    return -1;
}

GreEndNamespace
