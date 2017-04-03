//////////////////////////////////////////////////////////////////////
//
// Structs/Material.glsl
// Defines the material structure.
//
//////////////////////////////////////////////////////////////////////

struct MaterialStruct {

	vec4 ambient ;
	vec4 diffuse ;
	vec4 specular ;
	vec4 emission ;
	float shininess ;
};

// As we don't know if the material texture is a sampler2D , a samplerCube or other structure ,
// we should let the user defines the following uniforms : 
//   - MaterialDiffuseTexture  : Texture used to look up diffuse color.
//   - MaterialSpecularTexture : Texture used to look up specular color.
//   - MaterialNormalTexture   : Texture used to transform normals vectors.
