//
//  Vertex.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "Vertex.h"

GRE_BEGIN_NAMESPACE

bool Vertex::operator==(const Vertex &rhs) const
{
    return position == rhs.position &&
           texture  == rhs.texture  &&
           color    == rhs.color;
}

bool Vertex::operator!=(const Vertex &rhs) const
{
    return !(*this == rhs);
}

VertexBatch VertexBatchFromRaw(float* data, size_t sz)
{
    VertexBatch batch;
    size_t pos = 0;
    size_t numelemen = sizeof(Vertex)/sizeof(float);
    while(pos < sz)
    {
        Vertex added;
        added.position.x   = data[pos*numelemen + 0];
        added.position.y   = data[pos*numelemen + 1];
        added.position.z   = data[pos*numelemen + 2];
        
        added.texture.x    = data[pos*numelemen + 3];
        added.texture.y    = data[pos*numelemen + 4];
        
        added.color.red    = data[pos*numelemen + 5];
        added.color.green  = data[pos*numelemen + 6];
        added.color.blue   = data[pos*numelemen + 7];
        added.color.alpha  = data[pos*numelemen + 8];
        batch.push_back(added);
        pos++;
    }
    
    return std::move(batch);
}

GRE_END_NAMESPACE
