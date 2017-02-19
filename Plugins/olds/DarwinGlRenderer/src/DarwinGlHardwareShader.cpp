//////////////////////////////////////////////////////////////////////
//
//  DarwinGlHardwareShader.cpp
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

#include "DarwinGlHardwareShader.h"

namespace DarwinGl
{
    GLenum GlShaderType ( const ShaderType & type )
    {
        if ( type == ShaderType::Vertex )
            return GL_VERTEX_SHADER;
        
        if ( type == ShaderType::Fragment )
            return GL_FRAGMENT_SHADER;
        
        return GL_INVALID_ENUM;
    }
    
    DarwinGlHardwareShader::DarwinGlHardwareShader ( const std::string& name , const ShaderType& type , const std::string& src )
    : Gre::HardwareShaderPrivate(name, type, src)
    , iGlShader(0)
    {
        
    }
    
    DarwinGlHardwareShader::DarwinGlHardwareShader ( const std::string& name , const ShaderType& type )
    : Gre::HardwareShaderPrivate(name, type)
    , iGlShader(0)
    {
        
    }
    
    DarwinGlHardwareShader::~DarwinGlHardwareShader()
    {
        if ( iGlShader )
            glDeleteShader(iGlShader);
    }
    
    bool DarwinGlHardwareShader::compile()
    {
        if ( iGlShader )
        {
            glDeleteShader(iGlShader);
            iGlShader = 0;
        }

        std::string src = getSource() ;
        
        if ( src.empty() )
            return false;
        
        iGlShader = glCreateShader( GlShaderType(getType()) );
        GLint sz = (GLint) src.size();
        GLchar* srcp = & src [0];
        glShaderSource( iGlShader, 1, (const GLchar**) & srcp, &sz );
        glCompileShader( iGlShader );
        
        iCompiled = iCheckCompilation(iGlShader);
        return iCompiled;
    }
    
    void DarwinGlHardwareShader::clear()
    {
        if ( iGlShader )
        {
            glDeleteShader(0);
            iGlShader = 0;
        }
        
        HardwareShaderPrivate::clear();
    }
    
    const void* DarwinGlHardwareShader::getProperty(const std::string &name) const
    {
        if ( name == "GlShader" )
            return & iGlShader;
        
        return HardwareShaderPrivate::getProperty(name);
    }
    
    bool DarwinGlHardwareShader::iCheckCompilation(GLuint shaderID)
    {
        // http://alexandre-laurent.developpez.com/tutoriels/OpenGL/OpenGL-GLSL/?page=page_2
        
        GLint compilationStatus = 0;
        
        // Vérification de la compilation pour le vertex shader
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus);
        
        if ( compilationStatus != GL_TRUE )
        {
            // Nous savons que la compilation a échoué, donc nous devons savoir pourquoi
            // Nous devons récupéré la taille du log
            GLint logLength = 0;
            GLchar* log = NULL;
            
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
            
            // Maintenant que nous avons la taille, nous pouvons alloué la mémoire suffisante pour ce log
            log = (GLchar*)malloc(logLength);
            if ( log == NULL )
            {
                fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
                return false;
            }
            
            glGetShaderInfoLog(shaderID, logLength, &logLength, log);
            
            // On peut afficher le message
            fprintf(stderr,"Erreur de compilation:\n%s",log);
            
            // Et on n'oublie pas de libéré la mémoire
            free(log);
            
            return false;
        }
        
        return true; // Pas d'erreur
    }
}
