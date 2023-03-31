#include "sthpch.h"

#include "Editor.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Example/stb_image.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "Core/Buffers/UniformBuffer.h"
#include "Core/Buffers/VertexIndexBuffer.h"
#include "Core/Devices/GraphicCard.h"
#include "Core/Shaders/ShadersLibrary.h"
#include "EditorViewport.h"
#include "Mesh.h"

namespace South
{
namespace Private
{

void InitImGuiStyle()
{
    ImGuiStyle& Style = ImGui::GetStyle();

    ImVec4* Colors                         = Style.Colors;
    Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    Colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, .50f);
    Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    Colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    Colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.4156f, 0.3529f, 0.8039f, 1.00f);
    Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
    Colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    Colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    Colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.70f);
    Colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    Colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    Colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    Colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    Colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    Colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    Colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    Colors[ImGuiCol_DockingPreview]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);
    Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    Colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    Colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    Style.WindowPadding     = ImVec2(8.00f, 8.00f);
    Style.FramePadding      = ImVec2(5.00f, 2.00f);
    Style.CellPadding       = ImVec2(6.00f, 6.00f);
    Style.ItemSpacing       = ImVec2(6.00f, 6.00f);
    Style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
    Style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    Style.IndentSpacing     = 25;
    Style.ScrollbarSize     = 15;
    Style.GrabMinSize       = 10;
    Style.WindowBorderSize  = 1;
    Style.ChildBorderSize   = 1;
    Style.PopupBorderSize   = 1;
    Style.FrameBorderSize   = 1;
    Style.TabBorderSize     = 1;
    Style.WindowRounding    = 0.f;
    Style.ChildRounding     = 4;
    Style.FrameRounding     = 3;
    Style.PopupRounding     = 4;
    Style.ScrollbarRounding = 9;
    Style.GrabRounding      = 3;
    Style.LogSliderDeadzone = 4;
    Style.TabRounding       = 4;
}

} // namespace Private

Camera g_EditorCam;

const std::vector g_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

const std::vector<Vertex> g_Vertices = {{glm::vec3(-0.5f, -0.5f, 0.0f), {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                                        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                                        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

                                        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                                        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                                        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

const std::vector<uint32_t> g_Indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

Editor::Editor(GLFWwindow& InWindow) : m_Window(&InWindow)
{
    m_MainViewport = new EditorViewport();

    CreateInstance();

    if(m_bEnableValidationLayers)
    {
        if(!CheckValidationLayers())
        {
            STH_VK_WARN("ValidationsLayers not found.");
        }
        else
        {
            CreateMessenger();
        }
    }

    CreateSurface();

    CreateDevices();

    CreateSwapChain();

    g_EditorCam.SetView(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.0f, 0.0f, 0.0f));
    g_EditorCam.SetProjection(glm::radians(45.0f),
                              static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height),
                              0.01f,
                              10.0f);

    CreateImageViews();
    CreateRenderPass();
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();
    CreateCommandPool();
    CreateDepthResources();
    CreateFramebuffers();
    CreateTextureImage();
    CreateTextureImageView();
    CreateTextureSampler();

    CreateCameraUbos();

    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSyncObjects();

    m_QuadBuffer = VertexIndexBuffer::Create(
        m_LogicalDevice->GetLogicalDevice(),
        m_CommandBuffers[0],
        m_LogicalDevice->GetGraphicQueue(),
        {static_cast<const void*>(g_Vertices.data()), static_cast<uint32_t>(sizeof(Vertex)), static_cast<uint32_t>(g_Vertices.size())},
        {static_cast<const void*>(g_Indices.data()), static_cast<uint32_t>(sizeof(uint32_t)), static_cast<uint32_t>(g_Indices.size())});

    // #TODO: Add ImGui ignoring other rendering.

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();

    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    ImGui_ImplGlfw_InitForVulkan(&InWindow, true);

    // // #TODO : Remove this struct later and refactor ImGui_ImplVulkan_...
    ImGui_ImplVulkan_InitInfo InitInfo = {
        .Instance        = m_VulkanInstance,
        .PhysicalDevice  = m_GPU->GetPhysicalDevice(),
        .Device          = m_LogicalDevice->GetLogicalDevice(),
        .QueueFamily     = m_LogicalDevice->GetGraphicQueueFamilyIndex(),
        .Queue           = m_LogicalDevice->GetGraphicQueue(),
        .PipelineCache   = nullptr,
        .DescriptorPool  = m_DescriptorPool,
        .Subpass         = 0,
        .MinImageCount   = 2,
        .ImageCount      = MAX_FRAMES_IN_FLIGHT,
        .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
        .Allocator       = nullptr,
        .CheckVkResultFn = nullptr,
    };
    ImGui_ImplVulkan_Init(&InitInfo, m_RenderPass);

    // constexpr float FontSize = 17.f;
    // #TODO : Path should be somewhere coded?
    // IO.FontDefault = IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\DroidSans.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Cousine-Regular.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Karla-Regular.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyClean.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyTiny.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Medium.ttf", FontSize);

    vkResetCommandPool(m_LogicalDevice->GetLogicalDevice(), m_CommandPool, 0);

    VkCommandBufferBeginInfo BeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    };
    BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], &BeginInfo);

    ImGui_ImplVulkan_CreateFontsTexture(m_CommandBuffers[m_CurrentFrameIndex]);

    const VkSubmitInfo SubmitInfo = {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &m_CommandBuffers[m_CurrentFrameIndex],
    };

    vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex]);

    vkQueueSubmit(m_LogicalDevice->GetGraphicQueue(), 1, &SubmitInfo, VK_NULL_HANDLE);

    vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    Private::InitImGuiStyle();
}

Editor::~Editor()
{
    // #TODO: What is this?
    vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    CleanupSwapChain();

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    vkDestroyPipeline(Device, m_GraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(Device, m_PipelineLayout, nullptr);
    vkDestroyRenderPass(Device, m_RenderPass, nullptr);

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        UniformBuffer::Destroy(Device, m_CameraUbos[i]);
    }

    vkDestroyDescriptorPool(Device, m_DescriptorPool, nullptr);

    vkDestroySampler(Device, m_TextureSampler, nullptr);
    vkDestroyImageView(Device, m_TextureImageView, nullptr);

    vkDestroyImage(Device, m_TextureImage, nullptr);
    vkFreeMemory(Device, m_TextureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(Device, m_DescriptorSetLayout, nullptr);

    VertexIndexBuffer::Destroy(Device, m_QuadBuffer);

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(Device, m_RenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Device, m_ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(Device, m_InFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(Device, m_CommandPool, nullptr);

    vkDestroyDevice(Device, nullptr);

    if(m_bEnableValidationLayers)
    {
        DestroyMessenger();
    }

    vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
    vkDestroyInstance(m_VulkanInstance, nullptr);
}

void Editor::Tick()
{
    m_MainViewport->Tick();
    UpdateCamera(m_CurrentFrameIndex);
}

void Editor::BeginFrame()
{
    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    // Wait for the previous frame to finish
    vkWaitForFences(Device, 1, &m_InFlightFences[m_CurrentFrameIndex], VK_TRUE, UINT64_MAX);
    vkResetFences(Device, 1, &m_InFlightFences[m_CurrentFrameIndex]);

    vkAcquireNextImageKHR(Device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrameIndex], nullptr, &m_CurrentImageIndex);

    vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], 0x00);

    const VkCommandBufferBeginInfo BeginInfo {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = 0,
        .pInheritanceInfo = nullptr,
    };
    vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], &BeginInfo);

    std::array<VkClearValue, 2> ClearColor {};
    ClearColor[0].color        = {{0.0f, 0.0f, 0.0f, 1.0f}};
    ClearColor[1].depthStencil = {1.0f, 0};

    const VkRenderPassBeginInfo RenderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext           = nullptr,
        .renderPass      = m_RenderPass,
        .framebuffer     = m_SwapChainFramebuffers[m_CurrentImageIndex],
        .renderArea      = VkRect2D({0, 0}, m_SwapChainExtent),
        .clearValueCount = static_cast<uint32_t>(ClearColor.size()),
        .pClearValues    = ClearColor.data(),
    };
    vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrameIndex], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
}

void Editor::EndFrame()
{
    vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrameIndex]);
    vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex]);
}

void Editor::Render()
{
    VkViewport viewport {};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = (float)m_SwapChainExtent.width;
    viewport.height   = (float)m_SwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = m_SwapChainExtent;
    vkCmdSetScissor(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &scissor);

    const VkBuffer         QuadBuffer = m_QuadBuffer->GetBuffer();
    constexpr VkDeviceSize Offset     = 0;
    vkCmdBindVertexBuffers(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &QuadBuffer, &Offset);

    vkCmdBindIndexBuffer(m_CommandBuffers[m_CurrentFrameIndex], QuadBuffer, m_QuadBuffer->GetVerticesSize(), VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(m_CommandBuffers[m_CurrentFrameIndex],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_PipelineLayout,
                            0,
                            1,
                            &m_DescriptorSets[m_CurrentFrameIndex],
                            0,
                            nullptr);

    vkCmdDrawIndexed(m_CommandBuffers[m_CurrentFrameIndex], m_QuadBuffer->GetIndicesCount(), 1, 0, 0, 0);
}

void Editor::BeginGUI()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::RenderGUI()
{
    // #TODO: Setup UI and when drawing ui layer get image of a scene (from framebuffer).


    ImGuiIO& IO                     = ImGui::GetIO();
    IO.ConfigWindowsResizeFromEdges = IO.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

    constexpr ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                             ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport& Viewport = *ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(Viewport.Pos);
    ImGui::SetNextWindowSize(Viewport.Size);
    ImGui::SetNextWindowViewport(Viewport.ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4 {0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::Begin("Editor", nullptr, WindowFlags);
    {
        ImGui::PopStyleColor(); // MenuBarBg
        ImGui::PopStyleVar(2);

        ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        ImGui::Begin("Viewport");
        {
            const ImVec2 ViewportSize = ImGui::GetContentRegionAvail();

            if(m_TextureId)
            {
                ImGui::Image(m_TextureId, ViewportSize);
            }
        }
        ImGui::End();
    }
    ImGui::End();
}

void Editor::EndGUI()
{
    ImGui::Render();

    ImDrawData& DrawData = *ImGui::GetDrawData();

    ImGui_ImplVulkan_RenderDrawData(&DrawData, m_CommandBuffers[m_CurrentFrameIndex]);

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void Editor::Present()
{
    VkPipelineStageFlags WaitStages(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    const VkSubmitInfo SubmitInfo {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,
        .waitSemaphoreCount   = 1,
        .pWaitSemaphores      = &m_ImageAvailableSemaphores[m_CurrentFrameIndex],
        .pWaitDstStageMask    = &WaitStages,
        .commandBufferCount   = 1,
        .pCommandBuffers      = &m_CommandBuffers[m_CurrentFrameIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores    = &m_RenderFinishedSemaphores[m_CurrentFrameIndex],
    };
    vkQueueSubmit(m_LogicalDevice->GetGraphicQueue(), 1, &SubmitInfo, m_InFlightFences[m_CurrentFrameIndex]);

    const VkPresentInfoKHR PresentInfo {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &m_RenderFinishedSemaphores[m_CurrentFrameIndex],
        .swapchainCount     = 1,
        .pSwapchains        = &m_SwapChain,
        .pImageIndices      = &m_CurrentImageIndex,
        .pResults           = nullptr,
    };
    vkQueuePresentKHR(m_LogicalDevice->GetPresentQueue(), &PresentInfo);

    m_TextureId =
        ImGui_ImplVulkan_AddTexture(m_TextureSampler, m_SwapChainImageViews[m_CurrentFrameIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Editor::CleanupSwapChain()
{
    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    vkDestroyImageView(Device, m_DepthImageView, nullptr);
    vkDestroyImage(Device, m_DepthImage, nullptr);
    vkFreeMemory(Device, m_DepthImageMemory, nullptr);

    for(auto framebuffer : m_SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(Device, framebuffer, nullptr);
    }

    for(auto imageView : m_SwapChainImageViews)
    {
        vkDestroyImageView(Device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(Device, m_SwapChain, nullptr);
}

void Editor::RecreateSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_Window, &width, &height);

    while(width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_Window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

    CleanupSwapChain();

    CreateSwapChain();
    CreateImageViews();
    CreateDepthResources();
    CreateFramebuffers();
}

void Editor::CreateInstance()
{
    const VkApplicationInfo sAppInfo {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "South",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "None",
        .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion         = VK_MAKE_API_VERSION(0, 1, 3, 0),
    };

    const std::vector<const char*> Extensions = GetRequiredInstanceExtensions();

    const VkInstanceCreateInfo sCreateInfo {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = nullptr,
        .pApplicationInfo        = &sAppInfo,
        .enabledLayerCount       = m_bEnableValidationLayers ? static_cast<uint32_t>(m_RequiredValidationLayers.size()) : 0,
        .ppEnabledLayerNames     = m_bEnableValidationLayers ? m_RequiredValidationLayers.data() : nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(Extensions.size()),
        .ppEnabledExtensionNames = Extensions.data(),
    };

    vkCreateInstance(&sCreateInfo, nullptr, &m_VulkanInstance);
}

void Editor::CreateSurface()
{
    if(glfwCreateWindowSurface(m_VulkanInstance, m_Window, nullptr, &m_Surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Editor::CreateDevices()
{
    m_GPU = GraphicCard::Create(m_VulkanInstance,
                                GraphicCard::CreateInfo {
                                    .RequiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                                    .RequiredFeatures =
                                        {
                                            .multiViewport = true,
                                        },
                                });


    m_LogicalDevice = LogicalDeviceAndQueues::Create(*m_GPU, m_Surface);
}

void Editor::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_GPU->GetPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR   presentMode   = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D         extent        = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_Surface;

    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    const uint32_t queueFamilyIndices[] = {m_LogicalDevice->GetGraphicQueueFamilyIndex(), m_LogicalDevice->GetPresentQueueFamilyIndex()};

    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;

    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    if(vkCreateSwapchainKHR(Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(Device, m_SwapChain, &imageCount, nullptr);
    m_SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent      = extent;
}

void Editor::CreateImageViews()
{
    m_SwapChainImageViews.resize(m_SwapChainImages.size());

    for(uint32_t i = 0; i < m_SwapChainImages.size(); i++)
    {
        m_SwapChainImageViews[i] = CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void Editor::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment {};
    colorAttachment.format         = m_SwapChainImageFormat;
    colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment {};
    depthAttachment.format         = FindDepthFormat();
    depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency {};
    dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass    = 0;
    dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo                 renderPassInfo {};
    renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments    = attachments.data();
    renderPassInfo.subpassCount    = 1;
    renderPassInfo.pSubpasses      = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies   = &dependency;

    if(vkCreateRenderPass(m_LogicalDevice->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Editor::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding {};
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding {};
    samplerLayoutBinding.binding            = 1;
    samplerLayoutBinding.descriptorCount    = 1;
    samplerLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo             layoutInfo {};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings    = bindings.data();

    if(vkCreateDescriptorSetLayout(m_LogicalDevice->GetLogicalDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void Editor::CreateGraphicsPipeline()
{
    ShadersLibrary::Init();

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    VkPipelineShaderStageCreateInfo vertShaderStageInfo =
        ShadersLibrary::AddShader(Device, "VulkanTutorial_V", "Resources/Shaders/vulkanTutorial.vert", VK_SHADER_STAGE_VERTEX_BIT)
            ->GetInfo();

    VkPipelineShaderStageCreateInfo fragShaderStageInfo =
        ShadersLibrary::AddShader(Device, "VulkanTutorial_F", "Resources/Shaders/vulkanTutorial.frag", VK_SHADER_STAGE_FRAGMENT_BIT)
            ->GetInfo();

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription    = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributesDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount   = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
    inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState {};
    viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount  = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer {};
    rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable        = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth               = 1.0f;
    rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable         = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling {};
    multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable  = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil {};
    depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable       = VK_TRUE;
    depthStencil.depthWriteEnable      = VK_TRUE;
    depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable     = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment {};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending {};
    colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable     = VK_FALSE;
    colorBlending.logicOp           = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount   = 1;
    colorBlending.pAttachments      = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState>      dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState {};
    dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates    = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts    = &m_DescriptorSetLayout;

    if(vkCreatePipelineLayout(Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount          = 2;
    pipelineInfo.pStages             = shaderStages;
    pipelineInfo.pVertexInputState   = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState      = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState   = &multisampling;
    pipelineInfo.pDepthStencilState  = &depthStencil;
    pipelineInfo.pColorBlendState    = &colorBlending;
    pipelineInfo.pDynamicState       = &dynamicState;
    pipelineInfo.layout              = m_PipelineLayout;
    pipelineInfo.renderPass          = m_RenderPass;
    pipelineInfo.subpass             = 0;
    pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

    if(vkCreateGraphicsPipelines(Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

void Editor::CreateFramebuffers()
{
    m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

    for(size_t i = 0; i < m_SwapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {m_SwapChainImageViews[i], m_DepthImageView};

        VkFramebufferCreateInfo framebufferInfo {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = m_RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments    = attachments.data();
        framebufferInfo.width           = m_SwapChainExtent.width;
        framebufferInfo.height          = m_SwapChainExtent.height;
        framebufferInfo.layers          = 1;

        if(vkCreateFramebuffer(m_LogicalDevice->GetLogicalDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Editor::CreateCommandPool()
{
    VkCommandPoolCreateInfo poolInfo {};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = m_LogicalDevice->GetGraphicQueueFamilyIndex();

    if(vkCreateCommandPool(m_LogicalDevice->GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void Editor::CreateDepthResources()
{
    VkFormat depthFormat = FindDepthFormat();

    CreateImage(m_SwapChainExtent.width,
                m_SwapChainExtent.height,
                depthFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                m_DepthImage,
                m_DepthImageMemory);
    m_DepthImageView = CreateImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat Editor::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for(VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_GPU->GetPhysicalDevice(), format, &props);

        if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat Editor::FindDepthFormat()
{
    return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool Editor::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Editor::CreateTextureImage()
{
    int          texWidth, texHeight, texChannels;
    stbi_uc*     pixels    = stbi_load("Resources/Textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if(!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(imageSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer,
                 stagingBufferMemory);

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    void* data;
    vkMapMemory(Device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(Device, stagingBufferMemory);

    stbi_image_free(pixels);

    CreateImage(texWidth,
                texHeight,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                m_TextureImage,
                m_TextureImageMemory);

    TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(m_TextureImage,
                          VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(Device, stagingBuffer, nullptr);
    vkFreeMemory(Device, stagingBufferMemory, nullptr);
}

void Editor::CreateTextureImageView()
{
    m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Editor::CreateTextureSampler()
{
    VkPhysicalDeviceProperties properties {};
    vkGetPhysicalDeviceProperties(m_GPU->GetPhysicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo {};
    samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter               = VK_FILTER_LINEAR;
    samplerInfo.minFilter               = VK_FILTER_LINEAR;
    samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable        = VK_TRUE;
    samplerInfo.maxAnisotropy           = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable           = VK_FALSE;
    samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if(vkCreateSampler(m_LogicalDevice->GetLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

VkImageView Editor::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo {};
    viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image                           = image;
    viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                          = format;
    viewInfo.subresourceRange.aspectMask     = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    VkImageView imageView;
    if(vkCreateImageView(m_LogicalDevice->GetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void Editor::CreateImage(uint32_t              width,
                         uint32_t              height,
                         VkFormat              format,
                         VkImageTiling         tiling,
                         VkImageUsageFlags     usage,
                         VkMemoryPropertyFlags properties,
                         VkImage&              image,
                         VkDeviceMemory&       imageMemory)
{
    VkImageCreateInfo imageInfo {};
    imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType     = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width  = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth  = 1;
    imageInfo.mipLevels     = 1;
    imageInfo.arrayLayers   = 1;
    imageInfo.format        = format;
    imageInfo.tiling        = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage         = usage;
    imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    if(vkCreateImage(Device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Device, image, imageMemory, 0);
}

void Editor::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier {};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = newLayout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    EndSingleTimeCommands(commandBuffer);
}

void Editor::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferImageCopy region {};
    region.bufferOffset                    = 0;
    region.bufferRowLength                 = 0;
    region.bufferImageHeight               = 0;
    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;
    region.imageOffset                     = {0, 0, 0};
    region.imageExtent                     = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    EndSingleTimeCommands(commandBuffer);
}

void Editor::CreateCameraUbos()
{
    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_CameraUbos[i] = UniformBuffer::Create(m_LogicalDevice->GetLogicalDevice());
    }
}

void Editor::CreateDescriptorPool()
{
    // #TODO: How many PoolSizes? And how many sets.
    const std::vector<VkDescriptorPoolSize> PoolSizes = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                                         {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    const VkDescriptorPoolCreateInfo CreateInfo = {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets       = 1000 * static_cast<uint32_t>(PoolSizes.size()),
        .poolSizeCount = static_cast<uint32_t>(PoolSizes.size()),
        .pPoolSizes    = PoolSizes.data(),
    };
    vkCreateDescriptorPool(m_LogicalDevice->GetLogicalDevice(), &CreateInfo, nullptr, &m_DescriptorPool);
}

void Editor::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
    VkDescriptorSetAllocateInfo        allocInfo {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = m_DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts        = layouts.data();

    m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if(vkAllocateDescriptorSets(m_LogicalDevice->GetLogicalDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = m_CameraUbos[i]->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView   = m_TextureImageView;
        imageInfo.sampler     = m_TextureSampler;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

        descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet          = m_DescriptorSets[i];
        descriptorWrites[0].dstBinding      = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &bufferInfo;

        descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet          = m_DescriptorSets[i];
        descriptorWrites[1].dstBinding      = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo      = &imageInfo;

        vkUpdateDescriptorSets(m_LogicalDevice->GetLogicalDevice(),
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(),
                               0,
                               nullptr);
    }
}

void Editor::CreateBuffer(VkDeviceSize          size,
                          VkBufferUsageFlags    usage,
                          VkMemoryPropertyFlags properties,
                          VkBuffer&             buffer,
                          VkDeviceMemory&       bufferMemory)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size        = size;
    bufferInfo.usage       = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    if(vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Device, buffer, bufferMemory, 0);
}

VkCommandBuffer Editor::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = m_CommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_LogicalDevice->GetLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Editor::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(m_LogicalDevice->GetGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_LogicalDevice->GetGraphicQueue());

    vkFreeCommandBuffers(m_LogicalDevice->GetLogicalDevice(), m_CommandPool, 1, &commandBuffer);
}

void Editor::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

uint32_t Editor::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_GPU->GetPhysicalDevice(), &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Editor::CreateCommandBuffers()
{
    m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = m_CommandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

    if(vkAllocateCommandBuffers(m_LogicalDevice->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Editor::CreateSyncObjects()
{
    m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    const VkDevice Device = m_LogicalDevice->GetLogicalDevice();

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if(vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
           vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
           vkCreateFence(Device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Editor::UpdateCamera(uint32_t currentImage)
{
    static auto StartTime = std::chrono::high_resolution_clock::now();
    const auto  CurrTime  = std::chrono::high_resolution_clock::now();
    const float DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(CurrTime - StartTime).count();

    const UniformBufferObject Ubo {
        .m_Model = glm::rotate(glm::mat4(1.0f), DeltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .m_View  = g_EditorCam.GetView(),
        .m_Proj  = g_EditorCam.GetProjection(),
    };

    m_CameraUbos[currentImage]->SetData(m_LogicalDevice->GetLogicalDevice(), &Ubo);
}

VkShaderModule Editor::CreateShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if(vkCreateShaderModule(m_LogicalDevice->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

VkSurfaceFormatKHR Editor::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Editor::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Editor::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width  = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

SwapChainSupportDetails Editor::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

std::vector<const char*> Editor::GetRequiredInstanceExtensions() const
{
    uint32_t     ExtensionCount = 0;
    const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);

    std::vector<const char*> Extensions(GlfwExtensions, GlfwExtensions + ExtensionCount);

    if(m_bEnableValidationLayers)
    {
        Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return Extensions;
}


void Editor::CreateMessenger()
{
    const VkDebugUtilsMessengerCreateInfoEXT CreateInfo {
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext           = nullptr,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = ValidationMessageCallback,
        .pUserData       = nullptr,
    };

    if(const auto Func =
           reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_VulkanInstance, "vkCreateDebugUtilsMessengerEXT")))
    {
        Func(m_VulkanInstance, &CreateInfo, nullptr, &m_Messenger);
    }
    else
    {
        STH_VK_WARN("vkCreateDebugUtilsMessengerEXT not found");
    }
}

void Editor::DestroyMessenger()
{
    if(const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
           vkGetInstanceProcAddr(m_VulkanInstance, "vkDestroyDebugUtilsMessengerEXT")))
    {
        Func(m_VulkanInstance, m_Messenger, nullptr);
        m_Messenger = nullptr;
    }
    else
    {
        STH_VK_WARN("vkDestroyDebugUtilsMessengerEXT not found");
    }
}

VkBool32 Editor::ValidationMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      InMessageSeverity,
                                           VkDebugUtilsMessageTypeFlagsEXT             InMessageType,
                                           const VkDebugUtilsMessengerCallbackDataEXT* InCallbackData,
                                           void*                                       InUserData)
{
    if(InCallbackData)
    {
        switch(InMessageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                STH_VK_ERR(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                STH_VK_WARN(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                STH_VK_INFO(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                STH_VK_TRACE(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                break;
        }
    }

    // Return always false.
    return VK_FALSE;
}

bool Editor::CheckValidationLayers() const
{
    uint32_t Count;
    vkEnumerateInstanceLayerProperties(&Count, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(Count);
    vkEnumerateInstanceLayerProperties(&Count, AvailableLayers.data());

    for(const char* ReqLayer : m_RequiredValidationLayers)
    {
        bool bFound = false;

        for(const VkLayerProperties& AvLayer : AvailableLayers)
        {
            if(strcmp(ReqLayer, AvLayer.layerName) == 0)
            {
                bFound = true;
                break;
            }
        }

        if(!bFound)
        {
            return false;
        }
    }

    return true;
}

} // namespace South
