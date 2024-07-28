#version 450

layout(location = 0) out vec4 OutColor;
layout(location = 0) in vec3 InColor;

void main()
{
    OutColor = vec4(InColor, 1.0);
}