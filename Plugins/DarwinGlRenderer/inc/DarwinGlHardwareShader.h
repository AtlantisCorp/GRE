//////////////////////////////////////////////////////////////////////
//
//  DarwinGlHardwareShader.h
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

#ifndef DarwinGlHardwareShader_h
#define DarwinGlHardwareShader_h

#include "DarwinGlHeader.h"
#include "HardwareShader.h"

namespace DarwinGl
{
    //////////////////////////////////////////////////////////////////////
    /// @brief OpenGl HardwareShader.
    //////////////////////////////////////////////////////////////////////
    class DarwinGlHardwareShader : public HardwareShaderPrivate
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlHardwareShader ( const std::string& name , const ShaderType& type , const std::string& src );
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlHardwareShader ( const std::string& name , const ShaderType& type );
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual ~DarwinGlHardwareShader () ;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual bool compile ();
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual void clear () ;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns a custom property defined by given name.
        //////////////////////////////////////////////////////////////////////
        const void* getProperty ( const std::string& name ) const;
        
    protected:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Check Errors at compile time for OpenGl Shaders.
        //////////////////////////////////////////////////////////////////////
        static bool iCheckCompilation ( GLuint shaderID ) ;
        
    protected:
        
        /// @brief Shader ID .
        GLuint iGlShader ;
    };
}

#endif /* DarwinGlHardwareShader_h */
