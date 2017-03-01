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

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the given light direction between the light and
/// the fragment position.
vec3 LightDirection ( const int i )
{
    return normalize ( vertex_output.position - lights[i].position ) ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the distance between light and fragment.
vec3 LightDistance ( const int i )
{
    return normalize ( vertex_output.position - lights[i].position ) ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the ambient color for the given light.
vec3 LightColorDirectionnalAmbient ( const int i )
{
    return vec3 ( lights[i].ambient ) * MaterialAmbient () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates diffuse directionnal light color.
vec3 LightColorDirectionnalDiffuse ( const int i , const vec3 normal , const vec3 lightdir )
{
    float diff = max(dot(normal, lightdir), 0.0) ;
    return lights[i].diffuse.rgb * diff * MaterialDiffuse () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates specular directionnal light color.
vec3 LightColorDirectionnalSpecular ( const int i , const vec3 norm , const vec3 lightdir , const vec3 viewdist )
{
    vec3 reflectdir = reflect ( -lightdir , norm ) ;
    float spec = pow(max(dot(normalize(viewdist), reflectdir), 0.0), lights[i].shininess) ;
    return lights[i].specular.rgb * spec * MaterialSpecular () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the ambient, specular and diffuse color from the
/// given directionnal light and the current material.
vec3 LightColorDirectionnal ( const int i , const vec3 normal , const vec3 viewdist )
{
    //////////////////////////////////////////////////////////////////////
    // Temporary datas used by more than one function.
    vec3 lightdir = normalize ( - lights[i].direction ) ;
    
    //////////////////////////////////////////////////////////////////////
    // Calculates ambient, diffuse and specular colors.
    vec3 ambient = LightColorDirectionnalAmbient (i) ;
    vec3 diffuse = LightColorDirectionnalDiffuse (i, normal, lightdir) ;
    vec3 specular = LightColorDirectionnalSpecular (i, normal, lightdir, viewdist) ;
    
    return ambient + diffuse + specular ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates ambient point color.
vec3 LightColorPointAmbient ( const int i )
{
    return vec3 ( lights[i].ambient ) * MaterialAmbient () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates diffuse point color.
vec3 LightColorPointDiffuse ( const int i , const vec3 normal , const vec3 lightdir )
{
    float diff = max(dot(normal, lightdir), 0.0) ;
    return lights[i].diffuse.rgb * diff * MaterialDiffuse () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates specular point color.
vec3 LightColorPointSpecular ( const int i , const vec3 normal , const vec3 lightdir , const vec3 viewdist )
{
    vec3 reflectdir = reflect ( -lightdir , normal ) ;
    float spec = pow(max(dot(normalize(viewdist), reflectdir), 0.0), lights[i].shininess) ;
    return lights[i].specular.rgb * spec * MaterialSpecular () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates attenuation for point.
float LightAttenuationPoint ( const int i )
{
    float d = length( LightDistance(i) ) ;
    return 1.0f / ( lights[i].attenuationConstant + lights[i].attenuationLinear * d + lights[i].attenuationQuadratique * d * d ) ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the colors for Point Light.
vec3 LightColorPoint ( const int i , const vec3 normal , const vec3 viewdist )
{
    //////////////////////////////////////////////////////////////////////
    // Temporary datas used by more than one function.
    vec3 lightdir = LightDirection (i) ;
    
    //////////////////////////////////////////////////////////////////////
    // Calculates ambient, diffuse and specular colors.
    vec3 ambient = LightColorPointAmbient (i) ;
    vec3 diffuse = LightColorPointDiffuse (i, normal, lightdir) ;
    vec3 specular = LightColorPointSpecular (i, normal, lightdir, viewdist) ;
    
    //////////////////////////////////////////////////////////////////////
    // Attenuation thing.
    float att = LightAttenuationPoint (i) ;
    
    ambient = ambient * att ;
    diffuse = diffuse * att ;
    specular = specular * att ;
    
    return ambient + diffuse + specular ;
}

//////////////////////////////////////////////////////////////////////
/// @brief calculates the ambient spot color.
vec3 LightColorSpotAmbient ( const int i )
{
    return vec3 ( lights[i].ambient ) * MaterialAmbient () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the diffuse spot color.
vec3 LightColorSpotDiffuse ( const int i , const vec3 normal , const vec3 lightdir )
{
    float diff = max(dot(normal, lightdir), 0.0) ;
    return lights[i].diffuse.rgb * diff * MaterialDiffuse () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the specular spot color.
vec3 LightColorSpotSpecular ( const int i , const vec3 normal , const vec3 lightdir , const vec3 viewdist )
{
    vec3 reflectdir = reflect ( -lightdir , normal ) ;
    float spec = pow(max(dot(normalize(viewdist), reflectdir), 0.0), lights[i].shininess) ;
    return lights[i].specular.rgb * spec * MaterialSpecular () ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the attenuation spot.
float LightAttenuationSpot ( const int i , const vec3 lightdir )
{
    float maxangle = lights[i].angle ;
    float curangle = acos( dot ( lightdir , normalize(lights[i].direction) ) );
    float att = 0.0f ;
    
    if ( curangle >= maxangle ) {
        return 0.0f ;
    }
    
    if ( curangle < maxangle && curangle != 0.0 )
    {
        float factor = maxangle / curangle ;
        float intensity = 1.0 - ( 1.0 / pow ( factor , lights[i].exposition ) ) ;
        att = clamp ( intensity , 0.0 , 1.0 ) ;
    }
    
    return LightAttenuationPoint (i) * att ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates spot colors.
vec3 LightColorSpot ( const int i , const vec3 normal , const vec3 viewdist )
{
    //////////////////////////////////////////////////////////////////////
    // Temporary datas used by more than one function.
    vec3 lightdir = LightDirection (i) ;
    
    //////////////////////////////////////////////////////////////////////
    // Calculates ambient, diffuse and specular colors.
    vec3 ambient = LightColorSpotAmbient (i) ;
    vec3 diffuse = LightColorSpotDiffuse (i, normal, lightdir) ;
    vec3 specular = LightColorSpotSpecular (i, normal, lightdir, viewdist) ;
    
    //////////////////////////////////////////////////////////////////////
    // Attenuation thing.
    float att = LightAttenuationSpot (i, lightdir) ;
    
    ambient = ambient * att ;
    diffuse = diffuse * att ;
    specular = specular * att ;
    
    return ambient + diffuse + specular ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculates the color for every lights registered.
vec4 LightsColor ( void )
{
    vec3 finalcolor = vec3(0.0f, 0.0f, 0.0f) ;
    
    //////////////////////////////////////////////////////////////////////
    // Temporary datas used in each functions.
    vec3 normal = MaterialNormal () ;
    vec3 viewdist = CameraFragmentDistance () ;
    
    //////////////////////////////////////////////////////////////////////
    // Parse every enabled lights.
    for ( int i = 0 ; i < GRE_MAX_LIGHT_NUM ; ++i ) {
        if ( lights[i].enabled == 1 )
        {
            if ( lights[i].type == 0 ) {
                finalcolor = finalcolor + LightColorDirectionnal ( i , normal , viewdist ) ;
            }
            
            else if ( lights[i].type == 1 ) {
                finalcolor = finalcolor + LightColorPoint ( i , normal , viewdist ) ;
            }
            
            else if ( lights[i].type == 2 ) {
                finalcolor = finalcolor + LightColorSpot ( i , normal , viewdist ) ;
            }
        }
    }
    
    return vec4(finalcolor, 1.0) ;
}

