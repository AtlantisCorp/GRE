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
/// @brief Calculates non - attenuated Color Ambient, Diffuse and Specular
/// from given built-in light.
vec4 CalculateColor ( VertexOutput Fragment , const LightStruct light )
{
    vec3 ViewLightPosition = light.position.xyz ;
    vec3 N = normalize(MaterialGetNormal ( Fragment )) ;
    vec3 L = normalize(ViewLightPosition - Fragment.position) ;
    vec3 E = normalize(Camera.position - Fragment.position ) ;
    vec3 R = normalize( -reflect( L, N ) ) ;

    // Calculate Ambient term.
    vec4 Ambient = MaterialGetAmbientColor(Fragment) * light.ambient ;

    // Calculate Diffuse term.
    vec4 Diffuse = MaterialGetDiffuseColor(Fragment) * light.diffuse * max(dot(N,L), 0.0) ;
    Diffuse = clamp ( Diffuse , 0.0 , 1.0 ) ;

    // Calculate Specular term.
    vec4 Specular = MaterialGetSpecularColor(Fragment) * light.specular * pow(max(dot(R,E),0.0), Material.shininess) ;
    Specular = clamp ( Specular , 0.0 , 1.0 ) ;

    // Return total color.
    return Ambient + Diffuse + Specular ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculate normal attenuation from linear, constant and
/// quadratic values.
float CalculateAttenuation ( VertexOutput Fragment , const LightStruct light )
{
    vec3 ViewLightPosition = vec4( vec4(light.position, 1.0) ).xyz ;
    float D = length ( ViewLightPosition - Fragment.position ) ;

    float Attenuation = light.attCnst + light.attLine * D + light.attQuad * D * D ;

    if ( Attenuation == 0.0f ) {
        return 1.0f ;
    } else {
        return 1.0f / Attenuation ;
    }
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculate attenuation depending on spotlight values.
float CalculateAttenuationSpotlight ( VertexOutput Fragment , const LightStruct light )
{
    vec3 ViewLightPosition = vec4( vec4(light.position, 1.0) ).xyz ;
    vec3 LightFragmentDirection = normalize ( ViewLightPosition - Fragment.position ) ;

    float AngleMax = light.angle ;
    float Angle = acos( dot ( LightFragmentDirection , -normalize(light.direction) ) ) ;

    if ( Angle >= AngleMax ) {
        return 0.0f ;
    }

    float Factor = AngleMax / Angle ;
    float Intensity = 1.0 - ( 1.0 / pow ( Factor , light.exposition ) ) ;
    float Attenuation = clamp ( Intensity , 0.0 , 1.0 ) ;

    return CalculateAttenuation (Fragment, light) * Attenuation ;
}

//////////////////////////////////////////////////////////////////////
/// @brief Calculate total amount of color adding every built-in
/// lights.
vec4 CalculateLightsColors ( VertexOutput Fragment )
{
    vec4 FinalColor = vec4 ( 0.0 , 0.0 , 0.0 , 1.0 ) ;

    for ( int i = 0 ; i < Light.length() ; ++i )
    {
        vec4 Color = CalculateColor ( Fragment , Light[i] ) ;

        //if ( Light[i].type == 1 ) Color = Color * CalculateAttenuation ( Fragment , Light[i] ) ;
        //if ( Light[i].type == 2 ) Color = Color * CalculateAttenuationSpotlight ( Fragment , Light[i] ) ;

        FinalColor = FinalColor + Color ;
    }

    return vec4 ( FinalColor.rgb , 1.0 ) ;
}
