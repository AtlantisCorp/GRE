//
//  MeshLoader.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_MeshLoader_h
#define GRE_MeshLoader_h

#include "Mesh.h"
#include "Renderer.h"

GreBeginNamespace

class DLL_PUBLIC MeshLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    MeshLoader();
    MeshLoader(const MeshLoader&);
    virtual ~MeshLoader();
    
    virtual Resource* load(Resource::Type type, const std::string& name, const std::string& file, Renderer renderer) const;
    
    Resource* load(Resource::Type type, const std::string& name, const HardwareVertexBuffer& vbuf, const HardwareIndexBufferBatch& ibuf) const;
    
    Resource* load(Resource::Type type, const std::string& name, const MeshPrivate::Type& meshtype, const Renderer& renderer, const VertexBatch& vbatch, const MaterialIndexedFaceBatch& ibatch) const;
    
    /// @brief Returns true if given Resource::Type is supported.
    bool isTypeSupported (Resource::Type type) const;
    
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    virtual ResourceLoader* clone() const;
};

typedef ResourceLoaderFactory<MeshLoader> MeshLoaderFactory;

GreEndNamespace

#endif
