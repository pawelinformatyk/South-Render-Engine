#pragma once

#include "Core/Renderer/RendererContext.h"

namespace South
{

class VertexIndexBuffer;
class UniformBuffer;

// #TODO : Not a fan of static.
// Should be able to be initialised, deinitialised and render objects (using commands).
// It decides for itself whether renderer should be vulkan, opengl, directx...
class Renderer
{
public:
    static void Init(const RendererContext::CreateInfo& Info);
    static void Deinit();

    // static void Submit(std::function<void()>&);

    static void RenderQuad(VkCommandBuffer InCommandBuffer, VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform);

    static void RenderTriangle(VkCommandBuffer InCommandBuffer, VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform);

    static void RenderMesh(VkCommandBuffer          InCommandBuffer,
                           VkPipelineLayout         InPipelineLayout,
                           const VertexIndexBuffer& InMeshBuffer,
                           const glm::mat4&         InTransform);

    static void RenderMesh(VkCommandBuffer InCommandBuffer, const VertexIndexBuffer& InMeshBuffer);

    //    //     Q submit etc.
    //    static void Present();
};

} // namespace South