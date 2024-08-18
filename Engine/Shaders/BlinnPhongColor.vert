#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 View;
    mat4 Proj;
} Ubo;

layout(push_constant, std430) uniform PC {
    mat4 Model;
};

layout(location = 0) in vec3 InLocation;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InColor;

layout(location = 0) out vec3 OutLoc_WS;
layout(location = 1) out vec3 OutNormal_WS;
layout(location = 2) out vec3 OutColor;

void main()
{
    gl_Position = Ubo.Proj * Ubo.View * Model * vec4(InLocation, 1.0);
    OutLoc_WS = vec3(Model * vec4(InLocation, 1.0));
    OutNormal_WS = normalize(vec3(Model * vec4(InNormal, 1.0)));
    OutColor = InColor;
}