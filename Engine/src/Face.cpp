//
//  Face.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "Face.h"

GreBeginNamespace

IndexedFaceBatch IndexedFaceBatchFromRaw(unsigned* indices, unsigned facenum, unsigned faceelem)
{
    IndexedFaceBatch ret;
    
    unsigned pos = 0;
    while(pos < facenum)
    {
        IndexedFace face;
        face.material = Material::Null;
        
        unsigned elemnum = 0;
        while(elemnum < faceelem) {
            face.indices.push_back(indices[pos*faceelem + elemnum]);
            elemnum++;
        }
        ret.push_back(face);
        pos++;
    }
    
    return ret;
}

MaterialIndexedFace::MaterialIndexedFace()
: indexedFaces(), material()
{
    
}

MaterialIndexedFace::~MaterialIndexedFace()
{
    
}

GreEndNamespace
