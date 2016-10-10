// DefaultVertex.vert : Default Vertex Shader for Gang's Resource Engine.

#version 330

layout(location = 0) in vec3 Position ;
layout(location = 2) in vec2 TexCoord ;

out vec2 uTexCoord ;

uniform mat4 Projection ;
uniform mat4 View ;
uniform mat4 Model ;

void main () 
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0) ;
    uTexCoord = TexCoord;
}
