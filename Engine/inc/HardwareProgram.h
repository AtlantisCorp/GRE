//////////////////////////////////////////////////////////////////////
//
//  HardwareProgram.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
#include "Light.h"

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
    //////////////////////////////////////////////////////////////////////
    HardwareProgram ( const std::string & name ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgram(const std::string& name,
                    const HardwareShaderUser& vertexShader = HardwareShaderUser(nullptr),
                    const HardwareShaderUser& fragmentShader = HardwareShaderUser(nullptr),
                    bool cacheShaders = false );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgram() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program instance.
    //////////////////////////////////////////////////////////////////////
    virtual void _create () = 0 ;
    
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
    virtual void _attachShader ( const HardwareShaderUser& hwdShader, bool cacheShader = false ) = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    //////////////////////////////////////////////////////////////////////
    virtual void _finalize() = 0 ;
    
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
    /// @brief Adds an alias for given location component.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttribLocation ( const std::string & name , const VertexComponentType& component ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds an alias to given location.
    //////////////////////////////////////////////////////////////////////
    virtual void _setAttribLocation ( const std::string & name , int loc ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the components location.
    /// @note When changing the location for this component, every alias
    /// may be unvalidated.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttribLocation ( const VertexComponentType& component , int location ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    virtual int getAttribLocation ( const std::string& name ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given component in the Program.
    //////////////////////////////////////////////////////////////////////
    virtual int getAttribLocation ( const VertexComponentType& component ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Program has been successfully compiled.
    //////////////////////////////////////////////////////////////////////
    virtual bool isCompiled() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the HardwareProgram.
    /// This function also clears the HardwareShader it holds, the Variables
    /// and the Compile datas.
    //////////////////////////////////////////////////////////////////////
    virtual void reset();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activate the given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void bindTextureUnit ( int unit ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Location for given uniform, or -1.
    //////////////////////////////////////////////////////////////////////
    virtual int getUniformLocation ( const std::string& name ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the value of the given Variable.
    ///
    /// @note If the HardwareProgram is not binded , it will cache the query
    /// and do it when the HardwareProgram is binded.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void setVariable ( const HardwareProgramVariable & var ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Manually sets a Variable.
    //////////////////////////////////////////////////////////////////////
    virtual void _setVariable ( const HardwareProgramVariable & var ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activates given lights.
    //////////////////////////////////////////////////////////////////////
    virtual void setLights ( const std::vector < Light > & lights ) const ;
    
protected:
    
    /// @brief VertexShader object.
    HardwareShaderUser iVertexShader;
    
    /// @brief FragmentShader object.
    HardwareShaderUser iFragmentShader;
    
    /// @brief Summarize the components locations.
    std::map < VertexComponentType , int > iComponentLocations ;
    
    /// @brief Variables Cache.
    mutable HardwareProgramVariables iCachedVariables;
    
    /// @brief
    mutable bool iCompiled ;
    
    /// @brief
    mutable bool iBinded ;
};

/// @brief SpecializedCountedObjectHolder for HardwareProgramPrivate.
typedef SpecializedCountedObjectHolder<HardwareProgram> HardwareProgramHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramPrivate.
typedef SpecializedResourceHolderList<HardwareProgram> HardwareProgramHolderList;

/// @brief SpecializedCountedObjectUser for HardwareProgram.
typedef SpecializedCountedObjectUser<HardwareProgram> HardwareProgramUser;

GreEndNamespace

#endif
