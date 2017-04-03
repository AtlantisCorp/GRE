//////////////////////////////////////////////////////////////////////
//
// ShadowMapping.frag
//
// As OpenGl already writes depth buffer, this shader is a pass-through
// and does absolutely nothing.
//
//////////////////////////////////////////////////////////////////////
#version 330

void main ()
{
    // This is what OpenGl already does by itself :
    // gl_FragDepth = gl_FragCoord.z ;
}