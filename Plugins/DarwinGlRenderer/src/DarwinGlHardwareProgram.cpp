//////////////////////////////////////////////////////////////////////
//
//  DarwinGlHardwareProgram.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/10/2016.
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

#include "DarwinGlHardwareProgram.h"

namespace DarwinGl
{
    DarwinGlHardwareProgram::DarwinGlHardwareProgram ( const std::string& name , const HardwareShader& vertexShader, const HardwareShader& fragmentShader )
    : Gre::HardwareProgramPrivate(name, vertexShader, fragmentShader)
    , iGlProgram(0)
    , iGlLinked(false)
    {
        attachShader(vertexShader);
        attachShader(fragmentShader);
    }
    
    DarwinGlHardwareProgram::~DarwinGlHardwareProgram()
    {
        if ( iGlProgram )
        {
            glDeleteProgram(iGlProgram);
            iGlProgram = 0;
        }
    }
    
    void DarwinGlHardwareProgram::bind() const
    {
        if ( iGlProgram )
        {
            glUseProgram(iGlProgram);
        }
    }
    
    void DarwinGlHardwareProgram::unbind() const
    {
        glUseProgram(0);
    }
    
    void DarwinGlHardwareProgram::attachShader(const Gre::HardwareShader &hwdShader)
    {
        if ( iGlLinked )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Can't attach HardwareShader to HardwareProgram because this one is already linked." << std::endl;
#endif
            return;
        }
        
        if ( !iGlProgram )
        {
            iGlProgram = glCreateProgram();
        }
        
        if ( !hwdShader.isInvalid() )
        {
            HardwareShaderHolder shaderholder = hwdShader.lock();
            GLuint shaderid = *  ( (const GLuint*) shaderholder->getProperty("GlShader") );
            
            if ( shaderid )
            {
                glAttachShader(iGlProgram, shaderid);
            }
        }
    }
    
    void DarwinGlHardwareProgram::finalize()
    {
        if ( !iGlLinked && iGlProgram )
        {
            GLint programstate = 0;
            glLinkProgram(iGlProgram);
            
            // Check Link errors.
            
            glGetProgramiv(iGlProgram, GL_LINK_STATUS, &programstate);
            if ( programstate != GL_TRUE )
            {
                // On récupère la taille du log
                GLint logSize = 0;
                GLchar* log = NULL;
                
                glGetProgramiv(iGlProgram, GL_INFO_LOG_LENGTH, &logSize);
                
                // On peut allouer la mémoire, une fois que l'on a la taille du log
                log = (GLchar*)malloc(logSize);
                if ( log == NULL )
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Can't allocate log memory." << std::endl;
#endif
                }
                
                else
                {
                    // Et on récupère le log
                    glGetProgramInfoLog(iGlProgram, logSize, &logSize, log);
                    
                    // On affiche
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Error linking program '" << getName() << "' : " << log << std::endl;
#endif
                    
                    free(log);
                }
            }
            
            else
            {
                iGlLinked = true;
            }
        }
    }
    
    int DarwinGlHardwareProgram::getAttribLocation(const std::string &name) const
    {
        if ( iGlLinked )
            return glGetAttribLocation(iGlProgram, name.c_str());
        
        return -1;
    }
    
    void DarwinGlHardwareProgram::reset()
    {
        if ( iGlProgram )
            glDeleteProgram(iGlProgram);
        iGlProgram = 0;
        iGlLinked = false;
        HardwareProgramPrivate::reset();
    }
    
    bool DarwinGlHardwareProgram::hasOutputLocation(uint16_t location) const
    {
        // This is a workaround.
        return true;
    }
    
    void DarwinGlHardwareProgram::bindTextureUnit(int unit) const
    {
        glActiveTexture(unit);
    }
    
    int DarwinGlHardwareProgram::getAttribLocation(const Gre::VertexComponentType &component) const
    {
        if ( iGlLinked )
            return glGetAttribLocation( iGlProgram , VertexComponentTypeToString(component).c_str() );
        
        return -1;
    }
    
    void DarwinGlHardwareProgram::setAttribData(int index, size_t stride, const char *data) const
    {
        // TODO
    }
    
    void DarwinGlHardwareProgram::bindAttributeLocation(const Gre::VertexComponentType &component, int index)
    {
        if ( iGlProgram && !iGlLinked )
            glBindAttribLocation( iGlProgram, index, VertexComponentTypeToString(component).c_str() );
    }
    
    int DarwinGlHardwareProgram::getUniformLocation(const std::string &name) const
    {
        if ( iGlLinked )
            return glGetUniformLocation(iGlProgram, name.c_str());
        return -1;
    }
    
    void DarwinGlHardwareProgram::setUniformVariable(const Gre::HardwareProgramVariable &var) const
    {
        GLint loc = -1;
        
        if ( var.location == -1 )
            loc = glGetUniformLocation(iGlProgram, var.name.c_str());
        else
            loc = var.location;
        
        if ( var.type == HdwProgVarType::Mat4 )
            glUniformMatrix4fv(loc, 1, false, & var.value.mat4[0][0]);
        
        if ( var.type == HdwProgVarType::Vec2 )
            glUniform2fv(loc, 1, & var.value.vec2[0]);
        
        if ( var.type == HdwProgVarType::Vec3 )
            glUniform3fv(loc, 1, & var.value.vec3[0]);
        
        if ( var.type == HdwProgVarType::Vec4 )
            glUniform4fv(loc, 1, & var.value.vec4[0]);
    }
}
