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

MeshBinder::~MeshBinder()
{
    
}

// ---------------------------------------------------------------------------------------------------

Mesh::Mesh ( )
: Gre::Resource( )
, iIndexBuffer ( nullptr ) , iBoundingBox ( )
, iOriginalFile ( "" ) , iDefaultMaterial(nullptr)
{
    iBinder = nullptr ;
}

Mesh::Mesh ( const std::string & name )
: Gre::Resource( ResourceIdentifier::New() , name )
, iIndexBuffer( nullptr ) , iBoundingBox ( )
, iOriginalFile( "" ) , iDefaultMaterial(nullptr)
{

}

Mesh::~Mesh() noexcept ( false )
{

}

void Mesh::setVertexBuffers(const HardwareVertexBufferHolderList &attributes)
{
    GreAutolock ; iVertexBuffers = attributes ;
}

const HardwareVertexBufferHolderList & Mesh::getVertexBuffers() const
{
    GreAutolock ; return iVertexBuffers ;
}

void Mesh::setIndexBuffer(const HardwareIndexBufferHolder &buffer)
{
    GreAutolock ; iIndexBuffer = buffer ;
}

const HardwareIndexBufferHolder & Mesh::getIndexBuffer() const
{
    GreAutolock ; return iIndexBuffer ;
}

void Mesh::unload ()
{
    GreAutolock ;

    iVertexBuffers.clear() ;
    iIndexBuffer.clear() ;
    iBoundingBox.clear() ;
    iDefaultMaterial.clear() ;

    Resource::unload() ;
}

void Mesh::clearVertexBuffers()
{
    GreAutolock ; iVertexBuffers.clear() ;
}

void Mesh::clearIndexBuffer()
{
    GreAutolock ; iIndexBuffer.clear() ;
}

void Mesh::setBoundingBox ( const BoundingBox & bbox )
{
    GreAutolock ; iBoundingBox = bbox ;
}

const BoundingBox & Mesh::getBoundingBox ( ) const
{
    GreAutolock ; return iBoundingBox ;
}

const std::string & Mesh::getOriginalFilepath () const
{
    GreAutolock ; return iOriginalFile ;
}

void Mesh::setOriginalFilepath ( const std::string & filepath )
{
    GreAutolock ; iOriginalFile = filepath ;
}

const MaterialHolder & Mesh::getDefaultMaterial() const
{
    GreAutolock ; return iDefaultMaterial ;
}

MaterialHolder & Mesh::getDefaultMaterial()
{
    GreAutolock ; return iDefaultMaterial ;
}

void Mesh::setDefaultMaterial(const MaterialHolder &material)
{
    GreAutolock ; iDefaultMaterial = material ;
}

void Mesh::use ( const TechniqueHolder& technique ) const
{
    GreAutolock ;
    
    //////////////////////////////////////////////////////////////////////
    // The mesh'es default material is already bound by the node's use
    // function. In fact , the node use the mesh material if no other material
    // has been given to the node. More , if this mesh does not contain any
    // material , the node will keep a blank material.
    
    //////////////////////////////////////////////////////////////////////
    // Binds Custom parameters.

    TechniqueParamBinder::use ( technique ) ;
}

void Mesh::bind ( const TechniqueHolder & technique ) const
{
    GreAutolock ;
    
    if ( !iBinder )
    return ;
    
    //////////////////////////////////////////////////////////////////////
    // Uses the binder implementation to make some driver - specific adjusts.
    
    iBinder -> bind ( this , technique ) ;
    
    //////////////////////////////////////////////////////////////////////
    // Notes that as in OpenGl , and maybe other APIs , we don't need to
    // updates the bound values in , for example , a VAO if the datas did not
    // changed , we use that specific function to know if we have to update the
    // VAO's values.
    
    if ( !iBinder->needUpdate() && !isAnyBufferDirty() )
    return ;
    
    //////////////////////////////////////////////////////////////////////
    // Binds Vertex Descriptors Attributes to the technique. This step is
    // fundamental before rendering the object through the shader program.
    // Notes that a HardwareProgram must be valid in the technique.
    
    if ( !technique->getHardwareProgram().isInvalid() )
    {
        const HardwareProgramHolder & program = technique -> getHardwareProgram() ;
        
        for ( auto buffer : iVertexBuffers )
        {
            if ( !buffer.isInvalid() )
            {
                if ( buffer->isEnabled() && buffer->getSize() )
                {
                    const VertexDescriptor & vdesc = buffer -> getVertexDescriptor() ;
                    buffer -> bind() ;
                    
                    for ( auto component : vdesc.getComponents() )
                    {
                        program -> setVertexAttrib (technique -> getAttribName ( component.alias ) ,
                                                    component.elements ,
                                                    component.type ,
                                                    component.normalize ,
                                                    vdesc.getStride(component) ,
                                                    (void*) (buffer -> getData() + vdesc.getOffset(component)) ) ;
                    }
                    
                    buffer -> unbind () ;
                }
            }
        }
        
        //////////////////////////////////////////////////////////////////////
        // Reset the buffers dirty flag.
        
        if ( !iIndexBuffer.isInvalid() )
        iIndexBuffer -> clean () ;
        
        for ( auto buffer : iVertexBuffers )
            if ( !buffer.isInvalid() )
            buffer -> clean () ;
    }
}

void Mesh::unbind ( const TechniqueHolder & technique ) const
{
    GreAutolock ;
    
    if ( !iBinder )
        return ;
    
    //////////////////////////////////////////////////////////////////////
    // Uses the binder implementation to make some driver - specific adjusts.
    
    iBinder -> unbind ( this , technique ) ;
}

void Mesh::setBinder(Gre::MeshBinder *binder)
{
    GreAutolock ; iBinder = binder ;
}

bool Mesh::isAnyBufferDirty () const
{
    GreAutolock ;
    
    if ( !iIndexBuffer.isInvalid() )
    if ( iIndexBuffer -> isDirty() )
    return true ;
    
    for ( auto buffer : iVertexBuffers )
    {
        if ( !buffer.isInvalid() )
        if ( buffer -> isDirty() )
        return true ;
    }
    
    return false ;
}

// ---------------------------------------------------------------------------------------------------

MeshLoader::MeshLoader ()
{

}

MeshLoader::~MeshLoader () noexcept ( false )
{

}

// ---------------------------------------------------------------------------------------------------

MeshUser MeshManager::Triangle(float sz)
{
    MeshManagerHolder holder = ResourceManager::Get()->getMeshManager() ;
    if ( holder.isInvalid() ) return MeshUser ( nullptr ) ;

    float vertex [] = {
        - sz , 0.0f , 0.0f ,
          sz , 0.0f , 0.0f ,
        0.0f ,   sz , 0.0f
    } ;

    unsigned int indices [] = { 0, 1, 2 } ;
    
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
    // Resulting Mehs.

    MeshHolder mesh = MeshHolder ( new Mesh("Triangle") ) ;
    mesh -> setVertexBuffers(vbuflist) ;
    mesh -> setIndexBuffer(ibuf) ;
    
    //////////////////////////////////////////////////////////////////////
    // Registers the resulting mesh.

    holder -> iHolders.add(mesh) ;
    return MeshUser ( mesh ) ;
}

// ---------------------------------------------------------------------------------------------------

MeshManager::MeshManager ( )
: SpecializedResourceManager ( )
{
    getFactory().registers("OBJ File Loader", new ObjMeshLoader ()) ;
}

MeshManager::~MeshManager ( ) noexcept ( false )
{

}

MeshUser MeshManager::findFirstFile(const std::string &filepath)
{
    for ( auto mesh : iHolders ) {
        if ( !mesh.isInvalid() && mesh->getOriginalFilepath() == filepath )
            return mesh ;
    }

    return MeshUser ( nullptr ) ;
}

int MeshManager::loadBundledFile(const std::string &path, const ResourceLoaderOptions &ops)
{
    GreAutolock ;
    
    //////////////////////////////////////////////////////////////////////
    // Checks input values.
    
    if ( path.empty() )
    return 0 ;
    
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
    return 0 ;
    
    std::string realpath = ResourceManager::Get() -> findBundledFile(ResourceType::Mesh, bundlepath) ;
    
    if ( realpath.empty() )
    return 0 ;
    
    //////////////////////////////////////////////////////////////////////
    // Now tries to load the file as a normal file.
    
    return loadFile ( realpath , ops ) ;
}

int MeshManager::loadFile(const std::string &path, const ResourceLoaderOptions &ops)
{
    //////////////////////////////////////////////////////////////////////
    // Check input values.
    
    if ( path.empty() )
    return 0 ;
    
    //////////////////////////////////////////////////////////////////////
    // Tries to find a suitable loader for the given file.
    
    MeshLoader * loader = iFindBestLoader(path) ;
    
    if ( !loader )
    return 0 ;
    
    //////////////////////////////////////////////////////////////////////
    // Tries to load the file using the loader.
    
    MeshHolderList meshes = loader -> load ( path , ops ) ;
    
    if ( meshes.empty() )
    return 0 ;
    
    //////////////////////////////////////////////////////////////////////
    // Registers and returns.
    
    for ( auto mesh : meshes )
    {
        MeshBinder* binder = loadBinder() ;
        
        if ( !binder )
        continue ;
        
        mesh -> setBinder(binder) ;
        iHolders.add(mesh) ;
    }
    
    return meshes.size() ;
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
