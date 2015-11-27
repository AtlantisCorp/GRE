//
//  Face.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_Face_h
#define GRE_Face_h

#include "Vertex.h"
#include "Material.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines a Face, with its material if it exists.
struct DLL_PUBLIC Face
{
    std::vector<Vertex> vertexs;
    Material            material;
};
typedef struct Face Face;

/// @brief A face with indices to describe the vertex used.
struct DLL_PUBLIC IndexedFace
{
    std::vector<unsigned> indices;
    Material              material;
};

typedef std::vector<IndexedFace> IndexedFaceBatch;
IndexedFaceBatch IndexedFaceBatchFromRaw(unsigned* indices, unsigned facenum, unsigned faceelem);

struct DLL_PUBLIC MaterialIndexedFace
{
    IndexedFaceBatch indexedFaces;
    Material         material;
};

typedef std::vector<MaterialIndexedFace> MaterialIndexedFaceBatch;

GRE_END_NAMESPACE

#endif
