//////////////////////////////////////////////////////////////////////
//
//  BaseMaterial.glsl
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 22/02/2017.
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

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the material normal.
/// If the material contains a normal map, it will calculate the resulting
/// normal using the normal map texture.
vec3 MaterialGetNormal ( VertexOutput Fragment )
{
    if ( textureSize(MaterialNormalTexture,0).x > 0 )
    {
        vec3 bumpmap = texture( MaterialNormalTexture , Fragment.texcoord ).rgb ;
        bumpmap = normalize( bumpmap * 2.0 - 1.0 ) ;
        bumpmap = normalize( Fragment.bumpmapTBN * bumpmap ) ;
        
        return bumpmap ;
    }
    
    else
    {
        return normalize(Fragment.normal) ;
    }
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the diffuse color for the material.
vec4 MaterialGetDiffuseColor ( VertexOutput Fragment )
{
    if ( textureSize(MaterialDiffuseTexture,0).x > 0 ) {
        return texture ( MaterialDiffuseTexture , Fragment.texcoord ) ;
    }
    
    else {
        return Material.diffuse ;
    }
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the correct ambient color from material.
vec4 MaterialGetAmbientColor ( VertexOutput Fragment )
{
    return Material.ambient ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates correct specular color from material.
vec4 MaterialGetSpecularColor ( VertexOutput Fragment )
{
    if ( textureSize(MaterialSpecularTexture,0).x > 0 ) {
        return texture ( MaterialSpecularTexture , Fragment.texcoord ) ;
    }
    
    else {
        return Material.specular ;
    }
}
