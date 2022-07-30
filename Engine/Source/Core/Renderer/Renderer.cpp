#include "sthpch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/App/Application.h"
#include "Core/VulkanVertexIndexBuffer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace South
{

    void Renderer::Init()
    {
        if (s_Context) { return; };

        s_Context = new RendererContext;
        s_Context->Init();
    }

    void Renderer::DeInit()
    {
        if (!s_Context) { return; };

        s_Context->DeInit();

        delete s_Context;
        s_Context = nullptr;
    }

    void Renderer::BeginFrame()
    {
        // Animation
        {
            static int16_t i = 0;
            ++i;
            s_Context->m_PushConstant.Model =
                glm::rotate(s_Context->m_PushConstant.Model, glm::radians(.01f), glm::vec3(0.0f, 0.0f, 1.0f));
            s_Context->m_PushConstant.Model =
                glm::scale(s_Context->m_PushConstant.Model, (i > 0) ? glm::vec3(1.00005f) : glm::vec3(0.99995f));
        }

        auto& Device = s_Context->GetCurrentDevice();

        VkDevice LogicDevice  = Device.GetDevice();
        VkQueue GraphicsQueue = Device.GetQ();

        // Wait for the previous frame to finish
        vkWaitForFences(LogicDevice, 1, &s_Context->m_FlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(LogicDevice, 1, &s_Context->m_FlightFence);

        // Acquire an image from the swap chain
        vkAcquireNextImageKHR(LogicDevice, s_Context->m_SwapChain, UINT64_MAX, s_Context->m_ImageAvailableSemaphore,
                              VK_NULL_HANDLE, &s_ImageIndex);

        // Submit the recorded command buffer
        vkResetCommandBuffer(s_Context->m_CommandBuffer, 0);

        VkCommandBufferBeginInfo BeginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = 0,
            .pInheritanceInfo = nullptr,
        };

        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;
        vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

        // #TODO : Pipeline static value?
        VkClearValue ClearColor = { { 0.008f, 0.008f, 0.008f, 1.f } };

        VkRenderPassBeginInfo RenderPassInfo{
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

    void Renderer::Submit(std::function<void()>&) {}

    void Renderer::Draw()
    {
        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        // Draw
        {
            vkCmdPushConstants(CmdBuffer, s_Context->m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0,
                               sizeof(s_Context->m_PushConstant), &s_Context->m_PushConstant);

            VkBuffer MeshBuffer = s_Context->m_VI_Buffer->GetVulkanBuffer();
            VkDeviceSize Offset = 0;
            vkCmdBindVertexBuffers(CmdBuffer, 0, 1, &MeshBuffer, &Offset);
            vkCmdBindIndexBuffer(CmdBuffer, MeshBuffer, s_Context->m_VI_Buffer->GetIndexOffset(), VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(CmdBuffer, static_cast<uint32_t>(s_Context->m_Indices.size()), 1, 0, 0, 0);
        }

        // GUI
        {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // DrawTitleBar();
            ImGui::Begin("TitleBar", nullptr,
                         ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking);
            {

                ImGui::BeginMenuBar();
                {
                    ImGui::Separator();

                    if (ImGui::BeginMenu("File"))
                    {
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");

                        ImGui::EndMenu();
                    }
                    ImGui::Separator();

                    if (ImGui::BeginMenu("Project Settings"))
                    {
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");

                        ImGui::EndMenu();
                    }

                    ImGui::Separator();

                    if (ImGui::BeginMenu("Editor Settings"))
                    {
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");

                        ImGui::EndMenu();
                    }

                    ImGui::Separator();
                    if (ImGui::BeginMenu("View"))
                    {
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");
                        ImGui::MenuItem("Option");

                        ImGui::EndMenu();
                    }

                    ImGui::Separator();
                }
                ImGui::EndMenuBar();

                ImGui::BeginMenuBar();
                {
                    if (ImGui::Button("_")) { Application::Get().MinimiseApplication(true); }

                    if (ImGui::Button("[]")) { Application::Get().MaximiseApplication(); }

                    if (ImGui::Button("X")) { Application::Get().CloseApplication(); }
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();

            ImGui::Render();

            ImDrawData* DrawData = ImGui::GetDrawData();

            ImGui_ImplVulkan_RenderDrawData(DrawData, CmdBuffer);
        }
    }

    void Renderer::EndFrame()
    {
        VkCommandBuffer CmdBuffer = s_Context->m_CommandBuffer;

        vkCmdEndRenderPass(CmdBuffer);

        vkEndCommandBuffer(CmdBuffer);

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void Renderer::Flush()
    {
        VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore WaitSemaphores[]      = { s_Context->m_ImageAvailableSemaphore };
        VkSemaphore SignalSemaphores[]    = { s_Context->m_RenderFinishedSemaphore };

        VkSubmitInfo SubmitInfo{
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

        vkQueueSubmit(s_Context->GetCurrentDevice().GetQ(), 1, &SubmitInfo, s_Context->m_FlightFence);

        VkSwapchainKHR SwapChains[] = { s_Context->m_SwapChain };

        // Present the swap chain image
        VkPresentInfoKHR SresentInfo{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = SignalSemaphores,
            .swapchainCount     = 1,
            .pSwapchains        = SwapChains,
            .pImageIndices      = &s_ImageIndex,
            .pResults           = nullptr,
        };

        vkQueuePresentKHR(s_Context->GetCurrentDevice().GetQ(), &SresentInfo);
    }

} // namespace South