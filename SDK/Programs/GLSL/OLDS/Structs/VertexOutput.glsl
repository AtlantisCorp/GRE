//////////////////////////////////////////////////////////////////////
//
// VertexOutput.glsl
//
// Defines the possible standard output for a Vertex shader (and so
// standard inputs for a fragment shader).
//
//////////////////////////////////////////////////////////////////////

struct VertexOutput
{
    vec4 color ;
    vec3 position ;
    vec3 normal ;
    vec2 texcoord ;
    mat3 bumpmapTBN ;
};
