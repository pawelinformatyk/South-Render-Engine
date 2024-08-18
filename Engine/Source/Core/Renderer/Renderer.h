#pragma once

#include "Core/Renderer/RendererContext.h"

namespace South
{

class SVertexIndexBuffer;
class SUniformBuffer;

// #TODO : Not a fan of static.
// Should be able to be initialised, deinitialised and render objects (using commands).
// It decides for itself whether renderer should be vulkan, opengl, directx...
class SRenderer
{
public:
    static void Init(const SRendererContext::SCreateInfo& Info);
    static void Deinit();

    // static void Submit(std::function<void()>&);

    static void RenderQuad(VkCommandBuffer InCommandBuffer, VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform);

    static void RenderTriangle(VkCommandBuffer InCommandBuffer, VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform);

    static void RenderMesh(VkCommandBuffer           InCommandBuffer,
                           VkPipelineLayout          InPipelineLayout,
                           const SVertexIndexBuffer& InMeshBuffer,
                           const glm::mat4&          InTransform);

    static void RenderMesh(VkCommandBuffer InCommandBuffer, const SVertexIndexBuffer& InMeshBuffer);

    //    //     Q submit etc.
    //    static void Present();
};

} // namespace South