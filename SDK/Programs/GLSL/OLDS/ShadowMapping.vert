//////////////////////////////////////////////////////////////////////
//
// ShadowMapping.vert
//
// Simple Vertex GLSL Shader to compute the fragment position from a
// given light shadow camera.
// This shader may be called only when activating one light, which should
// be lights[0]. This light should contain a valid LightSpaceMatrix which
// translate the vertex position to light-space position.
// Please see ShadowMapping technique for more explanations.
//
//////////////////////////////////////////////////////////////////////
#version 330

#include Programs/GLSL/Structs/Light.glsl

uniform LightStruct Light[1] ;

layout(location = 0) in vec3 iPosition ;

void main ()
{
    gl_Position = Light[0].lightSpaceMatrix * vec4(iPosition, 1.0) ;
}