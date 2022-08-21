#include "sthpch.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "Core/App/Application.h"
#include "Core/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/VertexIndexBuffer.h"
#include "Editor/Camera.h"
#include "Editor/Mesh.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "tiny_obj_loader.h"
#include <gtx/string_cast.hpp>

namespace South
{

    PushConstant g_QuadPushConstant;
    PushConstant g_ExampleModelConstant;
    Camera g_EditorCam;

    void Renderer::Init()
    {
        if (s_Context) { return; }

        s_Context = new RendererContext;
        s_Context->Init();

        const std::vector<Vertex> QuadVertices = { {
                                                       glm::vec3(-1.f, -1.f, .0f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.1f, .1f, .1f),
                                                   },
                                                   {
                                                       glm::vec3(1.f, -1.f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.1f, .1f, .1f),
                                                   },
                                                   {
                                                       glm::vec3(1.f, 1.f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.1f, .1f, .1f),
                                                   },
                                                   {
                                                       glm::vec3(-1.f, 1.f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.1f, .1f, .1f),
                                                   } };

        const std::vector<uint32_t> QuadIndices = { 0, 1, 2, 2, 3, 0 };

        s_QuadModelBuffer = VertexIndexBuffer::Create({ static_cast<const void*>(QuadVertices.data()),
                                                        static_cast<uint32_t>(sizeof(Vertex)),
                                                        static_cast<uint32_t>(QuadVertices.size()) },
                                                      { static_cast<const void*>(QuadIndices.data()),
                                                        static_cast<uint32_t>(sizeof(uint32_t)),
                                                        static_cast<uint32_t>(QuadIndices.size()) });

        g_EditorCam.SetView(glm::vec3(0.f, 7.5f, -5.f), glm::vec3(0.0f, 0.0f, 0.0f));
        g_EditorCam.SetProjection(glm::radians(90.0f),
                                  static_cast<float>(s_Context->m_SwapChainExtent.width) /
                                      static_cast<float>(s_Context->m_SwapChainExtent.height),
                                  0.00000000001f,
                                  2000000.0f);

        g_ExampleModelConstant.Model = glm::mat4(1.f);
        g_ExampleModelConstant.Model =
            glm::rotate(g_ExampleModelConstant.Model, glm::radians(120.f), glm::vec3(1.f, 0.f, 0.f));
        g_ExampleModelConstant.View       = g_EditorCam.GetViewMatrix();
        g_ExampleModelConstant.Projection = g_EditorCam.GetProjectionMatrix();

        g_QuadPushConstant       = g_ExampleModelConstant;
        g_QuadPushConstant.Model = glm::mat4(1.f);
        g_QuadPushConstant.Model = glm::rotate(g_QuadPushConstant.Model, glm::radians(120.f), glm::vec3(1.f, 0.f, 0.f));
        g_QuadPushConstant.Model = glm::scale(g_QuadPushConstant.Model, glm::vec3(10.f, 10.f, 10.f));

        LoadExampleScene();

        STH_INFO("Renderer Initialized");
    }

    void Renderer::Deinit()
    {
        if (!s_Context) { return; }

        if (s_QuadModelBuffer) { VertexIndexBuffer::Destroy(s_Context->GetLogicalDevice(), *s_QuadModelBuffer); }

        s_Context->DeInit();

        delete s_Context;
        s_Context = nullptr;

        STH_INFO("Renderer Deinitialized");
    }

    void Renderer::BeginFrame()
    {
        VkDevice LogicDevice = s_Context->GetLogicalDevice();

        // Wait for the previous frame to finish
        vkWaitForFences(LogicDevice, 1, &s_Context->m_FlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(LogicDevice, 1, &s_Context->m_FlightFence);

        // Acquire an image from the swap chain
        vkAcquireNextImageKHR(LogicDevice,
                              s_Context->m_SwapChain,
                              UINT64_MAX,
                              s_Context->m_ImageAvailableSemaphore,
                              VK_NULL_HANDLE,
                              &s_ImageIndex);

        // Submit the recorded command buffer
        vkResetCommandBuffer(s_Context->m_CommandBuffer, 0);

        const VkCommandBufferBeginInfo BeginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = 0,
            .pInheritanceInfo = nullptr,
        };

        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;
        vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

        // #TODO : Pipeline static value?
        const VkClearValue ClearColor = { { 0.008f, 0.008f, 0.008f, 1.f } };

        const VkRenderPassBeginInfo RenderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext           = nullptr,
            .renderPass      = s_Context->m_RenderPass,
            .framebuffer     = s_Context->m_SwapChainFramebuffers[s_ImageIndex],
            .renderArea      = VkRect2D({ 0, 0 }, s_Context->m_SwapChainExtent),
            .clearValueCount = 1,
            .pClearValues    = &ClearColor,
        };

        vkCmdBeginRenderPass(CmdBuffer, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s_Context->m_GraphicsPipeline);
    }

    void Renderer::DrawExampleScene()
    {
        DrawQuad(glm::mat4(1.f));

        // Animation
        {
            static int16_t i = 0;
            ++i;
            g_ExampleModelConstant.Model =
                glm::rotate(g_ExampleModelConstant.Model, glm::radians(.01f), glm::vec3(1.0f, .0f, 1.0f));
        }

        const VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        vkCmdPushConstants(CmdBuffer,
                           s_Context->m_PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           sizeof(g_ExampleModelConstant),
                           &g_ExampleModelConstant);

        const VkBuffer MeshBuffer     = s_ExampleSceneBuffer->GetVulkanBuffer();
        constexpr VkDeviceSize Offset = 0;
        vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &MeshBuffer, &Offset);
        vkCmdBindIndexBuffer(CmdBuffer, MeshBuffer, s_ExampleSceneBuffer->GetVerticesSize(), VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(CmdBuffer, static_cast<uint32_t>(s_ExampleSceneBuffer->GetIndicesCount()), 1, 0, 0, 0);
    }

    void Renderer::DrawQuad(const glm::mat4& InTransform)
    {
        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        vkCmdPushConstants(CmdBuffer,
                           s_Context->m_PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           sizeof(g_QuadPushConstant),
                           &g_QuadPushConstant);

        VkBuffer MeshBuffer = s_QuadModelBuffer->GetVulkanBuffer();
        VkDeviceSize Offset = 0;
        vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &MeshBuffer, &Offset);
        vkCmdBindIndexBuffer(CmdBuffer, MeshBuffer, s_QuadModelBuffer->GetVerticesSize(), VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(CmdBuffer, static_cast<uint32_t>(s_QuadModelBuffer->GetIndicesCount()), 1, 0, 0, 0);
    }

    void Renderer::EndFrame()
    {
        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        vkCmdEndRenderPass(CmdBuffer);

        vkEndCommandBuffer(CmdBuffer);
    }

    void Renderer::Present()
    {
        VkQueue GraphicQueue                    = s_Context->GetGraphicQueue().m_Queue;
        const VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        const VkSemaphore WaitSemaphores[]      = { s_Context->m_ImageAvailableSemaphore };
        const VkSemaphore SignalSemaphores[]    = { s_Context->m_RenderFinishedSemaphore };

        const VkSubmitInfo SubmitInfo{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                = nullptr,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = WaitSemaphores,
            .pWaitDstStageMask    = WaitStages,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &s_Context->m_CommandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = SignalSemaphores,
        };

        vkQueueSubmit(GraphicQueue, 1, &SubmitInfo, s_Context->m_FlightFence);

        const VkSwapchainKHR SwapChains[] = { s_Context->m_SwapChain };

        // Present the swap chain image
        const VkPresentInfoKHR SresentInfo{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = SignalSemaphores,
            .swapchainCount     = 1,
            .pSwapchains        = SwapChains,
            .pImageIndices      = &s_ImageIndex,
            .pResults           = nullptr,
        };

        vkQueuePresentKHR(GraphicQueue, &SresentInfo);
    }

    void Renderer::LoadExampleScene()
    {
        std::random_device Dev;
        std::mt19937 Rng(Dev());
        std::uniform_real_distribution<float> Dist(0.f, 1.f);

        tinyobj::attrib_t Attrib;
        std::vector<tinyobj::shape_t> Shapes;

        const std::string FileName = "Resources/Models/jetengine8.obj";

        if (!tinyobj::LoadObj(&Attrib, &Shapes, nullptr, nullptr, nullptr, FileName.c_str())) { return; }

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        for (const auto& Shape : Shapes)
        {
            const glm::vec3 Color = glm::vec3(.6f, Dist(Rng), 1.f);

            for (const auto& Index : Shape.mesh.indices)
            {
                const Vertex ShapeVertex = {
                    .m_Pos   = { Attrib.vertices[3 * Index.vertex_index + 0],
                               Attrib.vertices[3 * Index.vertex_index + 1],
                               Attrib.vertices[3 * Index.vertex_index + 2] },
                    .m_Color = Color,
                };

                Vertices.emplace_back(ShapeVertex);
                Indices.emplace_back(static_cast<uint32_t>(Indices.size()));
            }
        }

        s_ExampleSceneBuffer = VertexIndexBuffer::Create({ static_cast<const void*>(Vertices.data()),
                                                           static_cast<uint32_t>(sizeof(Vertex)),
                                                           static_cast<uint32_t>(Vertices.size()) },
                                                         { static_cast<const void*>(Indices.data()),
                                                           static_cast<uint32_t>(sizeof(uint32_t)),
                                                           static_cast<uint32_t>(Indices.size()) });
    }
} // namespace South