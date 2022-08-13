#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/Renderer/Renderer.h"
#include "Core/VertexIndexBuffer.h"
#include "Core/VulkanDevice.h"
#include "Editor/Camera.h"
#include "Editor/Mesh.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <gtx/string_cast.hpp>

namespace South
{

    PushConstant g_PushConstant;
    Camera g_EditorCam;

    void Renderer::Init()
    {
        if (s_Context) { return; };

        s_Context = new RendererContext;
        s_Context->Init();

        const std::vector<Vertex> QuadVertices = { {
                                                       // NDC space
                                                       glm::vec3(-0.5f, -0.5f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(1.f, 1.f, 1),
                                                   },
                                                   {
                                                       glm::vec3(0.5f, -0.5f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.6f, .25f, 1.f),
                                                   },
                                                   {
                                                       glm::vec3(0.5f, 0.5f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(.6f, .25f, 1.f),
                                                   },
                                                   {
                                                       glm::vec3(-0.5f, 0.5f, 0.f),
                                                       glm::vec3(0.f),
                                                       glm::vec2(0.f),
                                                       glm::vec3(1.f, 1.f, 1),
                                                   } };

        const std::vector<uint32_t> QuadIndices = { 0, 1, 2, 2, 3, 0 };

        s_QuadModelBuffer = VertexIndexBuffer::Create({ static_cast<const void*>(QuadVertices.data()),
                                                        static_cast<uint32_t>(sizeof(Vertex)),
                                                        static_cast<uint32_t>(QuadVertices.size()) },
                                                      { static_cast<const void*>(QuadIndices.data()),
                                                        static_cast<uint32_t>(sizeof(uint32_t)),
                                                        static_cast<uint32_t>(QuadIndices.size()) });

        g_EditorCam.SetView(glm::vec3(2.f, 0.f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        g_EditorCam.SetProjection(glm::radians(70.0f),
                                  s_Context->m_SwapChainExtent.width / (float)s_Context->m_SwapChainExtent.height,
                                  0.1f,
                                  200.0f);


        g_PushConstant.Model      = glm::mat4(1.f);
        g_PushConstant.View       = g_EditorCam.GetViewMatrix();
        g_PushConstant.Projection = g_EditorCam.GetProjectionMatrix();

        STH_INFO("Renderer Initialized");
    }

    void Renderer::Deinit()
    {
        if (!s_Context) { return; };

        if (s_QuadModelBuffer)
        {
            VertexIndexBuffer::Destroy(s_Context->GetGpuDevice().GetDevice(), *s_QuadModelBuffer);
        }

        s_Context->DeInit();

        delete s_Context;
        s_Context = nullptr;

        STH_INFO("Renderer Deinitialized");
    }

    void Renderer::BeginFrame()
    {
        const auto& Device = s_Context->GetGpuDevice();

        VkDevice LogicDevice  = Device.GetDevice();
        VkQueue GraphicsQueue = Device.GetGraphicQueue();

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
        // Animation
        {
            static int16_t i = 0;
            ++i;
            g_PushConstant.Model = glm::rotate(g_PushConstant.Model, glm::radians(.01f), glm::vec3(0.0f, 0.0f, 1.0f));
            g_PushConstant.Model =
                glm::scale(g_PushConstant.Model, (i > 0) ? glm::vec3(1.00005f) : glm::vec3(0.99995f));
        }

        DrawQuad();
    }

    void Renderer::DrawQuad()
    {
        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        vkCmdPushConstants(CmdBuffer,
                           s_Context->m_PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           sizeof(g_PushConstant),
                           &g_PushConstant);

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

        vkQueueSubmit(s_Context->GetGpuDevice().GetGraphicQueue(), 1, &SubmitInfo, s_Context->m_FlightFence);

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

        vkQueuePresentKHR(s_Context->GetGpuDevice().GetGraphicQueue(), &SresentInfo);
    }

} // namespace South