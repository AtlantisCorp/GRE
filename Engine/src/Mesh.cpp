//////////////////////////////////////////////////////////////////////
//
//  Mesh.cpp
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

#include "Mesh.h"

// Because of 'MeshManager::CreateSquare()' , we must use the ResourceManager to retrieve the
// MeshManager in place .
#include "ResourceManager.h"

GreBeginNamespace

Mesh::Mesh ( )
: Gre::Resource( )
, iIndexBuffer ( nullptr ) , iBoundingBox ( )
, iOriginalFile ( "" ) , iDefaultMaterial(nullptr)
{
    
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

const MaterialHolder & Mesh::getPreferredMaterial() const
{
    GreAutolock ; return iDefaultMaterial ;
}

void Mesh::setPreferredMaterial(const MaterialHolder &material)
{
    GreAutolock ; iDefaultMaterial = material ;
}

// ---------------------------------------------------------------------------------------------------

MeshLoader::MeshLoader ()
{
    
}

MeshLoader::~MeshLoader () noexcept ( false )
{
    
}

// ---------------------------------------------------------------------------------------------------

MeshUser MeshManager::Cube(float sz)
{
    MeshManagerHolder holder = ResourceManager::Get().getMeshManager() ;
    if ( holder.isInvalid() ) return MeshUser ( nullptr ) ;
    
    std::vector<float> vertexdata = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,   sz,
        0.0f,   sz,   sz,
        0.0f,   sz, 0.0f,
        
          sz, 0.0f, 0.0f,
          sz, 0.0f,   sz,
          sz,   sz,   sz,
          sz,   sz, 0.0f
    };
    
    std::vector<unsigned short> indexdata = {
        0, 1, 2, 2, 3, 0,
        0, 4, 7, 7, 3, 0,
        0, 1, 5, 5, 4, 0,
        4, 5, 6, 6, 7, 4,
        1, 5, 6, 6, 2, 1,
        3, 7, 6, 6, 2, 3
    };
    
    VertexDescriptor desc ;
    desc.addComponent(VertexComponentType::Position) ;
    HardwareVertexBufferHolder vbuf = holder -> createVertexBuffer(& vertexdata[0],
                                                                   vertexdata.size() * sizeof(float),
                                                                   desc) ;
    vbuf -> setEnabled(true) ;
    HardwareVertexBufferHolderList vbuflist ; vbuflist.add(vbuf) ;
    
    IndexDescriptor idesc ;
    idesc.setMode(IndexDrawmode::Triangles) ;
    idesc.setType(IndexType::UnsignedShort) ;
    HardwareIndexBufferHolder ibuf = holder -> createIndexBuffer(& indexdata[0],
                                                                 indexdata.size() * sizeof(unsigned short),
                                                                 idesc) ;
    ibuf -> setEnabled(true) ;
    
    BoundingBox bbox = BoundingBox({0.0f, 0.0f, 0.0f}, {sz, sz, sz}) ;
    
    MeshHolder mesh = MeshHolder ( new Mesh("Cube") ) ;
    mesh -> setBoundingBox(bbox) ;
    mesh -> setVertexBuffers(vbuflist) ;
    mesh -> setIndexBuffer(ibuf) ;
    
    holder -> iHolders.add(mesh) ;
    return MeshUser ( mesh ) ;
}

MeshUser MeshManager::Triangle(float sz)
{
    MeshManagerHolder holder = ResourceManager::Get().getMeshManager() ;
    if ( holder.isInvalid() ) return MeshUser ( nullptr ) ;
    
    float vertex [] = {
        - sz , 0.0f , 0.0f ,
          sz , 0.0f , 0.0f ,
        0.0f ,   sz , 0.0f
    } ;
    
    unsigned int indices [] = { 0, 1, 2 } ;
    
    VertexDescriptor vdesc ; vdesc.addComponent(VertexComponentType::Position) ;
    HardwareVertexBufferHolder vbuf = holder -> createVertexBuffer(vertex, sizeof(vertex), vdesc) ;
    HardwareVertexBufferHolderList vbuflist ; vbuflist.add(vbuf) ;
    vbuf->setEnabled(true);
    
    IndexDescriptor idesc ; idesc.setMode(IndexDrawmode::Triangles) ;
    idesc.setType(IndexType::UnsignedInteger) ;
    HardwareIndexBufferHolder ibuf = holder -> createIndexBuffer(indices, sizeof(indices), idesc) ;
    ibuf->setEnabled(true) ;
    
    MeshHolder mesh = MeshHolder ( new Mesh("Triangle") ) ;
    mesh -> setVertexBuffers(vbuflist) ;
    mesh -> setIndexBuffer(ibuf) ;
    
    holder -> iHolders.add(mesh) ;
    return MeshUser ( mesh ) ;
}

// ---------------------------------------------------------------------------------------------------

MeshManager::MeshManager ( )
: SpecializedResourceManager ( )
{
    
}

MeshManager::~MeshManager ( ) noexcept ( false )
{
    
}

std::vector < MeshUser > MeshManager::load ( const std::string & name , const std::string & filepath )
{
    GreAutolock ;
    
    // See if this file wasn't already loaded. If true, we returns the meshes
    // that were already loaded.
    
    {
        std::vector < MeshUser > meshes ;
        
        for ( auto it : iHolders )
        {
            if ( !it.isInvalid() ) {
                if ( it->getOriginalFilepath() == filepath ) {
                    meshes.push_back(MeshUser(it));
                }
            }
        }
        
        if ( !meshes.empty() )
            return meshes ;
    }
    
    // Find a correct loader to load this file.
    
    MeshLoader* loader = iFindBestLoader(filepath) ;
    if ( !loader ) {
#ifdef GreIsDebugMode
        GreDebug("[WARN] No loader found for Mesh '") << name << "'." << Gre::gendl ;
#endif
        return std::vector < MeshUser > () ;
    }
    
    // Loads the file.
    
    MeshHolderList meshes = loader->load(name, filepath);
    if ( meshes.empty() ) {
#ifdef GreIsDebugMode
        GreDebug("[WARN] Can't load '") << filepath << "'." << Gre::gendl ;
#endif
        return std::vector < MeshUser > () ;
    }
    
    std::vector < MeshUser > retvalue ;
    
    iHolders.insert(iHolders.end(), meshes.begin(), meshes.end());
    retvalue.insert(retvalue.begin(), meshes.begin(), meshes.end());
    
#ifdef GreIsDebugMode
    GreDebug("[INFO] Loaded ") << retvalue.size() << " meshes." << Gre::gendl ;
#endif
    
    return retvalue ;
}

MeshUser MeshManager::findFirstFile(const std::string &filepath)
{
    for ( auto mesh : iHolders ) {
        if ( !mesh.isInvalid() && mesh->getOriginalFilepath() == filepath )
            return mesh ;
    }
    
    return MeshUser ( nullptr ) ;
}

GreEndNamespace
