#version 450

layout(location = 0) in vec3 InLoc_WS;
layout(location = 1) in vec3 InNormal_WS;
layout(location = 2) in vec4 InColor;

layout(location = 0) out vec4 OutColor;

void main()
{
    const vec3 CameraPos_WS = vec3(-55, 35, 27);
    const vec3 LightPosition_WS = vec3(-100, 100, 500);

    const vec3 CameraToLightDir = normalize(CameraPos_WS - InLoc_WS);
    const vec3 LightToVertDir = normalize(LightPosition_WS - InLoc_WS);

    const float Diff = max(dot(LightToVertDir, InNormal_WS), 0.0);

    const float Shininess = 2;

    const vec3 HalfwayDir = normalize(LightToVertDir + CameraToLightDir);
    const float Spec = pow(max(dot(InNormal_WS, HalfwayDir), 0.0), Shininess);

    const float LightToVertDst = length(LightPosition_WS - InLoc_WS);

    const vec3 DiffusePart  = vec3(1.45) * InColor.xyz * Diff;
    const vec3 SpecularPart = vec3(0.8) * Spec;

    const vec3 Ambient = vec3(0.1);

    const vec3 Color = Ambient + DiffusePart + SpecularPart;

    // Output color of pixel. 
    OutColor = vec4(InColor.xyz, 1.0);
}