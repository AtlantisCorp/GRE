//
//  Material.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_Material_h
#define GRE_Material_h

#include "Color.h"
#include "Texture.h"

GRE_BEGIN_NAMESPACE

enum class FaceType
{
    Front,
    Back,
    FrontAndBack
};

struct DLL_PUBLIC Material
{
    Color    ambient;
    Color    diffuse;
    Color    specular;
    Color    emission;
    float    shininess;
    Texture  texture; ///< @brief Not null if used.
    FaceType faceUsed;
    
    static Material Null;
};
typedef struct Material Material;

GRE_END_NAMESPACE

#endif
