#version 450

layout(location = 1) in vec3 nearPoint;// nearPoint calculated in vertex shader
layout(location = 2) in vec3 InFarPoint;// farPoint calculated in vertex shader
layout(location = 3) in vec3 InIndex;

layout(location = 0) out vec4 outColor;

void main()
{
    float t = -nearPoint.y / (InFarPoint.y - nearPoint.y);
    //    outColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0));// opacity = 1 when t > 0, opacity = 0 otherwise
}