#include "Editor.h"
#include "Core/Buffers/UniformBuffer.h"
#include "Core/Buffers/VertexIndexBuffer.h"
#include "Core/Devices/GraphicCard.h"
#include "Core/Mesh/Mesh.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Shaders/ShadersLibrary.h"
#include "Event.h"
#include "Scene/SceneViewport.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "imgui.h"
#include <Application/App.h>

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
    Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.31f, 0.04f, 0.53f, 1.00f);
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
    Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
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

uint32_t FindMemoryType(const VkPhysicalDevice InPhysicalDevice, const uint32_t InTypeFilter, const VkMemoryPropertyFlags InProperties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(InPhysicalDevice, &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if((InTypeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & InProperties) == InProperties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void CreateImage(const VkDevice              InDevice,
                 const VkPhysicalDevice      InPhysicalDevice,
                 const uint32_t              InWidth,
                 const uint32_t              InHeight,
                 const VkFormat              InFormat,
                 const VkImageTiling         InTiling,
                 const VkImageUsageFlags     InUsage,
                 const VkMemoryPropertyFlags InProperties,
                 VkImage&                    OutImage,
                 VkDeviceMemory&             OutImageMemory)
{
    const VkImageCreateInfo ImageInfo {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext         = nullptr,
        .imageType     = VK_IMAGE_TYPE_2D,
        .format        = InFormat,
        .extent        = {InWidth, InHeight, 1},
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = InTiling,
        .usage         = InUsage,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    vkCreateImage(InDevice, &ImageInfo, nullptr, &OutImage);

    VkMemoryRequirements MemRequirements;
    vkGetImageMemoryRequirements(InDevice, OutImage, &MemRequirements);

    const VkMemoryAllocateInfo AllocInfo {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = MemRequirements.size,
        .memoryTypeIndex = FindMemoryType(InPhysicalDevice, MemRequirements.memoryTypeBits, InProperties),
    };
    vkAllocateMemory(InDevice, &AllocInfo, nullptr, &OutImageMemory);

    vkBindImageMemory(InDevice, OutImage, OutImageMemory, 0);
}

} // namespace Private


const std::vector g_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

SEditor::SEditor(const VkExtent2D InViewportExtent, GLFWwindow& InWindow) :
    Scene(std::make_shared<SScene>()), m_ViewportExtent(InViewportExtent)

{
    SShadersLibrary::Init();

    const SLogicalDeviceAndQueues& LogicalDevice = SRendererContext::Get().GetDeviceAndQueues();

    const auto ShaderName = "BlinnPhongColor";

    const SShader* VertexShader   = SShadersLibrary::AddShader(LogicalDevice.GetLogicalDevice(), ShaderName, VK_SHADER_STAGE_VERTEX_BIT);
    const SShader* FragmentShader = SShadersLibrary::AddShader(LogicalDevice.GetLogicalDevice(), ShaderName, VK_SHADER_STAGE_FRAGMENT_BIT);

    Viewport = std::make_unique<SSceneViewport>(Scene, *VertexShader, *FragmentShader);

    Camera.Location = SVectorFlt {-55, 35, 27};
    Camera.Yaw      = -20;
    Camera.Pitch    = -28;
    Camera.Aspect   = static_cast<float>(m_ViewportExtent.width) / static_cast<float>(m_ViewportExtent.height);
    Camera.UpdateView();
    Camera.UpdateProjection();

    CreateViewportImages();

    CreateViewportRenderPass();

    CreateDescriptorSetLayout();

    CreateViewportGraphicsPipeline();

    CreateViewportDepthResources();

    CreateViewportFramebuffers();

    CreateTextureSampler();

    CreateCameraUbos();

    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSyncObjects();

    Scene->LoadExample();

    // #TODO: When to create these buffers?
    for(const auto& SceneObject : Scene->SceneObjects)
    {
        auto* SceneMeshObject = dynamic_cast<SSceneMeshObject*>(SceneObject.get());
        if(!SceneMeshObject)
        {
            continue;
        }

        for(auto& Mesh : SceneMeshObject->Meshes)
        {
            if(!Mesh)
            {
                continue;
            }

            if(!Mesh->Buffer)
            {
                Mesh->Buffer = SVertexIndexBuffer::Create(LogicalDevice.GetLogicalDevice(),
                                                          m_CommandBuffers[m_CurrentFrameIndex],
                                                          LogicalDevice.GetGraphicQueue(),
                                                          {static_cast<const void*>(Mesh->Vertices.data()),
                                                           static_cast<uint32_t>(sizeof(SVertex)),
                                                           static_cast<uint32_t>(Mesh->Vertices.size())},
                                                          {static_cast<const void*>(Mesh->Indices.data()),
                                                           static_cast<uint32_t>(sizeof(uint32_t)),
                                                           static_cast<uint32_t>(Mesh->Indices.size())});
            }
        }
    }

    VkInstance VulkanInstance = SRendererContext::Get().GetVulkanInstance();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();

    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    ImGui_ImplGlfw_InitForVulkan(&InWindow, true);

    // // #TODO : Remove this struct later and refactor ImGui_ImplVulkan_...
    ImGui_ImplVulkan_InitInfo InitInfo = {
        .Instance        = VulkanInstance,
        .PhysicalDevice  = SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(),
        .Device          = LogicalDevice.GetLogicalDevice(),
        .QueueFamily     = LogicalDevice.GetGraphicQueueFamilyIndex(),
        .Queue           = LogicalDevice.GetGraphicQueue(),
        .PipelineCache   = nullptr,
        .DescriptorPool  = m_DescriptorPool,
        .Subpass         = 0,
        .MinImageCount   = 2,
        .ImageCount      = EDITOR_MAX_FRAMES_IN_FLIGHT,
        .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
        .Allocator       = nullptr,
        .CheckVkResultFn = nullptr,
    };
    ImGui_ImplVulkan_Init(&InitInfo, SRendererContext::Get().GetRenderPass().GetVulkanPass());

    // constexpr float FontSize = 17.f;
    // #TODO : Path should be somewhere coded?
    // IO.FontDefault = IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\DroidSans.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Cousine-Regular.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Karla-Regular.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyClean.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyTiny.ttf", FontSize);
    // IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Medium.ttf", FontSize);

    vkResetCommandPool(LogicalDevice.GetLogicalDevice(), SRendererContext::Get().GetCommandPool(), 0);

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

    vkQueueSubmit(LogicalDevice.GetGraphicQueue(), 1, &SubmitInfo, nullptr);

    vkDeviceWaitIdle(LogicalDevice.GetLogicalDevice());

    Private::InitImGuiStyle();

    m_ViewportTextures.resize(m_ViewportImagesViews.size());

    for(uint32_t Index = 0; Index < m_ViewportImages.size(); Index++)
    {
        m_ViewportTextures[Index] = ImGui_ImplVulkan_AddTexture(m_TextureSampler, m_ViewportImagesViews[Index], VK_IMAGE_LAYOUT_GENERAL);
    }
}

SEditor::~SEditor()
{
    // #TODO: It's null
    VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();
    if(!Device)
    {
        return;
    }

    // #TODO: What is this?
    vkDeviceWaitIdle(Device);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyImageView(Device, m_ViewportDepthImageView, nullptr);
    vkDestroyImage(Device, m_ViewportDepthImage, nullptr);
    vkFreeMemory(Device, m_ViewportDepthImageMemory, nullptr);

    vkDestroyPipeline(Device, m_ViewportGraphicsPipelineGrid, nullptr);
    vkDestroyPipeline(Device, m_ViewportGraphicsPipelineMesh, nullptr);
    vkDestroyPipelineLayout(Device, m_ViewportPipelineLayout, nullptr);
    vkDestroyRenderPass(Device, m_ViewportRenderPass, nullptr);

    vkDestroyRenderPass(Device, m_ViewportRenderPass, nullptr);

    vkDestroyDescriptorPool(Device, m_DescriptorPool, nullptr);

    vkDestroySampler(Device, m_TextureSampler, nullptr);

    vkFreeMemory(Device, m_SceneTextureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(Device, m_DescriptorSetLayout, nullptr);

    for(size_t Idx = 0; Idx < EDITOR_MAX_FRAMES_IN_FLIGHT; Idx++)
    {
        vkDestroySemaphore(Device, m_RenderFinishedSemaphores[Idx], nullptr);
        vkDestroySemaphore(Device, m_ImageAvailableSemaphores[Idx], nullptr);
        vkDestroyFence(Device, m_InFlightFences[Idx], nullptr);
    }
}

void SEditor::OnEvent(const SEvent& InEvent)
{
    if(auto* KeyEvent = dynamic_cast<const SKeyboardClickEvent*>(&InEvent))
    {
        if(KeyEvent->GetKey() == GLFW_KEY_W)
        {
            bMoveCameraForward = KeyEvent->GetAction() != GLFW_RELEASE;
        }
        else if(KeyEvent->GetKey() == GLFW_KEY_S)
        {
            bMoveCameraBackward = KeyEvent->GetAction() != GLFW_RELEASE;
        }
        else if(KeyEvent->GetKey() == GLFW_KEY_A)
        {
            bMoveCameraLeft = KeyEvent->GetAction() != GLFW_RELEASE;
        }
        else if(KeyEvent->GetKey() == GLFW_KEY_D)
        {
            bMoveCameraRight = KeyEvent->GetAction() != GLFW_RELEASE;
        }
        else if(KeyEvent->GetKey() == GLFW_KEY_E)
        {
            bMoveCameraUp = KeyEvent->GetAction() != GLFW_RELEASE;
        }
        else if(KeyEvent->GetKey() == GLFW_KEY_Q)
        {
            bMoveCameraDown = KeyEvent->GetAction() != GLFW_RELEASE;
        }
    }
    else if(auto* MouseClick = dynamic_cast<const SMouseClickEvent*>(&InEvent))
    {
        if(MouseClick->GetKey() == GLFW_MOUSE_BUTTON_RIGHT)
        {
            bCameraCanRotate = MouseClick->GetAction() != GLFW_RELEASE;
        }
    }
    else if(auto* MouseMove = dynamic_cast<const SMouseMoveEvent*>(&InEvent))
    {
        const SVector CurrentMouseMove = SVector(MouseMove->m_MousePosX, MouseMove->m_MousePosY);

        if(bCameraCanRotate && (CurrentMouseMove != PreviousMouseMove))
        {
            Camera.LookRight(CurrentMouseMove.X - PreviousMouseMove.X);
            Camera.LookUp(-CurrentMouseMove.Y + PreviousMouseMove.Y);
        }

        PreviousMouseMove = CurrentMouseMove;
    }
}

void SEditor::Tick(const double InFrameTime_Sec)
{
    m_LastFrame_Sec = InFrameTime_Sec;

    if(bMoveCameraForward)
    {
        Camera.MoveForward(1);
    }
    if(bMoveCameraBackward)
    {
        Camera.MoveForward(-1);
    }
    if(bMoveCameraRight)
    {
        Camera.MoveRight(1);
    }
    if(bMoveCameraLeft)
    {
        Camera.MoveRight(-1);
    }
    if(bMoveCameraUp)
    {
        Camera.MoveUp(1);
    }
    if(bMoveCameraDown)
    {
        Camera.MoveUp(-1);
    }
}

void SEditor::BeginFrame()
{
    const VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    // Wait for the previous frame to finish
    vkWaitForFences(Device, 1, &m_InFlightFences[m_CurrentFrameIndex], VK_TRUE, UINT64_MAX);
    vkResetFences(Device, 1, &m_InFlightFences[m_CurrentFrameIndex]);

    vkAcquireNextImageKHR(Device,
                          SRendererContext::Get().GetSwapChain().GetVulkanSwapChain(),
                          UINT64_MAX,
                          m_ImageAvailableSemaphores[m_CurrentFrameIndex],
                          nullptr,
                          &m_CurrentImageIndex);

    vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], 0x00);

    const VkCommandBufferBeginInfo BeginInfo {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = 0,
        .pInheritanceInfo = nullptr,
    };
    vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], &BeginInfo);
}

void SEditor::EndFrame()
{
    vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex]);
}

void SEditor::Render()
{
    // Begin

    const SUniformBufferObject Ubo {
        .m_View = Camera.View,
        .m_Proj = Camera.Projection,
    };

    const SLogicalDeviceAndQueues& DeviceAndQueues = SRendererContext::Get().GetDeviceAndQueues();

    for(auto& CameraUbo : m_CameraUbos)
    {
        CameraUbo->SetData(DeviceAndQueues.GetLogicalDevice(), &Ubo);
    }

    std::array<VkClearValue, 2> ClearColor {};
    ClearColor[0].color        = {{0.0f, 0.0f, 0.0f, 1.0f}};
    ClearColor[1].depthStencil = {1.0f, 0};

    const VkRenderPassBeginInfo RenderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext           = nullptr,
        .renderPass      = m_ViewportRenderPass,
        .framebuffer     = m_ViewportFramebuffers[m_CurrentImageIndex],
        .renderArea      = VkRect2D({0, 0}, m_ViewportExtent),
        .clearValueCount = static_cast<uint32_t>(ClearColor.size()),
        .pClearValues    = ClearColor.data(),
    };
    vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrameIndex], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport {};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = (float)m_ViewportExtent.width;
    viewport.height   = (float)m_ViewportExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = m_ViewportExtent;
    vkCmdSetScissor(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &scissor);

    vkCmdBindDescriptorSets(m_CommandBuffers[m_CurrentFrameIndex],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_ViewportPipelineLayout,
                            0,
                            1,
                            &m_DescriptorSets[m_CurrentFrameIndex],
                            0,
                            nullptr);


    for(const auto& SceneObject : Scene->SceneObjects)
    {
        auto* SceneMeshObject = dynamic_cast<SSceneMeshObject*>(SceneObject.get());
        if(!SceneMeshObject)
        {
            continue;
        }


        vkCmdPushConstants(m_CommandBuffers[m_CurrentFrameIndex],
                           m_ViewportPipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           sizeof(glm::mat4),
                           &SceneObject->ToWorld);

        vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_ViewportGraphicsPipelineMesh);

        for(auto& Mesh : SceneMeshObject->Meshes)
        {
            if(!Mesh || !Mesh->Buffer)
            {
                continue;
            }

            SRenderer::RenderMesh(m_CommandBuffers[m_CurrentFrameIndex], *Mesh->Buffer);
        }
    }

    // End
    vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrameIndex]);
}

void SEditor::BeginGui()
{
    std::array<VkClearValue, 2> ClearColor {};
    ClearColor[0].color        = {{0.0f, 0.0f, 0.0f, 1.0f}};
    ClearColor[1].depthStencil = {1.0f, 0};

    const VkRenderPassBeginInfo RenderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext           = nullptr,
        .renderPass      = SRendererContext::Get().GetRenderPass().GetVulkanPass(),
        .framebuffer     = SRendererContext::Get().GetSwapChain().GetFramebuffer(m_CurrentImageIndex),
        .renderArea      = VkRect2D({0, 0}, SRendererContext::Get().GetSwapChain().GetSize()),
        .clearValueCount = static_cast<uint32_t>(ClearColor.size()),
        .pClearValues    = ClearColor.data(),
    };
    vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrameIndex], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void SEditor::RenderGui()
{
    ImGuiStyle& Style               = ImGui::GetStyle();
    ImGuiIO&    IO                  = ImGui::GetIO();
    IO.ConfigWindowsResizeFromEdges = IO.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

    const ImGuiViewport& Viewport = *ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(Viewport.Pos);
    ImGui::SetNextWindowSize(Viewport.Size);
    ImGui::SetNextWindowViewport(Viewport.ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    constexpr ImGuiWindowFlags EditorFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                             ImGuiWindowFlags_NoDocking;

    ImGui::Begin("Editor", nullptr, EditorFlags);
    {
        ImGui::PopStyleVar(2);

        ImGui::BeginChild("Viewport", ImVec2(), false, ImGuiWindowFlags_NoDecoration);
        {
            if(m_LastViewportTexture)
            {
                const ImVec2 ViewportSize(static_cast<float>(m_ViewportExtent.width), static_cast<float>(m_ViewportExtent.height));

                ImGui::Image(m_LastViewportTexture, ViewportSize);
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void SEditor::EndGui()
{
    ImGui::Render();

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[m_CurrentFrameIndex]);

    vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrameIndex]);
}

void SEditor::Present()
{
    constexpr VkPipelineStageFlags WaitStages(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

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
    vkQueueSubmit(SRendererContext::Get().GetDeviceAndQueues().GetGraphicQueue(), 1, &SubmitInfo, m_InFlightFences[m_CurrentFrameIndex]);

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    VkSwapchainKHR Swapchain = SRendererContext::Get().GetSwapChain().GetVulkanSwapChain();

    const VkPresentInfoKHR PresentInfo {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &m_RenderFinishedSemaphores[m_CurrentFrameIndex],
        .swapchainCount     = 1,
        .pSwapchains        = &Swapchain,
        .pImageIndices      = &m_CurrentImageIndex,
        .pResults           = nullptr,
    };
    vkQueuePresentKHR(SRendererContext::Get().GetDeviceAndQueues().GetPresentQueue(), &PresentInfo);

    m_LastViewportTexture = m_ViewportTextures[m_CurrentFrameIndex];

    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % EDITOR_MAX_FRAMES_IN_FLIGHT;
}

void SEditor::CreateViewportImages()
{
    const uint32_t FramebuffersCount = SRendererContext::Get().GetSwapChain().GetFramebuffersCount();

    m_ViewportImages.resize(FramebuffersCount);
    m_ViewportImagesMemories.resize(FramebuffersCount);
    m_ViewportImagesViews.resize(m_ViewportImages.size());

    for(uint32_t i = 0; i < m_ViewportImages.size(); i++)
    {
        Private::CreateImage(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                             SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(),
                             m_ViewportExtent.width,
                             m_ViewportExtent.height,
                             m_ViewportImageFormat,
                             VK_IMAGE_TILING_OPTIMAL,
                             VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                             m_ViewportImages[i],
                             m_ViewportImagesMemories[i]);

        m_ViewportImagesViews[i] = CreateImageView(m_ViewportImages[i], m_ViewportImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void SEditor::CreateViewportRenderPass()
{
    const VkAttachmentDescription ColorAttachment {
        .format         = m_ViewportImageFormat,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_GENERAL,
    };

    const VkAttachmentDescription DepthAttachment {
        .format         = FindDepthFormat(),
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    const VkAttachmentReference ColorAttachmentRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    const VkAttachmentReference DepthAttachmentRef {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    const VkSubpassDescription Subpass {
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount    = 1,
        .pColorAttachments       = &ColorAttachmentRef,
        .pDepthStencilAttachment = &DepthAttachmentRef,
    };

    const VkSubpassDependency Dependency {
        .srcSubpass    = VK_SUBPASS_EXTERNAL,
        .dstSubpass    = 0,
        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };

    const std::array Attachments = {ColorAttachment, DepthAttachment};

    const VkRenderPassCreateInfo RenderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(Attachments.size()),
        .pAttachments    = Attachments.data(),
        .subpassCount    = 1,
        .pSubpasses      = &Subpass,
        .dependencyCount = 1,
        .pDependencies   = &Dependency,
    };
    vkCreateRenderPass(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &RenderPassInfo, nullptr, &m_ViewportRenderPass);
}

void SEditor::CreateDescriptorSetLayout()
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

    if(vkCreateDescriptorSetLayout(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                                   &layoutInfo,
                                   nullptr,
                                   &m_DescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void SEditor::CreateViewportGraphicsPipeline()
{
    const VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    const char* ShaderName = "BlinnPhongColor";

    VkPipelineShaderStageCreateInfo vertShaderStageInfo =
        SShadersLibrary::AddShader(Device, ShaderName, VK_SHADER_STAGE_VERTEX_BIT)->GetInfo();
    VkPipelineShaderStageCreateInfo fragShaderStageInfo =
        SShadersLibrary::AddShader(Device, ShaderName, VK_SHADER_STAGE_FRAGMENT_BIT)->GetInfo();

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    const VkVertexInputBindingDescription&                  bindingDescription    = SVertex::GetBindingDescription();
    const std::array<VkVertexInputAttributeDescription, 3>& attributeDescriptions = SVertex::GetAttributesDescriptions();

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

    constexpr VkPushConstantRange PushConstantRange {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset     = 0,
        .size       = sizeof(glm::mat4),
    };

    const VkPipelineLayoutCreateInfo PipelineLayoutInfo {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = nullptr,
        .setLayoutCount         = 1,
        .pSetLayouts            = &m_DescriptorSetLayout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges    = &PushConstantRange,
    };

    vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &m_ViewportPipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext               = nullptr;
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
    pipelineInfo.layout              = m_ViewportPipelineLayout;
    pipelineInfo.renderPass          = m_ViewportRenderPass;
    pipelineInfo.subpass             = 0;
    pipelineInfo.basePipelineHandle  = nullptr;

    vkCreateGraphicsPipelines(Device, nullptr, 1, &pipelineInfo, nullptr, &m_ViewportGraphicsPipelineMesh);
}

void SEditor::CreateViewportFramebuffers()
{
    m_ViewportFramebuffers.resize(m_ViewportImagesViews.size());

    for(size_t i = 0; i < m_ViewportImagesViews.size(); i++)
    {
        std::array Attachments = {m_ViewportImagesViews[i], m_ViewportDepthImageView};

        const VkFramebufferCreateInfo FramebufferInfo {
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .renderPass      = m_ViewportRenderPass,
            .attachmentCount = static_cast<uint32_t>(Attachments.size()),
            .pAttachments    = Attachments.data(),
            .width           = m_ViewportExtent.width,
            .height          = m_ViewportExtent.height,
            .layers          = 1,
        };
        vkCreateFramebuffer(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                            &FramebufferInfo,
                            nullptr,
                            &m_ViewportFramebuffers[i]);
    }
}

void SEditor::CreateViewportDepthResources()
{
    const VkFormat DepthFormat = FindDepthFormat();

    Private::CreateImage(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                         SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(),
                         m_ViewportExtent.width,
                         m_ViewportExtent.height,
                         DepthFormat,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         m_ViewportDepthImage,
                         m_ViewportDepthImageMemory);
    m_ViewportDepthImageView = CreateImageView(m_ViewportDepthImage, DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat
    SEditor::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features)
{
    for(VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(), format, &props);

        if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    return VK_FORMAT_UNDEFINED;
}

VkFormat SEditor::FindDepthFormat()
{
    return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool SEditor::HasStencilComponent(const VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void SEditor::CreateTextureSampler()
{
    VkPhysicalDeviceProperties properties {};
    vkGetPhysicalDeviceProperties(SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(), &properties);

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

    vkCreateSampler(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler);
}

VkImageView SEditor::CreateImageView(const VkImage image, const VkFormat format, const VkImageAspectFlags aspectFlags)
{
    VkImageView OutImageView;

    const VkImageViewCreateInfo ViewInfo {
        .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext    = nullptr,
        .image    = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format   = format,
        .subresourceRange =
            {
                .aspectMask     = aspectFlags,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
    };
    vkCreateImageView(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &ViewInfo, nullptr, &OutImageView);

    return OutImageView;
}

void SEditor::TransitionImageLayout(const VkImage image, VkFormat format, const VkImageLayout oldLayout, const VkImageLayout newLayout)
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

void SEditor::CopyBufferToImage(const VkBuffer buffer, const VkImage image, const uint32_t width, const uint32_t height)
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

void SEditor::CreateCameraUbos()
{
    for(size_t i = 0; i < EDITOR_MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_CameraUbos[i] = SUniformBuffer::Create(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice());
    }
}

void SEditor::CreateDescriptorPool()
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
    vkCreateDescriptorPool(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &CreateInfo, nullptr, &m_DescriptorPool);
}

void SEditor::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(EDITOR_MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
    VkDescriptorSetAllocateInfo        allocInfo {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = m_DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(EDITOR_MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts        = layouts.data();

    m_DescriptorSets.resize(EDITOR_MAX_FRAMES_IN_FLIGHT);
    if(vkAllocateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &allocInfo, m_DescriptorSets.data()) !=
       VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for(size_t i = 0; i < EDITOR_MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = m_CameraUbos[i]->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(SUniformBufferObject);

        VkDescriptorImageInfo imageInfo {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView   = m_ViewportImagesViews[i];
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

        vkUpdateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(),
                               0,
                               nullptr);
    }
}

void SEditor::CreateBuffer(const VkDeviceSize          size,
                           const VkBufferUsageFlags    usage,
                           const VkMemoryPropertyFlags properties,
                           VkBuffer&                   buffer,
                           VkDeviceMemory&             bufferMemory)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size        = size;
    bufferInfo.usage       = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    const VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    if(vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        Private::FindMemoryType(SRendererContext::Get().GetGraphicCard().GetPhysicalDevice(), memRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Device, buffer, bufferMemory, 0);
}

VkCommandBuffer SEditor::BeginSingleTimeCommands()
{
    VkCommandBuffer OutCommandBuffer;

    const VkCommandBufferAllocateInfo CmdBufferAllocInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = SRendererContext::Get().GetCommandPool(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    vkAllocateCommandBuffers(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &CmdBufferAllocInfo, &OutCommandBuffer);

    const VkCommandBufferBeginInfo CmdBufferBeginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    vkBeginCommandBuffer(OutCommandBuffer, &CmdBufferBeginInfo);

    return OutCommandBuffer;
}

void SEditor::EndSingleTimeCommands(const VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(SRendererContext::Get().GetDeviceAndQueues().GetGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(SRendererContext::Get().GetDeviceAndQueues().GetGraphicQueue());

    vkFreeCommandBuffers(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                         SRendererContext::Get().GetCommandPool(),
                         1,
                         &commandBuffer);
}

void SEditor::CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

void SEditor::CreateCommandBuffers()
{
    m_CommandBuffers.resize(EDITOR_MAX_FRAMES_IN_FLIGHT);

    const VkCommandBufferAllocateInfo CmdBufferAllocInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = SRendererContext::Get().GetCommandPool(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size()),
    };
    vkAllocateCommandBuffers(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &CmdBufferAllocInfo, m_CommandBuffers.data());
}

void SEditor::CreateSyncObjects()
{
    m_ImageAvailableSemaphores.resize(EDITOR_MAX_FRAMES_IN_FLIGHT);
    m_RenderFinishedSemaphores.resize(EDITOR_MAX_FRAMES_IN_FLIGHT);
    m_InFlightFences.resize(EDITOR_MAX_FRAMES_IN_FLIGHT);

    const VkSemaphoreCreateInfo SemaphoreInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
    };

    const VkFenceCreateInfo FenceInfo {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    const VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    for(size_t i = 0; i < EDITOR_MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);
        vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);
        vkCreateFence(Device, &FenceInfo, nullptr, &m_InFlightFences[i]);
    }
}

} // namespace South