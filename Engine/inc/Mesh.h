//////////////////////////////////////////////////////////////////////
//
//  Mesh.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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

#include "Renderable.h"
#include "ResourceLoader.h"
#include "SpecializedResourceManager.h"

#include "BoundingBox.h"

#include "SoftwareVertexBuffer.h"
#include "SoftwareIndexBuffer.h"

#include "HardwareProgram.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Groups a holder to the shared vertex buffer and to a pointer
/// to the first data in the buffer.
struct HardwareBufferBinding
{
    /// @brief Vertex Buffer to use with the submesh. This buffer is updated
    /// only by the parent mesh.
    HardwareVertexBufferHolder buffer ;

    /// @brief Address of the first vertex data in the buffer.
    uintptr_t first ;

    /// @brief Number of elements to use from the shared buffer.
    size_t elements ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Descripes a part of a group of meshes. This part has a default
/// render material, that should be used by the node to render it if the
/// node does not overwrite it.

class DLL_PUBLIC SubMesh : public Renderable
{
public:

    POOLED ( Pools::Resource )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SubMesh ( const std::string & name = std::string () ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~SubMesh () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setIndexBuffer ( const HardwareIndexBufferHolder & buffer ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const HardwareIndexBufferHolder & getIndexBuffer () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addVertexBuffer ( const HardwareVertexBufferHolder & buffer ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBufferHolderList & getVertexBuffers () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void clearVertexBuffers () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addSharedVertexBuffer (const HardwareVertexBufferHolder & buffer ,
                                uintptr_t first ,
                                size_t elements ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const std::list < HardwareBufferBinding > & getSharedVertexBuffers () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void clearSharedVertexBuffers () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setDefaultMaterial ( const MaterialHolder & material ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const MaterialHolder & getDefaultMaterial () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

protected:

    /// @brief Holds the index buffer to render the sub-mesh.
    HardwareIndexBufferHolder iIndexBuffer ;

    /// @brief Holds local vertex buffers.
    HardwareVertexBufferHolderList iVertexBuffers ;

    /// @brief Holds shared vertex buffers binding informations. Those informations
    /// are required as when the sub-mesh binds the shared vertex buffer , it has to
    /// know how to bind it , more exactly the first data used by the buffer.
    std::list < HardwareBufferBinding > iSharedBindings ;

    /// @brief Default Material used by this submesh. This material should be bound to
    /// the technique , only if the node's material is invalid. This let us customize
    /// the node's material decision : a mesh with more than one submesh holding a
    /// different material for every submesh. If the node's material is invalid, the
    /// default material for the submesh will be used. Else, the node's material is
    /// used. Notes when using the node material it should be bound only once for
    /// every submeshes, but when using the default material binding goes each time
    /// for each submesh.
    MaterialHolder iDefaultMaterial ;
};

/// @brief
typedef Holder < SubMesh > SubMeshHolder ;

//////////////////////////////////////////////////////////////////////
/// @brief Describes a group of submeshes that can be draw on the screen,
/// using an index buffer and one or more shared or local vertex buffers.
///
/// When the mesh is rendered , it will render every submeshes enabled ,
/// taking in account their shared and local bindings. Each submesh should
/// be drawn by the renderer using the draw command . To draw each submeshes,
/// the renderer should use the 'bindNextSubMesh()' to bind the next submesh
/// present. Then , it should use 'unbind()' to unbind the whole group of submeshes.
/// Taking in account a submesh may bind different vertex buffers as another
/// submesh , the bound vertex buffers should unbind themself.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Mesh : public Renderable
{
public:

    POOLED ( Pools::Resource )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh ( const std::string & name = std::string () ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Mesh () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const std::string & getOriginalFilepath () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setOriginalFilepath ( const std::string & filepath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a shared vertex buffer. This buffer can be used with
    /// any submesh.
    //////////////////////////////////////////////////////////////////////
    void addSharedVertexBuffer ( const HardwareVertexBufferHolder & buffer ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the shared vertex buffer at given index.
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBufferHolder getSharedVertexBuffer ( size_t idx ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the list of shared vertex buffers.
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBufferHolderList & getSharedVertexBuffers () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every shared buffers in this Mesh. Notes that if submeshes
    /// uses those buffers , they will be destroyed only when the submeshes are.
    //////////////////////////////////////////////////////////////////////
    void clearSharedVertexBuffers () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a submesh to this mesh.
    //////////////////////////////////////////////////////////////////////
    void addSubMesh ( const SubMeshHolder & submesh ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes submesh at given index.
    //////////////////////////////////////////////////////////////////////
    void removeSubMesh ( size_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every submeshes.
    //////////////////////////////////////////////////////////////////////
    const std::list < SubMeshHolder > & getSubMeshes () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every submeshes present in this mesh.
    //////////////////////////////////////////////////////////////////////
    void clearSubMeshes () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the bounding box of this mesh. Notes that adding
    /// and removing a submesh also changes the boundingbox. You can disable
    /// this behaviour by using 'setBoundingBoxUpdate(false)'.
    //////////////////////////////////////////////////////////////////////
    void setBoundingBox ( const BoundingBox & bbox ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the mesh's bounding box.
    //////////////////////////////////////////////////////////////////////
    const BoundingBox & getBoundingBox () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setBoundingBoxUpdate ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool getBoundingBoxUpdate () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the mesh to the given technique.
    //////////////////////////////////////////////////////////////////////
    virtual void bind ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the next submesh used by the renderer to draw this
    /// mesh. When firstly used , creates a copy of the submesh list and
    /// uses its iterator to iterate through the submeshes.
    /// @return true if the next submesh has been bound , false if it
    /// couldn't or if there are no more submeshes.
    //////////////////////////////////////////////////////////////////////
    virtual bool bindNextSubMesh ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the currently used submesh. This function also
    /// binds the iterator to the next submesh. The iterator is so checked
    /// by 'bindNextSubMesh'.
    //////////////////////////////////////////////////////////////////////
    virtual void unbindCurrentSubMesh ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the mesh. This has for effect to destroy the copied
    /// submesh list , and also use implementation unbind function.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the mesh.
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the current submesh if there is one , or null.
    //////////////////////////////////////////////////////////////////////
    virtual const SubMeshHolder getCurrentSubMesh () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the material for every submeshes.
    //////////////////////////////////////////////////////////////////////
    virtual void setDefaultMaterial ( const MaterialHolder & material ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Implementation bind function. Should bind the mesh to the
    /// given technique.
    //////////////////////////////////////////////////////////////////////
    virtual void iBind ( const TechniqueHolder & technique ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Implementation unbind function. Should unbind the mesh from
    /// the given technique.
    //////////////////////////////////////////////////////////////////////
    virtual void iUnbind ( const TechniqueHolder & technique ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Should bind the submesh vertex buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void iBindSubMesh ( const SubMeshHolder & submesh , const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Should unbind the submesh vertex buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void iUnbindSubMesh ( const SubMeshHolder & submesh , const TechniqueHolder & technique ) const ;

protected:

    /// @brief Holds the original file from where this mesh is from , if
    /// it has one , or empty. This property is only for information purpose.
    std::string iOriginalFile ;

    /// @brief Holds Shared Vertex buffers between the submeshes. Notes only
    /// shared vertex buffers are here. The submeshes are listed with a particular
    /// index buffer , local vertex buffers and a material. Those shared buffers
    /// won't be destroy if no submesh uses them in this mesh. This is done to
    /// make addition of submeshes using those buffers easier.
    HardwareVertexBufferHolderList iSharedVertexBuffers ;

    /// @brief Holds a list of sub-meshes. Those sub-meshes are rendered only if
    /// they are enabled. They each have a material and an index buffer. Notes that
    /// if no index buffer is present, they should render their vertex buffers.
    std::list < SubMeshHolder > iSubMeshes ;

    /// @brief Boundingbox for the Whole group of submeshes.
    BoundingBox iBoundingBox ;

    /// @brief True by default to updates the bounding box with the submeshes present
    /// in the mesh. The bounding box is calculated adding submeshes boundingbox to the
    /// current one. Notes that removing a submesh will recalculate the bounding box.
    bool iBoundingBoxUpdate ;

    /// @brief Holds the current submesh drawed by the renderer. When unbind is called,
    /// this holder should be invalid and not holding any submesh. When 'bindNextSubMesh()'
    /// is called , it returns the next submesh. Notes the submesh list is copied when using
    /// first the 'bindNextSubMesh()' function and this copy is destroyed when using 'unbind()'.
    /// Also notes that the submesh should be unbound using 'unbindCurrentSubMesh()'.
    mutable std::list < SubMeshHolder > :: const_iterator iCurrentSubMesh ;

    /// @brief Holds a copy of the submeshes list used by 'bindNextSubMesh()'. This copy is destroyed
    /// when 'unbind()' is called.
    mutable std::list < SubMeshHolder > iCurrentSubMeshList ;
};

/// @brief Holder for Mesh .
typedef Holder < Mesh > MeshHolder ;

/// @brief SpecializedResourceHolderList for Mesh .
typedef SpecializedResourceHolderList < Mesh > MeshHolderList ;

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
    /// @brief Loads a Mesh from a file. A Mesh file should hold at least
    /// one submesh to be loaded. A Mesh file should always be a group of
    /// submeshes , that can share vertex buffers.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder load ( const std::string& path , const ResourceLoaderOptions & ops ) const = 0;
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
    /// @brief Creates a triangle.
    //////////////////////////////////////////////////////////////////////
    static MeshHolder Triangle ( float sz ) ;

public:

    POOLED ( Pools::Manager )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MeshManager ( ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~MeshManager ( ) noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the first mesh wich origin file corresponds to given
    /// one.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder findFirstFile ( const std::string & filepath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareVertexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBufferHolder createVertexBuffer ( const void* data , size_t sz , const VertexDescriptor& desc ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareIndexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareIndexBufferHolder createIndexBuffer ( const void* data , size_t sz , const IndexDescriptor& desc ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Bundled File. The subdirectory is computed from the
    /// object file extension. For example , an '.obj' file will be in subdir
    /// 'OBJ'. This subdir is given by the correct loader found for this
    /// file. This subdir is then looked for in every bundle to get final path.
    /// Returns the mesh computed from given file.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder loadBundledFile ( const std::string & path , const ResourceLoaderOptions & ops ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a File. The path is considered as relative from the
    /// working directory. Returns the mesh computed from given file.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder loadFile ( const std::string & path , const ResourceLoaderOptions & ops ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a blank mesh with given name.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder loadBlank ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first mesh encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder get ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first mesh encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual const MeshHolder get ( const std::string & name ) const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Implementation to create a new mesh. This mesh should
    /// implement 'Mesh::bind()' and 'Mesh::unbind()' methods.
    //////////////////////////////////////////////////////////////////////
    virtual Mesh* create ( const std::string & name ) const = 0 ;
};

/// @brief Holder for MeshManager.
typedef Holder < MeshManager > MeshManagerHolder ;

GreEndNamespace

#endif
