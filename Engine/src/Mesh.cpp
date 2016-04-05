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
: Resource(name), _mType(MeshPrivate::Type::Buffered), _mBufMesh(bufmesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const MaterialIndexedMesh& matinmesh)
: Resource(name), _mType(MeshPrivate::Type::MaterialIndexed), _mMaterialIndexedMesh(matinmesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const IndexedMesh& mesh)
: Resource(name), _mType(MeshPrivate::Type::Indexed), _mIndexedMesh(mesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name, const MeshRaw& mesh)
: Resource(name), _mType(MeshPrivate::Type::Raw), _mRawMesh(mesh)
{
    
}

MeshPrivate::MeshPrivate(const std::string& name)
: Resource(name), _mType(MeshPrivate::Type::Raw), _mRawMesh()
{
    
}

MeshPrivate::~MeshPrivate()
{
    
}


HardwareVertexBuffer& MeshPrivate::getVertexBuffer()
{
    if(_mType == Type::Buffered) {
        return _mBufMesh.vertexBuffer;
    } else {
        return HardwareVertexBuffer::Null;
    }
}

const HardwareVertexBuffer& MeshPrivate::getVertexBuffer() const
{
    if(_mType == Type::Buffered) {
        return _mBufMesh.vertexBuffer;
    } else {
        return HardwareVertexBuffer::Null;
    }
}

HardwareIndexBufferBatch& MeshPrivate::getIndexBufferBatch()
{
    if(_mType == Type::Buffered) {
        return _mBufMesh.indexBuffers;
    } else {
        return HardwareIndexBufferBatch::Null;
    }
}

const HardwareIndexBufferBatch& MeshPrivate::getIndexBufferBatch() const
{
    if(_mType == Type::Buffered) {
        return _mBufMesh.indexBuffers;
    } else {
        return HardwareIndexBufferBatch::Null;
    }
}

MeshPrivate::Type MeshPrivate::getType() const
{
    return _mType;
}

Mesh::Mesh()
: ResourceUser(), _mMesh()
{
    
}

Mesh::Mesh(const Mesh& mesh)
: ResourceUser(mesh.lock()), _mMesh(mesh._mMesh)
{
    
}

Mesh::Mesh(Mesh&& mesh)
: ResourceUser(mesh), _mMesh(std::move(mesh._mMesh))
{
    
}

Mesh::Mesh(const ResourceUser& ruser)
: ResourceUser(ruser.lock()), _mMesh(std::dynamic_pointer_cast<MeshPrivate>(ruser.lock()))
{
    
}

Mesh::~Mesh()
{
    
}

Mesh& Mesh::operator = (const Mesh& mesh)
{
    ResourceUser::operator=(mesh);
    _mMesh = mesh._mMesh;
    return *this;
}

HardwareVertexBuffer& Mesh::getVertexBuffer()
{
    auto ptr = _mMesh.lock();
    if(ptr)
        return ptr->getVertexBuffer();
    return HardwareVertexBuffer::Null;
}

const HardwareVertexBuffer& Mesh::getVertexBuffer() const
{
    auto ptr = _mMesh.lock();
    if(ptr)
        return ptr->getVertexBuffer();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBufferBatch& Mesh::getIndexBufferBatch()
{
    auto ptr = _mMesh.lock();
    if(ptr)
        return ptr->getIndexBufferBatch();
    return HardwareIndexBufferBatch::Null;
}

const HardwareIndexBufferBatch& Mesh::getIndexBufferBatch() const
{
    auto ptr = _mMesh.lock();
    if(ptr)
        return ptr->getIndexBufferBatch();
    return HardwareIndexBufferBatch::Null;
}

MeshPrivate::Type Mesh::getType() const
{
    auto ptr = _mMesh.lock();
    if(ptr)
        return ptr->getType();
    return MeshPrivate::Type::Raw;
}

HardwareIndexBufferBatch HardwareIndexBufferBatch::Null = HardwareIndexBufferBatch();
Mesh Mesh::Null = Mesh();

GreEndNamespace
