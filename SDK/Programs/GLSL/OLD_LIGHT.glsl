//////////////////////////////////////////////////////////////////////
//
//  BaseLight.glsl
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

// Defines some functions to compute color for different lights.

vec3 MakeNormal ( void )
{
    if ( material.normalmap_enabled == 1 )
    {
        vec3 bumpmap = texture( material.normalmap , vertex_output.texcoord ).rgb ;
        bumpmap = normalize( bumpmap * 2.0 - 1.0 ) ;
        bumpmap = normalize( vertex_output.bumpmapTBN * bumpmap ) ;
        
        return bumpmap ;
    }
    
    else
    {
        return normalize(vertex_output.normal) ;
    }
}

vec4 ComputeDirectionnalLight ( const int i )
{
    vec3 lightDistance = normalize( vertex_output.position - lights[i].position ) ;
    vec3 cameraDistance = normalize( vertex_output.position - camera.position ) ;
    vec3 norm = MakeNormal() ;
    
    // Ambient color.
    vec4 ambient = lights[i].ambient * material.ambient ;
    
    // Diffuse color.
    float angle = max(dot(norm , lightDistance), 0.0) ;
    vec4 diffuse = lights[i].diffuse * angle * material.diffuse ;
    
    // Specular color.
    vec3 reflectdir = reflect( -lightDistance , norm ) ;
    float spec = pow(max(dot(cameraDistance, reflectdir), 0.0), lights[i].shininess) ;
    vec4 specular = lights[i].specular * ( spec * material.specular ) ;
    
    return ambient + diffuse + specular ;
}

vec4 ComputePointLight ( const int i )
{
    vec3 lightDistance = normalize( vertex_output.position - lights[i].position ) ;
    vec3 cameraDistance = normalize( vertex_output.position - camera.position ) ;
    vec3 norm = MakeNormal() ;
    
    // Compute attenuation for given light.
    float d = length(lightDistance) ;
    float attenuation = lights[i].attenuationConstant +
                        lights[i].attenuationLinear * d +
                        lights[i].attenuationQuadratique * d * d ;
    
    if ( attenuation == 0.0 )
        attenuation = 1.0 ;
    
    attenuation = 1.0 / attenuation ;
    
    // Ambient color.
    vec4 ambient = lights[i].ambient * material.ambient ;
    
    // Diffuse color.
    float angle = max(dot(norm , lightDistance), 0.0) ;
    vec4 diffuse = lights[i].diffuse * angle * material.diffuse ;
    
    // Specular color.
    vec3 reflectdir = reflect( -lightDistance , norm ) ;
    float spec = pow(max(dot(cameraDistance, reflectdir), 0.0), lights[i].shininess) ;
    vec4 specular = lights[i].specular * ( spec * material.specular ) ;
    
    if ( attenuation == 0.0 ) {
        attenuation = 1.0f ;
    }
    
    return abs((ambient + diffuse + specular) * attenuation) ;
}

vec4 ComputeSpotLight ( const int i )
{
    vec3 lightDistance = normalize( vertex_output.position - lights[i].position ) ;
    vec3 cameraDistance = normalize( vertex_output.position - camera.position ) ;
    vec3 norm = MakeNormal() ;
    
    // Compute attenuation for given light.
    
    float d = length(lightDistance) ;
    float attenuation = lights[i].attenuationConstant +
                        lights[i].attenuationLinear * d +
                        lights[i].attenuationQuadratique * d * d ;
    
    if ( attenuation == 0.0 )
        attenuation = 1.0 ;
    
    attenuation = 1.0 / attenuation ;
    
    // Compute second attenuation. (spotlight intensity)
    
    float maxangle = lights[i].angle ;
    float curangle = acos( dot ( lightDistance , normalize(lights[i].direction) ) );
    
    if ( curangle < maxangle && curangle != 0.0 )
    {
        float factor = maxangle / curangle ;
        float intensity = 1.0 - ( 1.0 / pow ( factor , lights[i].exposition ) ) ;
        attenuation = attenuation * clamp ( intensity , 0.0 , 1.0 ) ;
    }
    
    if ( curangle >= maxangle )
    {
        attenuation = 0.0f ;
    }
    
    // Ambient color.
    vec4 ambient = lights[i].ambient * material.ambient ;
    
    // Diffuse color.
    float angle = max(dot(norm , lightDistance), 0.0) ;
    vec4 diffuse = lights[i].diffuse * angle * material.diffuse ;
    
    // Specular color.
    vec3 reflectdir = reflect( -lightDistance , norm ) ;
    float spec = pow(max(dot(cameraDistance, reflectdir), 0.0), lights[i].shininess) ;
    vec4 specular = lights[i].specular * ( spec * material.specular ) ;
    
    if ( attenuation == 0.0 ) {
        attenuation = 0.0f ;
    }
    
    return abs((ambient + diffuse + specular) * attenuation) ;
}

vec4 ComputeLightsColor ( void )
{
    int i = 0 ;
    vec4 value = vec4(0.0f, 0.0f, 0.0f, 1.0f) ;
    
    for ( i = 0 ; i < GRE_MAX_LIGHT_NUM ; ++i )
    {
        if ( lights[i].enabled == 1 )
        {
            if ( lights[i].type == 0 ) {
                value = value + ComputeDirectionnalLight(i) ;
            }
            
            else if ( lights[i].type == 1 ) {
                value = value + ComputePointLight(i) ;
            }
            
            else if ( lights[i].type == 2 ) {
                value = value + ComputeSpotLight(i) ;
            }
        }
    }
    
    return vec4(vec3(value), 1.0) ;
}


