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

std::string GetFileContent ( const std::string & path )
{
    // Loading the source from file.
    // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    
    std::ifstream srcstream ( path );
    
    if ( !srcstream )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't open file '" << path << "'." << Gre::gendl;
#endif
        return std::string () ;
    }
    
    std::string src;
    
    srcstream.seekg(0, std::ios::end);
    src.reserve(srcstream.tellg());
    srcstream.seekg(0, std::ios::beg);
    
    src.assign( (std::istreambuf_iterator<char>(srcstream)), std::istreambuf_iterator<char>() );
    return src ;
}

std::string OpenGlShaderMakeSource ( const std::string & source )
{
    std::string value ;
    
    char c = 0 ;
    std::stringstream stream ( source ) ;
    while ( stream >> std::noskipws >> c )
    {
        if ( c == '#' )
        {
            std::string word ; stream >> word ;
            if ( word == "include" ) {
                stream >> std::skipws >> word ;
                value . append( OpenGlShaderMakeSource(GetFileContent(word)) ) ;
            } else {
                value . push_back(c) ;
                value . append(word) ;
            }
        }
        
        else
        {
            value . push_back(c) ;
        }
    }
    
    return value ;
}

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
    GreAutolock ; CHECK_GLEW
    
    std::string realsource = OpenGlShaderMakeSource(source) ;
    
    GLenum shadtype = translateGlShader(type) ;
    iGlShader = glCreateShader(shadtype);
    
    if ( iGlShader )
    {
        const char* src = realsource.c_str() ;
        GLint lenght = realsource.size() ;
        
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
