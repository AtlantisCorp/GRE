//
//  HardwareShader.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "OpenGlHardwareShader.h"
#include "OpenGlRenderContext.h"

GreBeginNamespace

OpenGlHardwareShader::OpenGlHardwareShader(const std::string& name, const ShaderType& type, const std::string& text)
: HardwareShaderPrivate(name, type, text), _mShaderId(0), _mIsCompiled(false)
{
    if(!text.empty())
    {
        if(type == ShaderType::Vertex)
            _mShaderId = glGlobalContext->getGl().CreateShader(GL_VERTEX_SHADER);
        else if(type == ShaderType::Fragment)
            _mShaderId= glGlobalContext->getGl().CreateShader(GL_FRAGMENT_SHADER);

        if(_mShaderId)
        {
            const char* source = text.c_str();
//          GLint lenght = (GLint) text.size();
            GLint isCompiled;
            glGlobalContext->getGl().ShaderSource(_mShaderId, 1, (const GLchar**) &source, 0);
            glGlobalContext->getGl().CompileShader(_mShaderId);
            
            int infoLogLenght;
            glGlobalContext->getGl().GetShaderiv(_mShaderId, GL_COMPILE_STATUS, &isCompiled);
            glGlobalContext->getGl().GetShaderiv(_mShaderId, GL_INFO_LOG_LENGTH, &infoLogLenght);
            if(isCompiled == GL_FALSE)
            {
                std::vector<char> shaderErrorMessage(infoLogLenght+1);
                glGlobalContext->getGl().GetProgramInfoLog(_mShaderId, infoLogLenght, NULL, &shaderErrorMessage[0]);
                GreDebugPretty() << "Glsl Linker Error : " << &shaderErrorMessage[0] << std::endl;
            }
            
            else
            {
                _mIsCompiled = true;
            }
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "Can't define type of shader '" << name << "'." << std::endl;
        }
#endif
    }
}

OpenGlHardwareShader::~OpenGlHardwareShader()
{
    if(_mShaderId)
        glGlobalContext->getGl().DeleteShader(_mShaderId);
}

const void* OpenGlHardwareShader::getCustomData(const std::string &dataname) const
{
    if(dataname == "OpenGlId")
        return &_mShaderId;
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------

OpenGlHardwareShaderLoader::OpenGlHardwareShaderLoader()
{
    
}

OpenGlHardwareShaderLoader::~OpenGlHardwareShaderLoader()
{
    
}

ResourceLoader* OpenGlHardwareShaderLoader::clone() const
{
    return new OpenGlHardwareShaderLoader();
}

Resource* OpenGlHardwareShaderLoader::load(Resource::Type type, const std::string &name, const std::string &shaderSource, const Gre::ShaderType &stype) const
{
    return new OpenGlHardwareShader(name, stype, shaderSource);
}

GreEndNamespace
