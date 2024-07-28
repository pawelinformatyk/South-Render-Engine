#version 450

layout(location = 0) in vec3 InLocation;

layout(binding = 0) uniform UniformBufferObject {
    mat4 View;
    mat4 Proj;
} Ubo;

layout(push_constant, std430) uniform PC {
    mat4 Model;
};

layout(location = 0) out vec3 OutColor;

void main()
{
    gl_Position = Ubo.Proj * Ubo.View * Model * vec4(InLocation, 1.0);
    OutColor = vec3(0.3);
}