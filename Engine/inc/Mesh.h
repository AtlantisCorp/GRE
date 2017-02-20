//////////////////////////////////////////////////////////////////////
//
//  Mesh.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/11/2015.
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

#ifndef GRE_Mesh_h
#define GRE_Mesh_h

#include "Resource.h"
#include "ResourceLoader.h"
#include "SpecializedResourceManager.h"

#include "BoundingBox.h"

#include "SoftwareVertexBuffer.h"
#include "SoftwareIndexBuffer.h"

#include "HardwareProgram.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Describes an object which can be draw on the screen.
///
/// Mesh objects stores HardwareVertexBuffers, wich describes the raw
/// vertex (points) data. Those buffers can be disabled if necessary.
///
/// This object also stores HardwareIndexBuffer, wich describes the link
/// between the vertexs data.
///
/// You can configure every buffers in order to customize the drawing of
/// your mesh ( enable/disable some buffers at some times ).
///
/// A Mesh is a static object loaded from a file, or from a MeshBuilder
/// object. A Mesh has a preferred material that should be used by default
/// by every Node that uses this Mesh. Also, the BoundingBox is calculated
/// when loading the Mesh.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Mesh : public Resource
{
public:
    
    POOLED ( Pools::Resource )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh ( const std::string & name ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Mesh ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iVertexBuffers' property.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexBuffers ( const HardwareVertexBufferHolderList & attributes ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iVertexBuffers' property.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareVertexBufferHolderList & getVertexBuffers ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iIndexBuffer' property.
    //////////////////////////////////////////////////////////////////////
    virtual void setIndexBuffer ( const HardwareIndexBufferHolder & buffer ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iIndexBuffer' .
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareIndexBufferHolder & getIndexBuffer ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears only the Vertex Buffers .
    //////////////////////////////////////////////////////////////////////
    virtual void clearVertexBuffers ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears only the index buffer .
    //////////////////////////////////////////////////////////////////////
    virtual void clearIndexBuffer ( ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iBoundingBox' .
    //////////////////////////////////////////////////////////////////////
    virtual void setBoundingBox ( const BoundingBox & bbox ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iBoundingBox' .
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox & getBoundingBox ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iOriginalFile'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string & getOriginalFilepath () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iOriginalFile'.
    //////////////////////////////////////////////////////////////////////
    virtual void setOriginalFilepath ( const std::string & filepath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iDefaultMaterial'.
    //////////////////////////////////////////////////////////////////////
    virtual const MaterialHolder& getPreferredMaterial () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iDefaultMaterial'.
    //////////////////////////////////////////////////////////////////////
    virtual void setPreferredMaterial ( const MaterialHolder& material ) ;
    
protected:
    
    /// @brief List of HardwareVertexBuffers.
    HardwareVertexBufferHolderList iVertexBuffers ;
    
    /// @brief The index buffer.
    HardwareIndexBufferHolder iIndexBuffer ;
    
    /// @brief BoundingBox for this Mesh.
    BoundingBox iBoundingBox ;
    
    /// @brief The file this mesh comes from, empty if this mesh was created inside the Engine.
    std::string iOriginalFile ;
    
    /// @brief Preferred Material for this Mesh.
    MaterialHolder iDefaultMaterial ;
};

/// @brief SpecializedCountedObjectHolder for Mesh .
typedef SpecializedCountedObjectHolder < Mesh > MeshHolder ;

/// @brief SpecializedResourceHolderList for Mesh .
typedef SpecializedResourceHolderList < Mesh > MeshHolderList ;

/// @brief SpecializedCountedObjectUser for Mesh .
typedef SpecializedCountedObjectUser < Mesh > MeshUser ;

////////////////////////////////////////////////////////////////////////
/// @brief Specialized ResourceLoader for Mesh .
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC MeshLoader : public ResourceLoader
{
public:
    
    POOLED ( Pools::Loader )
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    MeshLoader ( ) ;
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~MeshLoader ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads one or more Mesh from a file.
    /// As some files can store more than one mesh (OBJ file for example),
    /// a MeshLoader should be able to return more than one resource.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolderList load ( const std::string& name , const std::string& filepath ) const = 0;
};

/// @brief ResourceLoaderFactory for MeshLoader.
typedef ResourceLoaderFactory < MeshLoader > MeshLoaderFactory ;

//////////////////////////////////////////////////////////////////////
/// @brief Manages the Mesh loaded.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC MeshManager : public SpecializedResourceManager < Mesh , MeshLoader >
{
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Cube of given vertice size.
    /// A valid MeshManager should have been registered to the ResourceManager
    /// to use this method. Also, a valid render context should have been
    /// given to the renderer.
    //////////////////////////////////////////////////////////////////////
    static MeshUser Cube ( float sz ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a triangle.
    //////////////////////////////////////////////////////////////////////
    static MeshUser Triangle ( float sz ) ;
    
public:
    
    POOLED ( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MeshManager ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~MeshManager ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a file and returns the Meshes found in this file.
    //////////////////////////////////////////////////////////////////////
    virtual std::vector < MeshUser > load ( const std::string & name , const std::string & filepath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the first mesh wich origin file corresponds to given
    /// one.
    //////////////////////////////////////////////////////////////////////
    virtual MeshUser findFirstFile ( const std::string & filepath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareVertexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBufferHolder createVertexBuffer ( const void* data , size_t sz , const VertexDescriptor& desc ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareIndexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareIndexBufferHolder createIndexBuffer ( const void* data , size_t sz , const IndexDescriptor& desc ) const = 0 ;
};

/// @brief SpecializedCountedObjectHolder for MeshManager.
typedef SpecializedCountedObjectHolder < MeshManager > MeshManagerHolder ;

GreEndNamespace

#endif
