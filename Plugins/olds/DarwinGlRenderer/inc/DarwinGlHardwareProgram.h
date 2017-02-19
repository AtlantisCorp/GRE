//////////////////////////////////////////////////////////////////////
//
//  DarwinGlHardwareProgram.h
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

#ifndef DarwinGlHardwareProgram_h
#define DarwinGlHardwareProgram_h

#include "DarwinGlHeader.h"
#include "HardwareProgram.h"

namespace DarwinGl
{
    //////////////////////////////////////////////////////////////////////
    /// @brief OpenGl Glsl HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    class DarwinGlHardwareProgram : public HardwareProgramPrivate
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlHardwareProgram ( const std::string& name , const HardwareShader& vertexShader = HardwareShader::Null, const HardwareShader& fragmentShader = HardwareShader::Null ) ;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual ~DarwinGlHardwareProgram () ;
        
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
        /// @brief Returns the location of a given attribute variable, or -1
        /// if this does not exists.
        //////////////////////////////////////////////////////////////////////
        virtual int getAttribLocation(const std::string& name) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Reset the HardwareProgram.
        /// This function also clears the HardwareShader it holds, the Variables
        /// and the Compile datas.
        //////////////////////////////////////////////////////////////////////
        virtual void reset();
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns true if the Compiled HardwareProgram has the current
        /// output location given.
        //////////////////////////////////////////////////////////////////////
        virtual bool hasOutputLocation(uint16_t location) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Activate the given Texture Unit.
        //////////////////////////////////////////////////////////////////////
        virtual void bindTextureUnit ( int unit ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the location for the given component.
        /// If this component is not found, it will return '-1' .
        //////////////////////////////////////////////////////////////////////
        virtual int getAttribLocation ( const VertexComponentType& component ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Sets the data that should be given as argument for the
        /// HardwareProgram at given index.
        ///
        /// @param index : Location for the data to be set.
        /// @param stride : The stride between two contiguous data.
        /// @param data : A pointer to an array of data.
        ///
        //////////////////////////////////////////////////////////////////////
        virtual void setAttribData ( int index , size_t stride , const char* data ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Bind an Attribute to the HardwareProgram's given location.
        /// This should be called before linking the HardwareProgram.
        //////////////////////////////////////////////////////////////////////
        virtual void bindAttributeLocation ( const VertexComponentType& component, int index );
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns the Location for given uniform, or -1.
        //////////////////////////////////////////////////////////////////////
        virtual int getUniformLocation ( const std::string& name ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Changes the value of a given uniform location.
        /// This value is changed when the function is called, not when updating
        /// the HardwareProgram.
        //////////////////////////////////////////////////////////////////////
        virtual void setUniformVariable ( const HardwareProgramVariable& var ) const;
        
    protected:
        
        /// @brief OpenGl Program object.
        GLuint iGlProgram ;
        
        /// @brief Link status.
        bool iGlLinked ;
    };
}

#endif /* DarwinGlHardwareProgram_h */
