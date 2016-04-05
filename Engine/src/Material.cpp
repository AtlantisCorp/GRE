//
//  Material.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#include "Material.h"

GreBeginNamespace

Material Material::Null = Material();

Material::Material()
: ambient(), diffuse(), specular(), emission(), shininess(0.0f), texture(Texture::Null), faceUsed(FaceType::FrontAndBack)
{
    
}

Material& Material::operator=(const Material &rhs)
{
    ambient = rhs.ambient;
    diffuse = rhs.diffuse;
    specular = rhs.specular;
    emission = rhs.emission;
    shininess = rhs.shininess;
    faceUsed = rhs.faceUsed;
    texture = rhs.texture;
    
    return *this;
}

bool Material::hasTexture() const
{
    return !texture.expired();
}

GreEndNamespace
