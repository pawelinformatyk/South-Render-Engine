#include "Core/Renderer/Renderer.h"

#include "Core/Buffers/VertexIndexBuffer.h"
#include "Core/Mesh/Mesh.h"

namespace South
{

void Renderer::Init(const RendererContext::CreateInfo& Info)
{
    RendererContext& Context = RendererContext::Get();
    Context.Init(Info);

    STH_INFO("Renderer Initialized");
}

void Renderer::Deinit()
{
    RendererContext& Context = RendererContext::Get();
    Context.Deinit();

    STH_INFO("Renderer Deinitialized");
}

void Renderer::RenderQuad(const VkCommandBuffer InCommandBuffer, const VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform)
{
    //    const PushConstant Ps {
    //        .Model      = InTransform,
    //        .View       = g_EditorCam.GetView(),
    //        .Projection = g_EditorCam.GetProjection(),
    //    };
    //
    //    vkCmdPushConstants(InCommandBuffer, InPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Ps), &Ps);
    //
    //    const VkBuffer         MeshBuffer = s_QuadModelBuffer->GetBuffer();
    //    constexpr VkDeviceSize Offset     = 0;
    //    vkCmdBindVertexBuffers(InCommandBuffer, 0, 1, &MeshBuffer, &Offset);
    //    vkCmdBindIndexBuffer(InCommandBuffer, MeshBuffer, s_QuadModelBuffer->GetVerticesSize(), VK_INDEX_TYPE_UINT32);
    //
    //    vkCmdDrawIndexed(InCommandBuffer, static_cast<uint32_t>(s_QuadModelBuffer->GetIndicesCount()), 1, 0, 0, 0);
}

void Renderer::RenderTriangle(const VkCommandBuffer InCommandBuffer, const VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform)
{
}

void Renderer::RenderMesh(const VkCommandBuffer InCommandBuffer, const VertexIndexBuffer& InMeshBuffer)
{
    const VkBuffer         Buffer = InMeshBuffer.GetBuffer();
    constexpr VkDeviceSize Offset = 0;

    vkCmdBindVertexBuffers(InCommandBuffer, 0, 1, &Buffer, &Offset);
    vkCmdBindIndexBuffer(InCommandBuffer, Buffer, InMeshBuffer.GetVerticesSize(), VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(InCommandBuffer, static_cast<uint32_t>(InMeshBuffer.GetIndicesCount()), 1, 0, 0, 0);
}

void Renderer::RenderMesh(VkCommandBuffer          InCommandBuffer,
                          VkPipelineLayout         InPipelineLayout,
                          const VertexIndexBuffer& InMeshBuffer,
                          const glm::mat4&         InTransform)
{
}

} // namespace South