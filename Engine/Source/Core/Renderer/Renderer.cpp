#include "sthpch.h"

#include "Core/Renderer/Renderer.h"

#include "CommandPool.h"

namespace South
{

void Renderer::Init(const RendererContext::CreateInfo& Info)
{
    s_Context = new RendererContext(Info);

    STH_INFO("Renderer Initialized");
}

void Renderer::Deinit()
{
    delete s_Context;

    STH_INFO("Renderer Deinitialized");
}

// void Renderer::RenderExampleScene()
// {
//     const VkCommandBuffer  CmdBuffer      = s_Context->m_CommandBuffer;
//     const VkPipelineLayout PipelineLayout = s_Context->m_PipelineLayout;
//
//     RenderQuad(s_Context->GetCommandBuffer(), s_Context->GetPipelineLayout(), glm::mat4(1.f));
//
//     // RenderMesh(CmdBuffer, PipelineLayout, *s_ExampleSceneBuffer, ModelTransform);
// }
//
// void Renderer::RenderQuad(const VkCommandBuffer InCommandBuffer, const VkPipelineLayout InPipelineLayout, const glm::mat4& InTransform)
// {
//     const PushConstant Ps {
//         .Model      = InTransform,
//         .View       = g_EditorCam.GetView(),
//         .Projection = g_EditorCam.GetProjection(),
//     };
//
//     vkCmdPushConstants(InCommandBuffer, InPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Ps), &Ps);
//
//     const VkBuffer         MeshBuffer = s_QuadModelBuffer->GetBuffer();
//     constexpr VkDeviceSize Offset     = 0;
//     vkCmdBindVertexBuffers(InCommandBuffer, 0, 1, &MeshBuffer, &Offset);
//     vkCmdBindIndexBuffer(InCommandBuffer, MeshBuffer, s_QuadModelBuffer->GetVerticesSize(), VK_INDEX_TYPE_UINT32);
//
//     vkCmdDrawIndexed(InCommandBuffer, static_cast<uint32_t>(s_QuadModelBuffer->GetIndicesCount()), 1, 0, 0, 0);
// }
//
// void Renderer::RenderTriangle(const VkCommandBuffer InCommandBuffer, const VkPipelineLayout InPipelineLayout, const glm::mat4&
// InTransform)
// {
// }
//
// void Renderer::RenderMesh(const VkCommandBuffer    InCommandBuffer,
//                           const VkPipelineLayout   InPipelineLayout,
//                           const VertexIndexBuffer& InMeshBuffer,
//                           const glm::mat4&         InTransform)
// {
//     const PushConstant Ps {
//         .Model      = InTransform,
//         .View       = g_EditorCam.GetView(),
//         .Projection = g_EditorCam.GetProjection(),
//     };
//
//     vkCmdPushConstants(InCommandBuffer, InPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Ps), &Ps);
//
//     const VkBuffer         Buffer = InMeshBuffer.GetBuffer();
//     constexpr VkDeviceSize Offset = 0;
//
//     vkCmdBindVertexBuffers(InCommandBuffer, 0, 1, &Buffer, &Offset);
//     vkCmdBindIndexBuffer(InCommandBuffer, Buffer, InMeshBuffer.GetVerticesSize(), VK_INDEX_TYPE_UINT32);
//
//     vkCmdDrawIndexed(InCommandBuffer, static_cast<uint32_t>(InMeshBuffer.GetIndicesCount()), 1, 0, 0, 0);
// }

// void Renderer::LoadExampleScene()
// {
//     std::random_device                    Dev;
//     std::mt19937                          Rng(Dev());
//     std::uniform_real_distribution<float> Dist(0.f, 1.f);
//
//     tinyobj::attrib_t             Attrib;
//     std::vector<tinyobj::shape_t> Shapes;
//
//     const std::string FileName = "Resources/Models/jetengine8.obj";
//
//     if(!tinyobj::LoadObj(&Attrib, &Shapes, nullptr, nullptr, nullptr, FileName.c_str()))
//     {
//         return;
//     }
//
//     std::vector<Vertex>   Vertices;
//     std::vector<uint32_t> Indices;
//
//     for(const auto& Shape : Shapes)
//     {
//         for(const auto& Index : Shape.mesh.indices)
//         {
//             const Vertex ShapeVertex = {
//                 .m_Pos = {Attrib.vertices[3 * Index.vertex_index + 0],
//                           Attrib.vertices[3 * Index.vertex_index + 1],
//                           Attrib.vertices[3 * Index.vertex_index + 2]},
//             };
//
//             Vertices.emplace_back(ShapeVertex);
//             Indices.emplace_back(static_cast<uint32_t>(Indices.size()));
//         }
//     }
//
//     s_ExampleSceneBuffer = VertexIndexBuffer::Create(
//         s_Context->GetLogicalDevice(),
//         s_Context->GetCommandBuffer(),
//         s_Context->GetGraphicQueue(),
//         {static_cast<const void*>(Vertices.data()), static_cast<uint32_t>(sizeof(Vertex)), static_cast<uint32_t>(Vertices.size())},
//         {static_cast<const void*>(Indices.data()), static_cast<uint32_t>(sizeof(uint32_t)), static_cast<uint32_t>(Indices.size())});
// }

} // namespace South