// These are the generics includes for every shaders used with GRE.


#define GRE_MAX_LIGHT_NUM 32

struct VertexOutput
{
    vec4 color ;
    vec3 position ;
    vec3 normal ;
    vec2 texcoord ;
    mat3 bumpmapTBN ;
};

struct Material
{
    vec4 emission ;
    vec4 ambient ;
    vec4 diffuse ;
    vec4 specular ;
    float shininess ;
    
    sampler2D normalmap ;
    int normalmap_enabled ;
    
    int hasDiffuseMap ;
    sampler2D diffuseMap ;
    
    int hasSpecularMap ;
    sampler2D specularMap ;
    
    int useDiffuseAsAmbient ;
};

struct Light
{
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess ; // Seulement pour la lumière spéculaire
    // Spécifique aux spots et points
    float attenuationConstant ;
    float attenuationLinear ;
    float attenuationQuadratique ;
    // Spécifique aux spots
    vec3 direction ;
    float angle ;
    float exposition;
    
    int type ; // 0 if directionnal, 1 if point, 2 if spot.
    int enabled ; // 0 if false , 1 if true.
};

struct Camera
{
    vec3 position ;
};

// Scene datas.

uniform Material    material ;
uniform Light       lights [ GRE_MAX_LIGHT_NUM ] ;
uniform Camera      camera ;

// Matrixes

uniform mat4 iProjection ;
uniform mat4 iView ;
uniform mat4 iModel ;
