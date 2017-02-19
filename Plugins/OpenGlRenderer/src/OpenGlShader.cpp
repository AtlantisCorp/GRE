//////////////////////////////////////////////////////////////////////
//
//  OpenGlShader.cpp
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

GLenum translateGlShader ( const Gre::ShaderType& type )
{
    if ( type == Gre::ShaderType::Vertex )
        return GL_VERTEX_SHADER ;
    if ( type == Gre::ShaderType::Fragment )
        return GL_FRAGMENT_SHADER ;
    return GL_INVALID_ENUM ;
}

OpenGlShader::OpenGlShader ( const std::string & name , const Gre::ShaderType& type , const std::string& source )
: Gre::HardwareShader(name, type, source)
{
    GreAutolock ;
    
    iGlShader = glCreateShader(translateGlShader(type));
    if ( iGlShader )
    {
        const char* src = source.c_str() ;
        GLint lenght = source.size() ;
        
        glShaderSource(iGlShader, 1, &src, &lenght);
        glCompileShader(iGlShader);
        
        GLint status ; glGetShaderiv(iGlShader, GL_COMPILE_STATUS, &status);
        if ( status == GL_FALSE ) {
            GLint infosz ; glGetShaderiv(iGlShader, GL_INFO_LOG_LENGTH, &infosz);
            GLchar* buf = (char*) malloc(infosz+1);
            glGetShaderInfoLog(iGlShader, infosz+1, &lenght, buf);
            buf[lenght+1] = '\0';
            
            iErrorLog = std::string ( buf ) ;
        }
    }
}

OpenGlShader::~OpenGlShader() noexcept ( false )
{
    if ( iGlShader )
    {
        glDeleteShader(iGlShader);
        iGlShader = 0 ;
    }
}

const std::string& OpenGlShader::getErrorLog() const
{
    GreAutolock ; return iErrorLog ;
}

GLuint OpenGlShader::getGlShader() const
{
    GreAutolock ; return iGlShader ;
}
