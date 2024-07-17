#version 450

layout(location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inColor;

layout( push_constant ) uniform PushConstant
{
	mat4 M;
	mat4 V;
	mat4 P;
}PS;

layout(location = 0) out vec3 fragColor;

void main() 
{
    gl_Position =   PS.P * PS.V * PS.M * vec4(inPosition, 1.0);
	fragColor = inColor;    
}