#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 View;
    mat4 Projection;
} Ubo;

layout(location = 0) in vec3 InPosition;

layout(location = 1) out vec3 OutNearPoint;
layout(location = 2) out vec3 OutFarPoint;
layout(location = 3) out vec3 OutIndex;

vec3 gridPlane[6] = vec3[] (
vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(vec3 Point) {
    mat4 viewInv = inverse(Ubo.View);
    mat4 projInv = inverse(Ubo.Projection);
    vec4 unprojectedPoint =  viewInv *   projInv * vec4(Point, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 Point = gridPlane[gl_VertexIndex];
    OutNearPoint = UnprojectPoint(vec3(Point.x, Point.y, 0.0));
    OutFarPoint = UnprojectPoint(vec3(Point.x, Point.y, 1.0));
    gl_Position = Ubo.Projection * Ubo.View * vec4(gridPlane[gl_VertexIndex].xyz, 1.0);

    OutIndex = vec3(gl_VertexIndex);
}