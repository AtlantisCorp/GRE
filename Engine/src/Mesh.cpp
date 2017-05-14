//////////////////////////////////////////////////////////////////////
//
//  Mesh.cpp
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

#include "Mesh.h"

#include "ResourceManager.h"
#include "ObjMeshLoader.h"

GreBeginNamespace

// ---------------------------------------------------------------------------------------------------

SubMesh::SubMesh ( const std::string & name )
: Gre::Renderable ( name )
{

}

SubMesh::~SubMesh () noexcept ( false )
{

}

void SubMesh::setIndexBuffer ( const HardwareIndexBufferHolder & buffer )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check if index buffer is different from our.

    if ( buffer == iIndexBuffer )
    return ;

    //////////////////////////////////////////////////////////////////////
    // The index buffer should always receive updates events. So , clear
    // the current index buffer and adds the new buffer to the listeners.

    removeListener (iIndexBuffer ) ;
    iIndexBuffer = buffer ;
    addFilteredListener ( buffer , { EventType::Update } ) ;
}

const HardwareIndexBufferHolder & SubMesh::getIndexBuffer () const
{
    GreAutolock ; return iIndexBuffer ;
}

void SubMesh::addVertexBuffer ( const HardwareVertexBufferHolder & buffer )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // If buffer is invalid , we don't add it.

    if ( buffer.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Checks if the vertex buffer is not already in the list.

    auto check = std::find ( iVertexBuffers.begin() , iVertexBuffers.end() , buffer ) ;
    if ( check != iVertexBuffers.end() ) return ;

    //////////////////////////////////////////////////////////////////////
    // Adds the buffer also to the listeners.

    iVertexBuffers.push_back ( buffer ) ;
    addFilteredListener ( buffer , { EventType::Update } ) ;
}

const HardwareVertexBufferHolderList & SubMesh::getVertexBuffers () const
{
    GreAutolock ; return iVertexBuffers ;
}

void SubMesh::clearVertexBuffers ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Remove each vertex buffers from listeners.

    for ( auto buffer : iVertexBuffers )
    removeListener ( buffer ) ;

    //////////////////////////////////////////////////////////////////////
    // Clears the list.

    iVertexBuffers.clear () ;
}

void SubMesh::addSharedVertexBuffer (const HardwareVertexBufferHolder & buffer ,
                                     uintptr_t first ,
                                     size_t elements)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check if buffer is not already present.

    for ( auto check : iSharedBindings )
    if ( check.buffer == buffer )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Adds the binding to the shared list but not to the listeners list.
    // The updates events to this buffer will come from the mesh.

    HardwareBufferBinding binding ;
    binding.buffer = buffer ;
    binding.first = first ;
    binding.elements = elements ;
    iSharedBindings.push_back ( binding ) ;
}

const std::list < HardwareBufferBinding > & SubMesh::getSharedVertexBuffers () const
{
    GreAutolock ; return iSharedBindings ;
}

void SubMesh::clearSharedVertexBuffers ()
{
    GreAutolock ; iSharedBindings.clear () ;
}

void SubMesh::setDefaultMaterial ( const MaterialHolder & material )
{
    GreAutolock ; iDefaultMaterial = material ;
}

const MaterialHolder & SubMesh::getDefaultMaterial () const
{
    GreAutolock ; return iDefaultMaterial ;
}

void SubMesh::clear ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Destroys the index buffer and remove it from listeners.

    if ( !iIndexBuffer.isInvalid() )
    removeListener ( iIndexBuffer ) ;
    iIndexBuffer.clear () ;

    //////////////////////////////////////////////////////////////////////
    // Destroys local and shared vertex buffers.

    clearVertexBuffers () ;
    clearSharedVertexBuffers () ;

    //////////////////////////////////////////////////////////////////////
    // Destroys material.

    iDefaultMaterial.clear () ;

    //////////////////////////////////////////////////////////////////////
    // Finish with renderable.

    Renderable::clear () ;
}

// ---------------------------------------------------------------------------------------------------

Mesh::Mesh ( const std::string & name )
: Gre::Renderable ( name )
{
    iBoundingBoxUpdate = true ;
}

Mesh::~Mesh() noexcept ( false )
{

}

const std::string & Mesh::getOriginalFilepath () const
{
    GreAutolock ; return iOriginalFile ;
}

void Mesh::setOriginalFilepath ( const std::string & filepath )
{
    GreAutolock ; iOriginalFile = filepath ;
}

void Mesh::addSharedVertexBuffer ( const HardwareVertexBufferHolder & buffer )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Skip this buffer if invalid.

    if ( buffer.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Check this buffer is not already in the list.

    auto check = std::find ( iSharedVertexBuffers.begin() , iSharedVertexBuffers.end() , buffer ) ;
    if ( check != iSharedVertexBuffers.end() ) return ;

    //////////////////////////////////////////////////////////////////////
    // Adds this buffer to the list. Notes this buffer is updated by this
    // mesh , so adds it to the listeners.

    iSharedVertexBuffers.push_back ( buffer ) ;
    addFilteredListener ( buffer , { EventType::Update } ) ;
}

const HardwareVertexBufferHolder Mesh::getSharedVertexBuffer ( size_t idx ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check index validity.

    if ( idx >= iSharedVertexBuffers.size() )
    return HardwareVertexBufferHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Returns the indexed shared vertex buffer.

    auto it = iSharedVertexBuffers.begin() ;
    std::advance ( it , idx ) ;

    return *it ;
}

const HardwareVertexBufferHolderList & Mesh::getSharedVertexBuffers () const
{
    GreAutolock ; return iSharedVertexBuffers ;
}

void Mesh::clearSharedVertexBuffers ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Removes all buffers from listeners.

    for ( auto buffer : iSharedVertexBuffers )
    removeListener ( buffer ) ;

    //////////////////////////////////////////////////////////////////////
    // Clears the buffers.

    iSharedVertexBuffers.clear () ;
}

void Mesh::addSubMesh ( const SubMeshHolder & submesh )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check submesh validity.

    if ( submesh.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Check this submesh is not already in the mesh.

    auto check = std::find ( iSubMeshes.begin() , iSubMeshes.end() , submesh ) ;
    if ( check != iSubMeshes.end() ) return ;

    //////////////////////////////////////////////////////////////////////
    // Adds this submesh , and makes him listens this mesh for updates.

    iSubMeshes.push_back ( submesh ) ;
    addFilteredListener ( submesh , { EventType::Update } ) ;
}

void Mesh::removeSubMesh ( size_t idx )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check index validity.

    if ( idx >= iSubMeshes.size() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Erases the submesh. Also removes it from listeners.

    auto it = iSubMeshes.begin() ;
    std::advance ( it , idx ) ;

    removeListener ( *it ) ;
    iSubMeshes.erase ( it ) ;
}

const std::list < SubMeshHolder > & Mesh::getSubMeshes () const
{
    GreAutolock ; return iSubMeshes ;
}

void Mesh::clearSubMeshes ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Removes every submeshes from listening.

    for ( auto submesh : iSubMeshes )
    removeListener ( submesh ) ;

    //////////////////////////////////////////////////////////////////////
    // Clears the submeshes.

    iSubMeshes.clear () ;
}

void Mesh::setBoundingBox ( const BoundingBox & bbox )
{
    GreAutolock ; iBoundingBox = bbox ;
}

const BoundingBox & Mesh::getBoundingBox ( ) const
{
    GreAutolock ; return iBoundingBox ;
}

void Mesh::setBoundingBoxUpdate ( bool value )
{
    GreAutolock ; iBoundingBoxUpdate = value ;
}

bool Mesh::getBoundingBoxUpdate () const
{
    GreAutolock ; return iBoundingBoxUpdate ;
}

void Mesh::bind ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Uses implementation specific binding.

    iBind ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Copies the submeshes list in order to use them.

    iCurrentSubMeshList = iSubMeshes ;
    iCurrentSubMesh = iCurrentSubMeshList.begin () ;
}

bool Mesh::bindNextSubMesh ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check the submesh iterator validity. If iterator is end() , this means
    // either the submesh list is empty either the list has been passed through ,
    // so we should returns false.

    if ( iCurrentSubMesh == iCurrentSubMeshList.end() )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Tries to bind the submesh pointed by the iterator.

    iBindSubMesh ( *iCurrentSubMesh , technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Returns success.

    return true ;
}

void Mesh::unbindCurrentSubMesh ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check iterator validity.

    if ( iCurrentSubMesh == iCurrentSubMeshList.end() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Tries to unbind the submesh.

    iUnbindSubMesh ( *iCurrentSubMesh , technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Sets the next iterator.

    std::advance ( iCurrentSubMesh , 1 ) ;
}

void Mesh::unbind ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Calls implementation specific unbinding.

    iUnbind ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Reset the submesh list.

    iCurrentSubMeshList.clear () ;
    iCurrentSubMesh = iCurrentSubMeshList.end () ;
}

void Mesh::clear ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Clear vertex buffers and submeshes.

    clearSharedVertexBuffers () ;
    clearSubMeshes () ;

    //////////////////////////////////////////////////////////////////////
    // Clear properties.

    iOriginalFile = std::string () ;
    iBoundingBox.clear () ;
    iBoundingBoxUpdate = true ;
    iCurrentSubMeshList.clear () ;
    iCurrentSubMesh = iCurrentSubMeshList.end () ;

    //////////////////////////////////////////////////////////////////////
    // Calls parent clear.

    Renderable::clear () ;
}

const SubMeshHolder Mesh::getCurrentSubMesh () const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check iterator validity.

    if ( iCurrentSubMesh == iCurrentSubMeshList.end () )
    return SubMeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Returns the submesh.

    return *iCurrentSubMesh ;
}

void Mesh::setDefaultMaterial ( const MaterialHolder & material )
{
    GreAutolock ;

    if ( material.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Loop through submeshes to set the material.

    for ( auto submesh : iSubMeshes )
    submesh -> setDefaultMaterial ( material ) ;
}

void Mesh::iBindSubMesh ( const SubMeshHolder & submesh , const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks submesh and technique validity.

    if ( submesh.isInvalid() || technique.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Gets the program bound to the technique and checks it is binded.

    auto program = technique -> getHardwareProgram () ;
    if ( program.isInvalid() ) return ;

    if ( !program -> binded () )
    return ;

    //////////////////////////////////////////////////////////////////////
    // For each vertex buffer , binds them to the program.

    for ( auto buffer : submesh -> getVertexBuffers () )
    {
        if ( buffer.isInvalid() )
        continue ;

        if ( !buffer->isEnabled() || !buffer->getSize() )
        continue ;

        //////////////////////////////////////////////////////////////////////
        // Binds the buffer in order to set attributes to the program.

        buffer -> bind () ;
        auto descriptor = buffer -> getVertexDescriptor () ;

        //////////////////////////////////////////////////////////////////////
        // Binds the attributes to the program.

        for ( auto component : descriptor.getComponents () )
        {
            program -> setVertexAttrib (technique -> getAttribName ( component.alias ) ,
                                        component.elements ,
                                        component.type ,
                                        component.normalize ,
                                        descriptor.getStride(component) ,
                                        (void*) (buffer -> getData() + descriptor.getOffset(component)) ) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Unbinds the buffer.

        buffer -> unbind () ;
    }

    //////////////////////////////////////////////////////////////////////
    // Do the same thing , but for shared vertex buffers.

    for ( auto binding : submesh -> getSharedVertexBuffers () )
    {
        auto buffer = binding.buffer ;

        if ( buffer.isInvalid() )
        continue ;

        if ( !buffer->isEnabled() || !buffer->getSize() )
        continue ;

        //////////////////////////////////////////////////////////////////////
        // Binds the buffer in order to set attributes to the program.

        buffer -> bind () ;
        auto descriptor = buffer -> getVertexDescriptor () ;

        //////////////////////////////////////////////////////////////////////
        // Binds the attributes to the program.

        for ( auto component : descriptor.getComponents () )
        {
            program -> setVertexAttrib (technique -> getAttribName ( component.alias ) ,
                                        binding.elements ,
                                        component.type ,
                                        component.normalize ,
                                        descriptor.getStride(component) ,
                                        (void*) (buffer -> getData() + descriptor.getOffset(component) + binding.first) ) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Unbinds the buffer.

        buffer -> unbind () ;
    }
}

void Mesh::iUnbindSubMesh ( const SubMeshHolder & submesh , const TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks program validity and disable every Vertex Attributes.

    auto program = technique -> getHardwareProgram () ;
    if ( program.isInvalid() ) return ;

    if ( !program -> binded () )
    return ;

    program -> disableVertexAttribs () ;
}

// ---------------------------------------------------------------------------------------------------

MeshLoader::MeshLoader ()
{

}

MeshLoader::~MeshLoader () noexcept ( false )
{

}

// ---------------------------------------------------------------------------------------------------

MeshHolder MeshManager::Triangle(float sz)
{
    MeshManagerHolder holder = ResourceManager::Get()->getMeshManager() ;
    if ( holder.isInvalid() ) return MeshHolder ( nullptr ) ;

    float vertex [] = {
        - sz , 0.0f , 0.0f ,
          sz , 0.0f , 0.0f ,
        0.0f ,   sz , 0.0f
    } ;

    unsigned int indices [] = { 0, 2, 1 } ;

    //////////////////////////////////////////////////////////////////////
    // Vertex Descriptor.

    VertexDescriptor vdesc ;

    VertexAttribComponent position ;
    position.alias = VertexAttribAlias::Position ;
    position.elements = 3 ;
    position.normalize = false ;
    position.size = 3 * sizeof ( float ) ;
    position.type = VertexAttribType::Float ;
    vdesc.addComponent(position) ;

    //////////////////////////////////////////////////////////////////////
    // Vertex Buffer.

    HardwareVertexBufferHolder vbuf = holder -> createVertexBuffer(vertex, sizeof(vertex), vdesc) ;
    HardwareVertexBufferHolderList vbuflist ; vbuflist.add(vbuf) ;
    vbuf->setEnabled(true);

    //////////////////////////////////////////////////////////////////////
    // Index Descriptor.

    IndexDescriptor idesc ;
    idesc.setMode(IndexDrawmode::Triangles) ;
    idesc.setType(IndexType::UnsignedInteger) ;

    //////////////////////////////////////////////////////////////////////
    // Index Buffer.

    HardwareIndexBufferHolder ibuf = holder -> createIndexBuffer(indices, sizeof(indices), idesc) ;
    ibuf->setEnabled(true) ;

    //////////////////////////////////////////////////////////////////////
    // Resulting Mesh.

    MeshHolder mesh = holder -> loadBlank ( "mesh.triangle" ) ;

    SubMeshHolder submesh = new SubMesh ( "mesh.triangle.submesh" ) ;
    submesh -> addVertexBuffer ( vbuf ) ;
    submesh -> setIndexBuffer ( ibuf ) ;
    mesh -> addSubMesh ( submesh ) ;

    BoundingBox bbox ( { -sz , 0.0 , 0.0 } , { sz , sz , 0.0 } ) ;
    mesh -> setBoundingBox ( bbox ) ;

    //////////////////////////////////////////////////////////////////////
    // Registers the resulting mesh.

    holder -> iHolders.add(mesh) ;
    return mesh ;
}

// ---------------------------------------------------------------------------------------------------

MeshManager::MeshManager ( )
: SpecializedResourceManager ( )
{
    getFactory().registers( "mesh.loader.obj" , new ObjMeshLoader () ) ;
}

MeshManager::~MeshManager ( ) noexcept ( false )
{

}

MeshHolder MeshManager::findFirstFile(const std::string &filepath)
{
    for ( auto mesh : iHolders ) {
        if ( !mesh.isInvalid() && mesh->getOriginalFilepath() == filepath )
        return mesh ;
    }

    return MeshHolder ( nullptr ) ;
}

MeshHolder MeshManager::loadBundledFile(const std::string &path, const ResourceLoaderOptions &ops)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks input values.

    if ( path.empty() )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Tries to find a suitable loader for the given file.

    MeshLoader * loader = iFindBestLoader(path) ;

    if ( !loader )
    return 0 ;

    //////////////////////////////////////////////////////////////////////
    // Tries to get the correct subdirectory and find the real file path.

    std::string subdir = loader -> getDirectory () ;
    std::string bundlepath = subdir + Platform::GetSeparator() + path ;

    if ( bundlepath.empty() )
    return MeshHolder ( nullptr ) ;

    std::string realpath = ResourceManager::Get() -> findBundledFile(ResourceType::Mesh, bundlepath) ;

    if ( realpath.empty() )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Now tries to load the file as a normal file.

    return loadFile ( realpath , ops ) ;
}

MeshHolder MeshManager::loadFile(const std::string &path, const ResourceLoaderOptions &ops)
{
    //////////////////////////////////////////////////////////////////////
    // Check input values.

    if ( path.empty() )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Tries to find a suitable loader for the given file.

    MeshLoader * loader = iFindBestLoader(path) ;

    if ( !loader )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Tries to load the file using the loader.

    MeshHolder mesh = loader -> load ( path , ops ) ;

    if ( mesh.isInvalid() )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Registers and returns.

    iHolders.push_back ( mesh ) ;
    addFilteredListener ( mesh , { EventType::Update } ) ;

    return mesh ;
}

MeshHolder MeshManager::loadBlank ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks name is not empty.

    if ( name.empty() )
    return MeshHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the new mesh , registers it to listeners and returns it.

    MeshHolder mesh = create ( name ) ;

    iHolders.push_back ( mesh ) ;
    addFilteredListener ( mesh , { EventType::Update } ) ;

    return mesh ;
}

MeshHolder MeshManager::get(const std::string &name)
{
    GreAutolock ; return findFirstHolder(name) ;
}

const MeshHolder MeshManager::get(const std::string &name) const
{
    GreAutolock ; return findFirstHolder(name) ;
}

GreEndNamespace
