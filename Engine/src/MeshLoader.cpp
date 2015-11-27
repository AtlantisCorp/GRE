//
//  MeshLoader.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "MeshLoader.h"

GRE_BEGIN_NAMESPACE

MeshLoader::MeshLoader()
{
    
}

MeshLoader::MeshLoader(const MeshLoader& rhs)
{
    
}

MeshLoader::~MeshLoader()
{
    
}

Resource* MeshLoader::load(Resource::Type type, const std::string &name, const std::string &file) const
{
    return nullptr;
}

Resource* MeshLoader::load(Resource::Type type, const std::string &name, const HardwareVertexBuffer &vbuf, const HardwareIndexBufferBatch &ibufs) const
{
    if(!isTypeSupported(type) || vbuf.isInvalid())
        return nullptr;
    
    BufferedMesh rBufMesh;
    rBufMesh.vertexBuffer = vbuf;
    rBufMesh.indexBuffers = ibufs;
    
    MeshPrivate* meshprivate = new MeshPrivate (name, rBufMesh);
    return (Resource*) meshprivate;
}

Resource* MeshLoader::load(Resource::Type type, const std::string &name, const MeshPrivate::Type &meshtype, const Gre::Renderer &renderer, const VertexBatch &vbatch, const MaterialIndexedFaceBatch &ibatch) const
{
    return nullptr;
}

bool MeshLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Mesh;
}

ResourceLoader* MeshLoader::clone() const
{
    return new MeshLoader();
}

GRE_END_NAMESPACE
