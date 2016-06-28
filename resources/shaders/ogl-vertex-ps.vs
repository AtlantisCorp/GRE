// Vertex Shader - file "ogl-vertex-ps.vs"

#version 140

in  vec3 Position;
in  vec3 Color;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 ex_Color;

void main(void)
{
    
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(Position, 1.0);
    ex_Color = Color;
}