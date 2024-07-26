#version 450

layout(location = 0) in vec3 InLoc_WS;
layout(location = 1) in vec3 InNormal_WS;

layout(location = 0) out vec4 OutColor;

void main()
{
    vec3 Albedo = vec3(0.9);
    float Roughness = 0.6;

    vec3 CameraPos_WS = vec3(20, 0, 50);
    vec3 LightPosition_WS = vec3(50, 0, 50);

    vec3 Diffuse = pow(Albedo, vec3(2.5));
    float Specular = 1 - Roughness;
    float Shininess = 20. / (Roughness * Roughness) - 2.;

    vec3 CameraToLightDir = normalize(CameraPos_WS - InLoc_WS);
    vec3 LightToVertDir = normalize(LightPosition_WS - InLoc_WS);

    float Diff = max(dot(LightToVertDir, InNormal_WS), 0.0);

    vec3 HalfwayDir = normalize(LightToVertDir + CameraToLightDir);
    float Spec = pow(max(dot(InNormal_WS, HalfwayDir), 0.0), Shininess);

    float LightToVertDst = length(LightPosition_WS - InLoc_WS);

    float Attenuation = 300. / (LightToVertDst * LightToVertDst);

    vec3 DiffusePart  = vec3(6.5) * Diff * Diffuse * Attenuation;
    vec3 SpecularPart = vec3(15.8) * Spec * Specular * Attenuation;

    vec3 Ambient = vec3(0.1);

    vec3 Color = Ambient + DiffusePart + SpecularPart;

    // Output color of pixel. 
    OutColor = vec4(Color, 1.0);
}