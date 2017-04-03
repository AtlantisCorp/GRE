//////////////////////////////////////////////////////////////////////
//
// Structs/Light.glsl
// Defines the light structure.
//
//////////////////////////////////////////////////////////////////////

struct LightStruct
{

	int type ;
	int enabled ;

    vec3 position ;
	vec3 direction ;
    
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    float shininess ;
    
    float attCnst ;
	float attLine ;
	float attQuad ;
    
    float angle ;
    float exposition ;
    
    // Gre::Light::iShadowTexture.
    sampler2D shadowTexture ;
    
    // Gre::Light::iShadowCamera.
    mat4 lightSpaceMatrix ;
    
    // Gre::Light::iShadowProjector.
    int isShadowProjector ;
};
