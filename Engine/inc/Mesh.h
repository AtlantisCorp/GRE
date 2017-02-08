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
/// @brief Describes an Attribute for a Mesh buffer . One Attribute
/// can hold one buffer . This buffer can have one or many Vertex data,
/// like position , color , texture , normals , ...
//////////////////////////////////////////////////////////////////////
struct MeshAttribute
{
    HardwareVertexBufferHolder buffer ;
    bool enabled ;
};

/// @brief A List of MeshAttribute.
typedef std::list < MeshAttribute > MeshAttributeList ;

//////////////////////////////////////////////////////////////////////
/// @brief Describes an object which can be draw on the screen.
///
/// The Mesh is directly loaded from either a file , either a Renderer
/// for example.
///
/// When a Loader loads a Mesh , generally the buffers are Software's buffers,
/// which means the data is on the CPU's RAM . To make this Mesh on the GPU's
/// RAM , the Renderer should provides the 'convertMesh()' function . Also ,
/// the Renderer can install a different MeshManager which will convert
/// the Mesh automatically after loading it with the correct loader.
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
    virtual void setVertexBuffers ( const MeshAttributeList & attributes ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iVertexBuffers' property.
    //////////////////////////////////////////////////////////////////////
    virtual const MeshAttributeList & getVertexBuffers ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds an Attribute to the Attribute's list.
    //////////////////////////////////////////////////////////////////////
    virtual void addVertexBuffer ( const MeshAttribute & attribute ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iIndexBuffer' property.
    //////////////////////////////////////////////////////////////////////
    virtual void setIndexBuffer ( const HardwareIndexBufferHolder & buffer ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iIndexBuffer' .
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareIndexBufferHolder & getIndexBuffer ( ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Binds the Mesh for rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void bind ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears only the Vertex Buffers .
    //////////////////////////////////////////////////////////////////////
    virtual void clearVertexBuffers ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears only the index buffer .
    //////////////////////////////////////////////////////////////////////
    virtual void clearIndexBuffer ( ) ;
    
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
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the BoundingBox using given MeshAttribute.
    /// Generally this function is called when adding or changing one of
    /// the Vertex's Buffers , but when update events hit the object.
    //////////////////////////////////////////////////////////////////////
    virtual void iUpdateBoundingBox ( const MeshAttribute & attribute ) ;
    
protected:
    
    /// @brief List of HardwareVertexBuffers .
    /// Vertex's buffers can be multiple , because a Mesh can store its Vertex either using multiple buffers ,
    /// for example one for normals , one for colors , ... , but it can also store every Vertex data in one
    /// buffer. Each buffer is packed in an MeshAttribute structure which can be enabled or not.
    MeshAttributeList iVertexBuffers ;
    
    /// @brief The index buffer .
    /// If the Mesh has a valid index buffer , this one will be used to draw the Mesh instead of using
    /// only the Vertex buffer .
    HardwareIndexBufferHolder iIndexBuffer ;
    
    /// @brief BoundingBox for this Mesh.
    /// The Bounding box is updated only if the user sets 'iAutomateBoundingBox' to true. If false , the user
    /// has to set himself the bounding box.
    BoundingBox iBoundingBox ;
    
    /// @brief True if the bounding box can be updated when changing one of the buffers.
    /// The bounding box will so be calculated using the 'VertexComponentType::Position' . If the changed buffer
    /// contains any of this component , the bounding box will be recalculated.
    bool iAutomateBoundingBox ;
    
    /// @brief The file this mesh comes from, empty if this mesh was created inside the Engine.
    std::string iOriginalFile ;
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
    /// @brief Loads a Mesh from given file.
    /// This function should always be provided in order to genrically loads
    /// a Mesh from a given file.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder load ( const std::string& name , const std::string& filepath ) const = 0;
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
    /// @brief Retrieves the MeshManager using ResourceManager , then uses
    /// 'MeshManager::iCreateSquare()' .
    /// The squares created with this function are loaded using a private
    /// map with its corresponding surface. This allow us to return the
    /// corresponding surface's square with reload it .
    //////////////////////////////////////////////////////////////////////
    static MeshHolder CreateSquare ( const Surface & surface ) ;
    
public:
    
    POOLED ( Pools::Referenced )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MeshManager ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~MeshManager ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Mesh and convert it to the underlying Rendering API
    /// used.
    //////////////////////////////////////////////////////////////////////
    virtual MeshUser load ( const std::string & name , const std::string & filepath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the first mesh wich origin file corresponds to given
    /// one.
    //////////////////////////////////////////////////////////////////////
    virtual MeshUser findFirstFile ( const std::string & filepath ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Converts the Mesh to the underlying API , if possible.
    /// This function should only use the srcmesh to create buffers on the
    /// GPU memory . Those buffers goes to another mesh. If the returned
    /// mesh is valid , it will be used instead of the loaded mesh.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder iConvertMesh ( const MeshHolder & srcmesh ) const ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns or creates the corresponding square to given surface.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder iCreateSquare ( const Surface & surface ) ;
    
protected:
    
    /// @brief A Map containing every loaded mesh using 'MeshManager::CreateSquare()'.
    std::map < Surface , MeshHolder > iSquares ;
};

/// @brief SpecializedCountedObjectHolder for MeshManager.
typedef SpecializedCountedObjectHolder < MeshManager > MeshManagerHolder ;

GreEndNamespace

#endif
