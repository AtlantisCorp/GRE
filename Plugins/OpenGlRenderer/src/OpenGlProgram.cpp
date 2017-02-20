//////////////////////////////////////////////////////////////////////
//
//  OpenGlProgram.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/02/2017.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#include "OpenGlRenderer.h"

OpenGlProgram::OpenGlProgram (const std::string& name ,
                              const Gre::HardwareShaderUser& vertex ,
                              const Gre::HardwareShaderUser& fragment ,
                              bool cacheShaders)
: Gre::HardwareProgram(name, vertex, fragment, cacheShaders)
, iGlProgram(0)
{
    _create() ;
    if ( iGlProgram )
    {
        _attachShader(vertex) ;
        _attachShader(fragment) ;
        _finalize() ;
    }
}

OpenGlProgram::~OpenGlProgram() noexcept ( false )
{
    _deleteProgram() ;
}

void OpenGlProgram::_create ()
{
    GreAutolock ;
    
    if ( !iGlProgram ) {
        iGlProgram = glCreateProgram();
    }
}

void OpenGlProgram::_bind() const
{
    GreAutolock ;
    
    if ( iGlProgram && iCompiled ) {
        glUseProgram(iGlProgram) ;
        iBinded = true ;
    }
}

void OpenGlProgram::_unbind() const
{
    GreAutolock ;
    
    glUseProgram(0);
    iBinded = false ;
}

void OpenGlProgram::_attachShader(const Gre::HardwareShaderUser &hwdShader, bool cacheShader)
{
    GreAutolock ;
    
    if ( hwdShader.isInvalid() || !iGlProgram || iCompiled )
        return ;
    
    const Gre::HardwareShaderHolder & holder = hwdShader.lock() ;
    
    if ( holder->getType() == Gre::ShaderType::Vertex )
    {
        const OpenGlShader* glshader = reinterpret_cast<const OpenGlShader*>(holder.getObject()) ;
        glAttachShader(iGlProgram, glshader->getGlShader());
        
        if ( cacheShader )
            iVertexShader = hwdShader ;
    }
    
    else if ( holder->getType() == Gre::ShaderType::Fragment )
    {
        const OpenGlShader* glshader = reinterpret_cast<const OpenGlShader*>(holder.getObject()) ;
        glAttachShader(iGlProgram, glshader->getGlShader());
        
        if ( cacheShader )
            iFragmentShader = hwdShader ;
    }
}

void OpenGlProgram::_finalize()
{
    GreAutolock ;
    
    if ( !iGlProgram || iCompiled )
        return ;
    
    glLinkProgram(iGlProgram);
    
    GLint status ; glGetProgramiv(iGlProgram, GL_LINK_STATUS, &status) ;
    if ( status == GL_FALSE ) {
        GLint infosz ; glGetProgramiv(iGlProgram, GL_INFO_LOG_LENGTH, &infosz) ;
        GLsizei lenght ;
        GLchar* buf = (GLchar*) malloc ( infosz + 1 ) ;
        glGetProgramInfoLog(iGlProgram, infosz, &lenght, buf) ;
        buf[lenght] = '\0' ;
        
        GreDebug("[WARN] Can't link program '") << getName() << "' : " << std::string(buf) << Gre::gendl ;
    }
    
    iCompiled = true ;
    
    // Try to iterate through the program uniforms.
    
    GLint uniformcount = 0 ;
    GLint uniformmaxlenght = 0 ;
    
    glGetProgramiv(iGlProgram, GL_ACTIVE_UNIFORMS, &uniformcount) ;
    GreDebug("Found ") << uniformcount << " uniforms." << Gre::gendl ;
    
    if ( uniformcount )
    {
        glGetProgramiv(iGlProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformmaxlenght) ;
        GreDebug("Uniform Max Lenght = ") << uniformmaxlenght << Gre::gendl ;
        
        if ( uniformmaxlenght )
        {
            for ( int i = 0 ; i < uniformcount ; ++i )
            {
                char* buf = (char*) malloc ( uniformmaxlenght+1 ) ;
                GLsizei lenght = 0 ;
                
                glGetActiveUniformName(iGlProgram, i, uniformmaxlenght, &lenght, buf);
                buf [uniformmaxlenght] = '\0' ;
                
                GreDebug("Uniform name = '") << std::string(buf) << "'." << Gre::gendl ;
                
                GLint location = glGetUniformLocation(iGlProgram, buf) ;
                GreDebug("Uniform location = ") << location << "." << Gre::gendl ;
            }
        }
    }
    
    // Try to iterate through Attributes.
    
    GLint attrcount = 0 ;
    GLint attrmaxlenght = 0 ;
    
    glGetProgramiv(iGlProgram, GL_ACTIVE_ATTRIBUTES, &attrcount) ;
    GreDebug("Found ") << attrcount << " attributes." << Gre::gendl ;
    
    if ( attrcount )
    {
        glGetProgramiv(iGlProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attrmaxlenght) ;
        GreDebug("AttrMaxLenght = ") << attrmaxlenght << Gre::gendl ;
        
        if ( attrmaxlenght )
        {
            for ( int i = 0 ; i < attrcount ; ++i )
            {
                char* buf = (char*) malloc ( attrmaxlenght+1 ) ;
                GLsizei lenght = 0 ;
                
                glGetActiveAttrib(iGlProgram, i, attrmaxlenght, &lenght, NULL, NULL, buf) ;
                buf [lenght] = '\0' ;
                GreDebug("ActivAttrib = '") << std::string(buf) << "'." << Gre::gendl ;
                
                GLint location = glGetAttribLocation(iGlProgram, buf) ;
                GreDebug("Location = ") << location << Gre::gendl ;
            }
        }
    }
}

void OpenGlProgram::_deleteProgram()
{
    GreAutolock ;
    if ( !iGlProgram )
        return ;
    
    glDeleteProgram(iGlProgram) ;
    iGlProgram = 0 ;
    iCompiled = false ;
    iBinded = false ;
}

void OpenGlProgram::_setAttribLocation(const std::string &name, int loc) const
{
    GreAutolock ;
    
    if ( iGlProgram ) {
        glBindAttribLocation(iGlProgram, loc, name.c_str());
    }
}

int OpenGlProgram::getAttribLocation(const std::string &name) const
{
    GreAutolock ;
    
    if ( iGlProgram ) {
        return glGetAttribLocation(iGlProgram, name.c_str());
    }
    
    return -1 ;
}

void OpenGlProgram::bindTextureUnit(int unit) const
{
    GreAutolock ;
    
    if ( iGlProgram ) {
        glActiveTexture(GL_TEXTURE0+unit);
    }
}

int OpenGlProgram::getUniformLocation(const std::string &name) const
{
    GreAutolock ;
    
    if ( iGlProgram ) {
        return glGetUniformLocation(iGlProgram, name.c_str());
    }
    
    return -1 ;
}

void OpenGlProgram::_setVariable(const Gre::HardwareProgramVariable &var) const
{
    GreAutolock ;
    
    if ( iGlProgram )
    {
        int location = var.location ;
        if ( location < 0 ) {
            location = getUniformLocation(var.name) ;
            if ( location < 0 ) {
                return ;
            }
        }
        
        if ( var.type == Gre::HdwProgVarType::Float1 ) {
            glUniform1fv(location, 1, & var.value.f1);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Float2 ) {
            glUniform2fv(location, 1, & var.value.f2[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Float3 ) {
            glUniform3fv(location, 1, & var.value.f3[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Float4 ) {
            glUniform4fv(location, 1, & var.value.f4[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Int1 ) {
            glUniform1iv(location, 1, & var.value.i1);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Int2 ) {
            glUniform2iv(location, 1, & var.value.i2[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Int3 ) {
            glUniform3iv(location, 1, & var.value.i3[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Int4 ) {
            glUniform4iv(location, 1, & var.value.i4[0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Matrix2 ) {
            glUniformMatrix2fv(location, 1, false, & var.value.m2[0][0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Matrix3 ) {
            glUniformMatrix3fv(location, 1, false, & var.value.m3[0][0]);
        }
        
        else if ( var.type == Gre::HdwProgVarType::Matrix4 ) {
            glUniformMatrix4fv(location, 1, false, & var.value.m4[0][0]);
        }
    }
}




