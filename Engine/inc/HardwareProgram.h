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

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Shader Program.
///
/// SubClassing Notes
/// When subclassing this Class, overwrite the method ::onUpdateEvent().
/// In this method, if ::iNeedsVarUpdate is true, then upload every
/// HardwareVariable object in ::iVariables. Then, set the flag to
/// false to prevent massive uploading.
///
/// Using the Program
///   - HardwareProgramManager:: createHardwareProgram();
///   - HardwareProgram:: attachShader( VertexShader );
///   - HardwareProgram:: attachShader( FragmentShader );
///   - HardwareProgram:: finalize();
///   - Pass:: setHardwareProgram( ThisProgram );
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgram : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgram(const std::string& name,
                    const HardwareShaderUser& vertexShader = HardwareShaderUser(nullptr),
                    const HardwareShaderUser& fragmentShader = HardwareShaderUser(nullptr));
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgram();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Bind' the Program, make it usable.
    //////////////////////////////////////////////////////////////////////
    virtual void use() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Unbind' the Program, or detaches it from the pipeline.
    //////////////////////////////////////////////////////////////////////
    virtual void unuse() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach the given shader to this Program.
    /// Remember that only one shader of each type can be attached.
    //////////////////////////////////////////////////////////////////////
    virtual void attachShader(const HardwareShaderUser& hwdShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    /// Call this function when every shaders is attached. 
    //////////////////////////////////////////////////////////////////////
    virtual void finalize();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Program is ready to be used (i.e. not
    /// binded but correctly compiled).
    //////////////////////////////////////////////////////////////////////
    virtual bool isReady() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    virtual int getAttribLocation(const std::string& name) const;
    
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
    /// @brief Returns true if the Compiled HardwareProgram has the current
    /// output location given.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasOutputLocation(uint16_t location) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activate the given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void bindTextureUnit ( int unit ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the given VertexDescriptor.
    /// The Attributes should be binded in the order given by the descriptor,
    /// or if the name corresponding to given entree already exists in the
    /// HardwareProgram.
    ///
    /// @param descriptor : The descriptor that should describe the Vertex
    /// structure.
    /// @param data : A generic pointer used to point to the first Vertex's
    /// data. This pointer should be 'nullptr' if the first Vertex used is
    /// the first data of an HardwareBuffer ( and not a SoftwareBuffer ).
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void bindAttribsVertex ( const VertexDescriptor& descriptor, const char* data ) const;
    
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
    /// @brief Sets the value of the given Variable.
    ///
    /// @note If the HardwareProgram is not binded , it will cache the query
    /// and do it when the HardwareProgram is binded.
    //////////////////////////////////////////////////////////////////////
    virtual void setVariable ( const HardwareProgramVariable & var ) const ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the program.
    //////////////////////////////////////////////////////////////////////
    virtual void iBind () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the program.
    //////////////////////////////////////////////////////////////////////
    virtual void iUnbind () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Upload the given variable to the program.
    //////////////////////////////////////////////////////////////////////
    virtual void iUploadVariable ( const HardwareProgramVariable & var ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal helper for subclass to change the iNeedsCompilation
    /// flags.
    /// This function should be called whenever the Compiled state of this
    /// Program has changed. For example, when resetted, it should be false.
    /// After successfull compilation, should be true.
    //////////////////////////////////////////////////////////////////////
    void setCompiled(bool flag);
    
protected:
    
    /// @brief The VertexShader object.
    HardwareShaderUser iVertexShader;
    
    /// @brief The FragmentShader object.
    HardwareShaderUser iFragmentShader;
    
    /// @brief A Cached list of Variables not uploaded to the program because it was not
    /// binded yet.
    mutable HardwareProgramVariables iCachedVariables;
    
    /// @brief Flags set to true if this Program needs compilation.
    mutable bool iNeedsCompilation;
    
    /// @brief Flag true if this Program needs to upload its variables.
    mutable bool iNeedsVarUpdate;
    
    /// @brief True if the program is currently bind, false otherwise.
    /// It is used to know if a variable or a uniform can be uploaded
    /// or not. If not, the next time the program is binded, it will
    /// also bind those variables.
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
