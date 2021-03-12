// Phong shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoord;

out vec2 vTexCoord;

out vec4 vPosition;
out vec3 vNormal;

uniform mat4 ProjectionViewModel;
// We need rhis for the transform normal
void main()
{
    vTexCoord = TexCoord;
    gl_Position = ProjectionViewModel * Position;
}