//////////////////////////////////////////////////////////////////////
//
//  HardwareProgram.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 06/01/2016.
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

#ifndef GRE_HardwareProgram_h
#define GRE_HardwareProgram_h

#include "Pools.h"
#include "Resource.h"
#include "HardwareShader.h"
#include "HardwareProgramVariables.h"
#include "VertexDescriptor.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Shader Program.
///
/// A Shader Program is a set of multiples Shaders that describes one
/// of the pipeline step to render a scene.
///
/// You can construct a program using the constructor, or use 'internal'
/// underscored functions to create your program manually. Please remember
/// that creating a program manually using an object already compiled
/// has undefined behaviour.
///
/// @note
/// When subclassing HardwareProgram, you should make your constructor
/// compiling the given arguments if the program was given some.
///
/// @note
/// HardwareProgram comes with built-in GRE variables for your shaders.
/// Those variables are :
/// - A material structure for the current object. ("material")
/// - A lights array filled with lights structure for the current scene.
/// - A Camera structure for the current camera.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgram : public Resource
{
public:

    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs an empty program. This object must be first attached
    /// to shaders (at least one vertex and one fragment) , and finalized before
    /// being used.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgram() noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief If this program is not finalized , this function attaches the
    /// given shaders to the program object using '_attachShader'.
    //////////////////////////////////////////////////////////////////////
    virtual void attachShaders ( const HardwareShaderHolderList & shaders ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief If this program is not finalized , this function finalizes
    /// the program using the '_finalize' function. It should also caches
    /// the different parameters and attributes locations in the program.
    //////////////////////////////////////////////////////////////////////
    virtual void finalize () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void _bind () const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void _unbind () const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Attach the given shader to this Program.
    //////////////////////////////////////////////////////////////////////
    virtual bool _attachShader ( const HardwareShaderHolder & hwdShader ) = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    //////////////////////////////////////////////////////////////////////
    virtual bool _finalize() = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the Program internal object.
    //////////////////////////////////////////////////////////////////////
    virtual void _deleteProgram () = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the HardwareProgram and upload any caches variables.
    //////////////////////////////////////////////////////////////////////
    virtual void use() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void unuse() const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iLinked' .
    //////////////////////////////////////////////////////////////////////
    virtual bool isFinalized () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Program has been successfully linked.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLinked() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Resets the program. Status remain not linked , not validated
    /// and every attached shaders are cleared.
    //////////////////////////////////////////////////////////////////////
    virtual void reset();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this program is currently bound.
    //////////////////////////////////////////////////////////////////////
    virtual bool isBound () const ;

public:

    // Those functions may be used only when 'isLinked()' returns true, i.e. when the
    // program has been successfully linked. When a program is not linked , those functions
    // shoud do nothing or emit a warning message.

    // ---------------------------------------------------------------------------------------------------
    // Attributes checking functions.

    //////////////////////////////////////////////////////////////////////
    /// @brief Looks for the given attribute name and bind it to defined
    /// component , enabling it.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexAttrib (const std::string & attrib ,
                                  size_t elements ,
                                  VertexAttribType type ,
                                  bool normalize ,
                                  size_t stride ,
                                  void * pointer) const = 0 ;

    // ---------------------------------------------------------------------------------------------------
    // Uniform setting / checking functions.

    //////////////////////////////////////////////////////////////////////
    /// @brief Should bind the texture unit with given number .
    //////////////////////////////////////////////////////////////////////
    virtual void bindTextureUnit ( int unit ) const = 0 ;

	//////////////////////////////////////////////////////////////////////
	/// @brief Returns true if given uniform is present in this program.
	//////////////////////////////////////////////////////////////////////
	virtual bool isUniformValid ( const std::string & name ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Location for given uniform, or -1.
    //////////////////////////////////////////////////////////////////////
    virtual int getUniformLocation ( const std::string& name ) const ;

	//////////////////////////////////////////////////////////////////////
	/// @brief Sets a given uniform (if it exists in the program) to the
	/// given value.
	//////////////////////////////////////////////////////////////////////
	virtual bool setUniform ( const std::string & name , const HdwProgVarType & type , const RealProgramVariable & value ) const ;

	//////////////////////////////////////////////////////////////////////
	/// @brief Sets the given variable to the program, if it exists.
	//////////////////////////////////////////////////////////////////////
	virtual bool setUniform ( const HardwareProgramVariable & variable ) const ;

	//////////////////////////////////////////////////////////////////////
	/// @brief Internally sets the uniform to the program object.
	//////////////////////////////////////////////////////////////////////
	virtual bool _setUniform ( int location , const HdwProgVarType & type , const RealProgramVariable & value ) const = 0 ;

    // ---------------------------------------------------------------------------------------------------
    // Utilities

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the maximum number of lights supported by the
    /// program driver.
    //////////////////////////////////////////////////////////////////////
    virtual unsigned int getMaximumLights () const = 0 ;

protected:

    /// @brief Shaders attached to this program. As a program can have only
    /// one shader attached by shader type , we use a simple map to access
    /// them.
    std::map < ShaderType , HardwareShaderHolder > iAttachedShaders ;

    /// @brief Attributes name / location lookup table.
    std::map < std::string , int > iAttribsLocation ;

    /// @brief Variables Cache.
    mutable HardwareProgramVariables iCachedVariables;

    /// @brief
    mutable bool iLinked ;

    /// @brief
    mutable bool iBinded ;

    /// @brief Uniforms loaded when linking the program. Uniforms are constant values through the
    /// program process. They can be modified externally using setUniform(). For optimization purpose,
    /// every uniforms are cached into this map when linking the program. Notes, their value is not
    /// cached. Only name and location (i.e. shader index) are stored.
    std::map < std::string , HardwareProgramVariable > iUniforms ;
};

/// @brief SpecializedCountedObjectHolder for HardwareProgramPrivate.
typedef SpecializedCountedObjectHolder<HardwareProgram> HardwareProgramHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramPrivate.
typedef SpecializedResourceHolderList<HardwareProgram> HardwareProgramHolderList;

/// @brief SpecializedCountedObjectUser for HardwareProgram.
typedef SpecializedCountedObjectUser<HardwareProgram> HardwareProgramUser;

GreEndNamespace

#endif
