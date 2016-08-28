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
#include "Material.h"

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
class DLL_PUBLIC HardwareProgramPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramPrivate(const std::string& name,
                           const HardwareShader& vertexShader = HardwareShader::Null,
                           const HardwareShader& fragmentShader = HardwareShader::Null);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramPrivate();
    
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
    /// @brief Returns true if the Program is ready to be used (i.e. not
    /// binded but correctly compiled).
    //////////////////////////////////////////////////////////////////////
    virtual bool isReady() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Dictionnary of every Variables that use this
    /// Program.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariables getVariables();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a uniform Matrix4 variable in this Program.
    //////////////////////////////////////////////////////////////////////
    virtual void setUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new HardwareVariable.
    //////////////////////////////////////////////////////////////////////
    virtual void setUniform(const HardwareProgramVariable& var);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unsets the HardwareVariable.
    //////////////////////////////////////////////////////////////////////
    virtual void unsetUniform(const std::string& name);
    
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
    /// @brief Binds Material's data to the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void bindMaterial ( const Material& material ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds Materials' data from the HardwareProgram.
    /// Basically, it is used to tell Material's Textures to unbind.
    //////////////////////////////////////////////////////////////////////
    virtual void unbindMaterial ( const Material& material ) const;
    
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
    HardwareShader iVertexShader;
    
    /// @brief The FragmentShader object.
    HardwareShader iFragmentShader;
    
    /// @brief Holds a table to HardwareProgramVariableReference Objects.
    /// Those reference are entry in the vertex shader.
    HardwareProgramVariables iVariables;
    
    /// @brief Flags set to true if this Program needs compilation.
    mutable bool iNeedsCompilation;
    
    /// @brief Flag true if this Program needs to upload its variables.
    mutable bool iNeedsVarUpdate;
};

/// @brief SpecializedResourceHolder for HardwareProgramPrivate.
typedef SpecializedResourceHolder<HardwareProgramPrivate> HardwareProgramHolder;

/// @brief SpecializedResourceHolderList for HardwareProgramPrivate.
typedef SpecializedResourceHolderList<HardwareProgramPrivate> HardwareProgramHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for HardwareProgramPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgram : public SpecializedResourceUser<HardwareProgramPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgram(const HardwareProgramPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgram(const HardwareProgramHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgram(const HardwareProgram& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgram();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Bind' the Program, make it usable.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief 'Unbind' the Program, or detaches it from the pipeline.
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    /// Call this function when every shaders is attached.
    //////////////////////////////////////////////////////////////////////
    void finalize();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Program is ready to be used (i.e. not
    /// binded but correctly compiled).
    //////////////////////////////////////////////////////////////////////
    bool isReady() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Dictionnary of every Variables that use this
    /// Program.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariables getVariables();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a uniform Matrix4 variable in this Program.
    //////////////////////////////////////////////////////////////////////
    void setUniformMat4(const std::string& name, const Matrix4& mat4);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new HardwareVariable.
    //////////////////////////////////////////////////////////////////////
    void setUniform(const HardwareProgramVariable& var);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unsets the HardwareVariable.
    //////////////////////////////////////////////////////////////////////
    void unsetUniform(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    int getAttribLocation(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Program has been successfully compiled.
    //////////////////////////////////////////////////////////////////////
    bool isCompiled() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the HardwareProgram.
    /// This function also clears the HardwareShader it holds, the Variables
    /// and the Compile datas.
    //////////////////////////////////////////////////////////////////////
    void reset();
    
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
    
    /// @brief Null HardwareProgram.
    static HardwareProgram Null;
};

GreEndNamespace

#endif
