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
#include "Vertex.h"
#include "Face.h"
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"

GreBeginNamespace

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

//////////////////////////////////////////////////////////////////////
/// @brief Defines a Mesh Resource Object.
///
/// The Mesh Object is responsible for its own drawing. You can subclass
/// the Mesh::onUpdateEvent() method or the Mesh::onRenderEvent() in order
/// to customize the Mesh.
///
/// Data is stored in the Mesh object as the following.
/// When loading a Mesh from the MeshLoader, your Mesh object will only
/// fills the SoftwareBuffer properties.
///
/// When the Renderer draw the Mesh, if the property Mesh::iUseHardwareBuffers
/// is true, then the Renderer will creates some HardwareBuffer, upload
/// data from the SoftwareBuffer's, and finally will release the SoftwareBuffer's
/// using Mesh::clearSoftwareBuffers().
///
/// This allows the user to separate the loading of the Mesh, which is
/// independent from the Rendering API, to the creation of HardwareBuffer's,
/// which is dependent from the APi.
///
/// Don't forget also to sort the Index Buffers by materials. This is much
/// more efficient.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC MeshPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MeshPrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~MeshPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the SoftwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const SoftwareVertexBuffer getSoftwareVertexBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the SoftwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void setSoftwareVertexBuffer(const SoftwareVertexBuffer& softvertexbuffer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the SoftwareIndexBuffers.
    //////////////////////////////////////////////////////////////////////
    virtual const SoftwareIndexBufferList getSoftwareIndexBuffers() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a SoftwareIndexBuffer to the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addSoftwareIndexBuffer(const SoftwareIndexBuffer& softindexbuffer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Vertex Buffer object.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareVertexBuffer getVertexBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void setVertexBuffer(const HardwareVertexBuffer& vertexbuffer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Index Buffer batch.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareIndexBufferList getIndexBuffers() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds an HardwareIndexBuffer to the list.
    //////////////////////////////////////////////////////////////////////
    virtual void addIndexBuffer(const HardwareIndexBuffer& indexbuffer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the iUseHardwareBuffers property.
    //////////////////////////////////////////////////////////////////////
    virtual bool useHardwareBuffers() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the iUseHardwareBuffers property.
    //////////////////////////////////////////////////////////////////////
    virtual void setUseHardwareBuffers(bool b);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the iSoftBuffersChanged property.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasSoftwareBuffersChanged() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Forces a change to iSoftBuffersChanged property.
    //////////////////////////////////////////////////////////////////////
    virtual void setSoftwareBuffersChanged(bool b);
    
private:
    
    /// @brief Holds the SoftwareVertexBuffer.
    SoftwareVertexBufferHolder iSoftVertexBuffer;
    
    /// @brief Holds the SoftwareIndexBuffers.
    SoftwareIndexBufferHolderList iSoftIndexBuffers;
    
    /// @brief True if the Renderer can create HardwareBuffers.
    bool iUseHardwareBuffers;
    
    /// @brief Holds the HardwareVertexBuffer.
    HardwareVertexBufferHolder iHardVertexBuffer;
    
    /// @brief Holds the HardwareIndexBuffers.
    HardwareIndexBufferHolderList iHardIndexBuffers;
    
    /// @brief True if the Software Buffers have changed.
    mutable bool iSoftBuffersChanged;
};

//////////////////////////////////////////////////////////////////////
/// @brief A proxy to use MeshPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Mesh : public SpecializedResourceUser<MeshPrivate>
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh(Resource* resource = nullptr);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh(const Mesh& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh(const ResourceUser& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Mesh& operator = (const Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~Mesh();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Vertex Buffer object.
    //////////////////////////////////////////////////////////////////////
    HardwareVertexBuffer& getVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Vertex Buffer object.
    //////////////////////////////////////////////////////////////////////
    const HardwareVertexBuffer& getVertexBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Index Buffer batch.
    //////////////////////////////////////////////////////////////////////
    HardwareIndexBufferBatch& getIndexBufferBatch();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Index Buffer batch.
    //////////////////////////////////////////////////////////////////////
    const HardwareIndexBufferBatch& getIndexBufferBatch() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh Hardware Type.
    //////////////////////////////////////////////////////////////////////
    MeshPrivate::Type getType() const;
    
    static Mesh Null;
};

GreEndNamespace

#endif
