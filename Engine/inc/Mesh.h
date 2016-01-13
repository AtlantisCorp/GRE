//
//  Mesh.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_Mesh_h
#define GRE_Mesh_h

#include "Resource.h"
#include "Vertex.h"
#include "Face.h"
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"

GRE_BEGIN_NAMESPACE

/// @brief A simple representation of a Mesh.
/* DRAWING
 foreach face
 setMaterial
 begin (geometry face.size)
 foreach vertex
 setColor
 setTexture
 drawPosition
 */
struct DLL_PUBLIC MeshRaw
{
    std::vector<Face> faces;
    Material          parentMaterial;
};
typedef struct MeshRaw MeshRaw;

/* DRAWING
 foreach indexed_face
 setMaterial
 begin (geometry face.size)
 foreach indice
 vertex = vertexs[indice]
 setColor
 setTexture
 drawPosition
 */
struct DLL_PUBLIC IndexedMesh
{
    std::vector<Vertex>      vertex;
    std::vector<IndexedFace> indexedFaces;
    Material                 parentMaterial;
};

/* DRAWING
 foreach material_indexed_face
    setMaterial
    foreach indexedFace
        begin(geometry face.size)
        foreach indice
            vertex = vertex[indice]
            setColor
            setTexture
            drawPosition
 */
struct DLL_PUBLIC MaterialIndexedMesh
{
    std::vector<Vertex>              vertex;
    std::vector<MaterialIndexedFace> materialIndexedFaces;
    Material                         parentMaterial;
};

struct DLL_PUBLIC HardwareIndexBufferBatch
{
    std::vector<HardwareIndexBuffer> batchs;
    static HardwareIndexBufferBatch Null;
};

/* DRAWING
 foreach indexBuffer
    enableIndiceBuffer
    enableVertexBuffer
    setMaterial
    drawBuffer
 */
struct DLL_PUBLIC BufferedMesh
{
    HardwareVertexBuffer     vertexBuffer;
    HardwareIndexBufferBatch indexBuffers;
};

class DLL_PUBLIC MeshPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    enum class Type
    {
        Buffered            = 0x0,
        MaterialIndexed     = 0x1,
        Indexed             = 0x2,
        Raw                 = 0x3
    };
    
    explicit MeshPrivate(const std::string& name, const BufferedMesh& bufmesh);
    explicit MeshPrivate(const std::string& name, const MaterialIndexedMesh& matinmesh);
    explicit MeshPrivate(const std::string& name, const IndexedMesh& inmesh);
    explicit MeshPrivate(const std::string& name, const MeshRaw& rawmesh);
    
    MeshPrivate(const std::string& name);
    virtual ~MeshPrivate();
    
    /// @brief Returns the Vertex Buffer object.
    HardwareVertexBuffer& getVertexBuffer();
    /// @brief Returns the Vertex Buffer object.
    const HardwareVertexBuffer& getVertexBuffer() const;
    
    /// @brief Returns the Index Buffer batch.
    HardwareIndexBufferBatch& getIndexBufferBatch();
    /// @brief Returns the Index Buffer batch.
    const HardwareIndexBufferBatch& getIndexBufferBatch() const;
    
    /// @brief Returns the Mesh Hardware Type.
    Type getType() const;
    
private:
    
    Type _mType;
    
    union {
        BufferedMesh        _mBufMesh;
        MaterialIndexedMesh _mMaterialIndexedMesh;
        IndexedMesh         _mIndexedMesh;
        MeshRaw             _mRawMesh;
    };
};

class DLL_PUBLIC Mesh : public ResourceUser
{
public:
    
    Mesh();
    Mesh(const Mesh& mesh);
    Mesh(Mesh&& mesh);
    explicit Mesh(const ResourceUser& ruser);
    
    ~Mesh();
    
    Mesh& operator = (const Mesh& mesh);
    
    /// @brief Returns the Vertex Buffer object.
    HardwareVertexBuffer& getVertexBuffer();
    /// @brief Returns the Vertex Buffer object.
    const HardwareVertexBuffer& getVertexBuffer() const;
    
    /// @brief Returns the Index Buffer batch.
    HardwareIndexBufferBatch& getIndexBufferBatch();
    /// @brief Returns the Index Buffer batch.
    const HardwareIndexBufferBatch& getIndexBufferBatch() const;
    
    /// @brief Returns the Mesh Hardware Type.
    MeshPrivate::Type getType() const;
    
    static Mesh Null;
    
private:
    
    std::weak_ptr<MeshPrivate> _mMesh;
};

GRE_END_NAMESPACE

#endif
