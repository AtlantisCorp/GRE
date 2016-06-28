//
//  Mesh.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "Mesh.h"

GreBeginNamespace

MeshPrivate::MeshPrivate(const std::string& name, const BufferedMesh& bufmesh)
: Resource(name), iType(MeshPrivate::Type::Buffered), iBufMesh(bufmesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const MaterialIndexedMesh& matinmesh)
: Resource(name), iType(MeshPrivate::Type::MaterialIndexed), iMaterialIndexedMesh(matinmesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const IndexedMesh& mesh)
: Resource(name), iType(MeshPrivate::Type::Indexed), iIndexedMesh(mesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const MeshRaw& mesh)
: Resource(name), iType(MeshPrivate::Type::Raw), iRawMesh(mesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name)
: Resource(name), iType(MeshPrivate::Type::Raw), iRawMesh()
{
    
}

MeshPrivate::~MeshPrivate()
{
    
}


HardwareVertexBuffer& MeshPrivate::getVertexBuffer()
{
    if(iType == Type::Buffered) {
        return iBufMesh.vertexBuffer;
    } else {
        return HardwareVertexBuffer::Null;
    }
}

const HardwareVertexBuffer& MeshPrivate::getVertexBuffer() const
{
    if(iType == Type::Buffered) {
        return iBufMesh.vertexBuffer;
    } else {
        return HardwareVertexBuffer::Null;
    }
}

HardwareIndexBufferBatch& MeshPrivate::getIndexBufferBatch()
{
    if(iType == Type::Buffered) {
        return iBufMesh.indexBuffers;
    } else {
        return HardwareIndexBufferBatch::Null;
    }
}

const HardwareIndexBufferBatch& MeshPrivate::getIndexBufferBatch() const
{
    if(iType == Type::Buffered) {
        return iBufMesh.indexBuffers;
    } else {
        return HardwareIndexBufferBatch::Null;
    }
}

MeshPrivate::Type MeshPrivate::getType() const
{
    return iType;
}

// ---------------------------------------------------------------------------------------------------

Mesh::Mesh(Resource* resource)
: SpecializedResourceUser<MeshPrivate>(resource)
{
    
}

Mesh::Mesh(const Mesh& user)
: SpecializedResourceUser<MeshPrivate>(user)
{
    
}

Mesh::Mesh(const ResourceUser& user)
: SpecializedResourceUser<Gre::MeshPrivate>(user)
{
    
}

Mesh& Mesh::operator=(const Gre::Mesh &mesh)
{
    SpecializedResourceUser<MeshPrivate>::operator=(mesh);
    return *this;
}

Mesh::~Mesh()
{
    
}

HardwareVertexBuffer& Mesh::getVertexBuffer()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getVertexBuffer();
    return HardwareVertexBuffer::Null;
}

const HardwareVertexBuffer& Mesh::getVertexBuffer() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getVertexBuffer();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBufferBatch& Mesh::getIndexBufferBatch()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getIndexBufferBatch();
    return HardwareIndexBufferBatch::Null;
}

const HardwareIndexBufferBatch& Mesh::getIndexBufferBatch() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getIndexBufferBatch();
    return HardwareIndexBufferBatch::Null;
}

MeshPrivate::Type Mesh::getType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getType();
    return MeshPrivate::Type::Raw;
}

HardwareIndexBufferBatch HardwareIndexBufferBatch::Null = HardwareIndexBufferBatch();
Mesh Mesh::Null = Mesh();

GreEndNamespace
