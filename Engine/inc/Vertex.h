//
//  Vertex.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_Vertex_h
#define GRE_Vertex_h

#include "Color.h"

GreBeginNamespace

/// @brief Defines a simple Vertex object.
struct DLL_PUBLIC Vertex
{
    Vector3 position;
    Vector2 texture;
    Color   color;
    
    bool operator == (const Vertex& rhs) const;
    bool operator != (const Vertex& rhs) const;
};
typedef struct Vertex Vertex;
typedef std::vector<Vertex> VertexBatch;

VertexBatch DLL_PUBLIC VertexBatchFromRaw(float* data, size_t sz);

GreEndNamespace

#endif
