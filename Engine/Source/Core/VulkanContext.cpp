#include "sthpch.h"

#include "Core/VulkanContext.h"

#include "Core/Application.h"
#include "Core/VulkanVertexIndexBuffer.h"
#include "Core/Shaders/VulkanShader.h"
#include "Core/Shaders/ShadersLibrary.h"

#include "Editor/Mesh.h"
#include "Editor/Camera.h"

#include <GLFW/glfw3.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>

namespace South
{
    std::random_device Dev;
    std::mt19937 Rng(Dev());
    std::uniform_real_distribution<float> Dist(0.f, 1.f);

    // NDC space
    const std::vector<Vertex> Vertices = { {
                                               glm::vec3(-0.5f, -0.5f, 0.f),
                                               glm::vec3(0.f),
                                               glm::vec2(0.f),
                                               glm::vec3(1.f, 1.f, 1),
                                           },
                                           {
                                               glm::vec3(0.5f, -0.5f, 0.f),
                                               glm::vec3(0.f),
                                               glm::vec2(0.f),
                                               glm::vec3(Dist(Rng), Dist(Rng), Dist(Rng)),
                                           },
                                           {
                                               glm::vec3(0.5f, 0.5f, 0.f),
                                               glm::vec3(0.f),
                                               glm::vec2(0.f),
                                               glm::vec3(Dist(Rng), Dist(Rng), Dist(Rng)),
                                           },
                                           {
                                               glm::vec3(-0.5f, 0.5f, 0.f),
                                               glm::vec3(0.f),
                                               glm::vec2(0.f),
                                               glm::vec3(Dist(Rng), Dist(Rng), Dist(Rng)),
                                           } };

    const std::vector<uint32_t> Indices = { 0, 1, 2, 2, 3, 0 };

    // Model/projection are not changing every frame - should be in uniform (desriptor buffer)
    // Projection too.
    struct PushConstant
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    };

    Camera EditorCam;
    PushConstant PushConstant;

    VulkanContext& VulkanContext::Get()
    {
        static VulkanContext Instance;
        return Instance;
    }

    void VulkanContext::Init()
    {
        auto* GlfwWindow = Application::Get().GetWindow().GetglfwWindow();
        if (!GlfwWindow)
        {
            return;
        }

        CreateInstance();
        CreateSurface(*GlfwWindow);

        Device = std::make_unique<VulkanDevice>();
        Device->Init(Surface);

        ShadersLibrary& ShadersLib = ShadersLibrary::Get();
        ShadersLib.Init();
        ShadersLib.AddShader("Base_V", "Resources/Shaders/Base.vert", VK_SHADER_STAGE_VERTEX_BIT);
        ShadersLib.AddShader("Base_F", "Resources/Shaders/Base.frag", VK_SHADER_STAGE_FRAGMENT_BIT);

        CreateSwapChain(*GlfwWindow);
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateModelBuffers();
        CreateCommands();
        CreateSyncObjects();

        EditorCam.SetView(glm::vec3(2.f, 0.f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        EditorCam.SetProjection(glm::radians(70.0f), SwapChainExtent.width / (float)SwapChainExtent.height, 0.1f,
                                200.0f);

        PushConstant.Model      = glm::mat4(1.f);
        PushConstant.View       = EditorCam.GetViewMatrix();
        PushConstant.Projection = EditorCam.GetProjectionMatrix();

        bCanTick = true;
    }

    void VulkanContext::DeInit()
    {
        ShadersLibrary::Get().DeInit();

        VkDevice LogicDevice = Device->GetDevice();

        vkDestroySemaphore(LogicDevice, ImageAvailableSemaphore, nullptr);
        vkDestroySemaphore(LogicDevice, RenderFinishedSemaphore, nullptr);
        vkDestroyFence(LogicDevice, FlightFence, nullptr);

        vkDestroyCommandPool(LogicDevice, CommandPool, nullptr);

        for (auto framebuffer : SwapChainFramebuffers)
        {
            vkDestroyFramebuffer(LogicDevice, framebuffer, nullptr);
        }

        vkDestroyPipeline(LogicDevice, GraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(LogicDevice, PipelineLayout, nullptr);
        vkDestroyRenderPass(LogicDevice, RenderPass, nullptr);

        vkDestroySurfaceKHR(VulkanInstance, Surface, nullptr);

        for (auto ImageView : SwapChainImageViews)
        {
            vkDestroyImageView(LogicDevice, ImageView, nullptr);
        }

        vkDestroySwapchainKHR(LogicDevice, SwapChain, nullptr);

        delete VI_Buffer;

        Device->DeInit();

        vkDestroyInstance(VulkanInstance, nullptr);

        bCanTick = false;
    }

    void VulkanContext::Tick()
    {
        if (!bCanTick)
        {
            return;
        }

        // Animation
        {
            static int16_t i = 0;
            ++i;
            PushConstant.Model = glm::rotate(PushConstant.Model, glm::radians(.01f), glm::vec3(0.0f, 0.0f, 1.0f));
            PushConstant.Model = glm::scale(PushConstant.Model, (i > 0) ? glm::vec3(1.00005f) : glm::vec3(0.99995f));
        }

        VkDevice LogicDevice  = Device->GetDevice();
        VkQueue GraphicsQueue = Device->GetQ();

        // Wait for the previous frame to finish
        vkWaitForFences(LogicDevice, 1, &FlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(LogicDevice, 1, &FlightFence);

        // Acquire an image from the swap chain
        uint32_t ImageIndex = 0;
        vkAcquireNextImageKHR(LogicDevice, SwapChain, UINT64_MAX, ImageAvailableSemaphore, VK_NULL_HANDLE, &ImageIndex);

        // Submit the recorded command buffer
        vkResetCommandBuffer(CommandBuffer, 0);
        RecordCommandBuffer(CommandBuffer, ImageIndex);

        VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore WaitSemaphores[]      = { ImageAvailableSemaphore };
        VkSemaphore SignalSemaphores[]    = { RenderFinishedSemaphore };

        VkSubmitInfo SubmitInfo{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                = nullptr,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = WaitSemaphores,
            .pWaitDstStageMask    = WaitStages,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &CommandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = SignalSemaphores,
        };

        vkQueueSubmit(Device->GetQ(), 1, &SubmitInfo, FlightFence);

        VkSwapchainKHR SwapChains[] = { SwapChain };

        // Present the swap chain image
        VkPresentInfoKHR SresentInfo{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = SignalSemaphores,
            .swapchainCount     = 1,
            .pSwapchains        = SwapChains,
            .pImageIndices      = &ImageIndex,
            .pResults           = nullptr,
        };

        vkQueuePresentKHR(GraphicsQueue, &SresentInfo);
    }

    VkInstance VulkanContext::GetVulkanInstance()
    {
        return VulkanInstance;
    }

    VulkanDevice& VulkanContext::GetCurrentDevice()
    {
        return *Device;
    }


    void VulkanContext::CreateInstance()
    {
        VkApplicationInfo sAppInfo{
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = nullptr,
            .pApplicationName   = GetAppName(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "None",
            .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
            .apiVersion         = VK_API_VERSION_1_0,
        };

        // Extensions
        uint32_t extensionCount = 0;
        const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        VkInstanceCreateInfo sCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                   = nullptr,
            .pApplicationInfo        = &sAppInfo,
            .enabledLayerCount       = 0,
            .enabledExtensionCount   = extensionCount,
            .ppEnabledExtensionNames = extensions,
        };

        vkCreateInstance(&sCreateInfo, nullptr, &VulkanInstance);
    }

    void VulkanContext::CreateSurface(GLFWwindow& Window)
    {
        if (!glfwCreateWindowSurface(VulkanInstance, &Window, nullptr, &Surface))
        {
            return /*error*/;
        }
    }

    void VulkanContext::CreateSwapChain(GLFWwindow& window)
    {
        VkPhysicalDevice physDevice = Device->GetPhysicalDevice();
        VkDevice logicDevice        = Device->GetDevice();
        uint32_t QueueFamilyIndex   = Device->GetQFamilyIndex();

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, Surface, &capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(physDevice, Surface);
        VkPresentModeKHR presentMode     = ChooseSwapPresentMode(physDevice, Surface);
        VkExtent2D extent                = ChooseSwapExtent(capabilities, window);

        VkSwapchainCreateInfoKHR createInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = nullptr,
            .surface               = Surface,
            .minImageCount         = imageCount,
            .imageFormat           = surfaceFormat.format,
            .imageColorSpace       = surfaceFormat.colorSpace,
            .imageExtent           = extent,
            .imageArrayLayers      = 1,
            .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices   = &QueueFamilyIndex,
            .preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode           = presentMode,
            .clipped               = VK_TRUE,
            .oldSwapchain          = VK_NULL_HANDLE,
        };

        vkCreateSwapchainKHR(logicDevice, &createInfo, nullptr, &SwapChain);

        vkGetSwapchainImagesKHR(logicDevice, SwapChain, &imageCount, nullptr);

        SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicDevice, SwapChain, &imageCount, SwapChainImages.data());

        SwapChainImageFormat = createInfo.imageFormat;
        SwapChainExtent      = createInfo.imageExtent;
    }

    void VulkanContext::CreateImageViews()
    {
        VkDevice logicDevice = Device->GetDevice();

        SwapChainImageViews.resize(SwapChainImages.size());

        VkImageSubresourceRange subresourceRange{
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        for (size_t i = 0; i < SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo sCreateInfo{
                .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext            = nullptr,
                .image            = SwapChainImages[i],
                .viewType         = VK_IMAGE_VIEW_TYPE_2D,
                .format           = SwapChainImageFormat,
                .components       = VkComponentMapping(VK_COMPONENT_SWIZZLE_IDENTITY),
                .subresourceRange = subresourceRange,
            };

            vkCreateImageView(logicDevice, &sCreateInfo, nullptr, &SwapChainImageViews[i]);
        }
    }

    void VulkanContext::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{
            .format         = SwapChainImageFormat,
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        VkAttachmentReference colorAttachmentRef{
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

        VkSubpassDescription subpass{
            .pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments    = &colorAttachmentRef,
        };

        VkSubpassDependency dependency{
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        };

        VkRenderPassCreateInfo renderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext           = nullptr,
            .attachmentCount = 1,
            .pAttachments    = &colorAttachment,
            .subpassCount    = 1,
            .pSubpasses      = &subpass,
            .dependencyCount = 1,
            .pDependencies   = &dependency,
        };

        vkCreateRenderPass(Device->GetDevice(), &renderPassInfo, nullptr, &RenderPass);
    }

    void VulkanContext::CreateGraphicsPipeline()
    {
        VkDevice logicDevice = Device->GetDevice();

        // #TODO : Check for errors;
        ShadersLibrary& ShadersLib = ShadersLibrary::Get();
        const auto& vertShader     = *ShadersLib.GetShader("Base_V");
        const auto& fragShader     = *ShadersLib.GetShader("Base_F");

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShader.GetInfo(), fragShader.GetInfo() };

        const auto& bindingDesc     = Vertex::GetBindingDescription();
        const auto& attributesDescs = Vertex::GetAttributesDescriptions();

        // Inputs to vertext shader.
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .vertexBindingDescriptionCount   = 1,
            .pVertexBindingDescriptions      = &bindingDesc,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributesDescs.size()),
            .pVertexAttributeDescriptions    = attributesDescs.data(),
        };

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        // Viewport and scissors.
        VkViewport viewport{
            .x        = 0.0f,
            .y        = 0.0f,
            .width    = (float)SwapChainExtent.width,
            .height   = (float)SwapChainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor{
            .offset = { 0, 0 },
            .extent = SwapChainExtent,
        };

        VkPipelineViewportStateCreateInfo viewportState{
            .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext         = nullptr,
            .viewportCount = 1,
            .pViewports    = &viewport,
            .scissorCount  = 1,
            .pScissors     = &scissor,
        };

        // Rasterizer.
        VkPipelineRasterizationStateCreateInfo rasterizer{
            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext                   = nullptr,
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = VK_POLYGON_MODE_FILL,
            .cullMode                = VK_CULL_MODE_BACK_BIT,
            .frontFace               = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable         = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp          = 0.0f,
            .depthBiasSlopeFactor    = 0.0f,
            .lineWidth               = 1.0f,
        };

        // Multisampling - antialiasing
        VkPipelineMultisampleStateCreateInfo multisampling{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable   = VK_FALSE,
            .minSampleShading      = 1.0f,
            .pSampleMask           = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable      = VK_FALSE,
        };

        // #TODO : Depth and stecncil testing.

        // Color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment{
            .blendEnable         = VK_TRUE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp        = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp        = VK_BLEND_OP_ADD,
            .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                              VK_COLOR_COMPONENT_A_BIT,
        };

        VkPipelineColorBlendStateCreateInfo colorBlending{
            .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext           = nullptr,
            .logicOpEnable   = VK_FALSE,
            .logicOp         = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments    = &colorBlendAttachment,
            .blendConstants  = { 0.0f, 0.0f, 0.0f, 0.0f },
        };

        // Dynamic state - dynamically change pipeline paremeters.
        std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

        VkPipelineDynamicStateCreateInfo dynamicState{
            .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext             = nullptr,
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates    = dynamicStates.data(),
        };

        // Pipeline layout - uniforms in shaders.
        VkPushConstantRange pushConstantRange{
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = sizeof(PushConstant),
        };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = nullptr,
            .setLayoutCount         = 0,
            .pSetLayouts            = nullptr,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &pushConstantRange,
        };

        vkCreatePipelineLayout(logicDevice, &pipelineLayoutInfo, nullptr, &PipelineLayout);

        VkGraphicsPipelineCreateInfo pipelineInfo{
            .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext               = nullptr,
            .stageCount          = 2,
            .pStages             = shaderStages,
            .pVertexInputState   = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pTessellationState  = nullptr,
            .pViewportState      = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState   = &multisampling,
            .pDepthStencilState  = nullptr,
            .pColorBlendState    = &colorBlending,
            //.pDynamicState       = &dynamicState,
            .layout             = PipelineLayout,
            .renderPass         = RenderPass,
            .subpass            = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex  = -1,
        };

        vkCreateGraphicsPipelines(logicDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &GraphicsPipeline);
    }

    void VulkanContext::CreateFramebuffers()
    {
        VkDevice LogicDevice = Device->GetDevice();

        SwapChainFramebuffers.resize(SwapChainImageViews.size());

        for (auto i = 0; i < SwapChainImageViews.size(); i++)
        {
            VkImageView Attachments[] = { SwapChainImageViews[i] };

            VkFramebufferCreateInfo FramebufferInfo{
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext           = nullptr,
                .renderPass      = RenderPass,
                .attachmentCount = 1,
                .pAttachments    = Attachments,
                .width           = SwapChainExtent.width,
                .height          = SwapChainExtent.height,
                .layers          = 1,
            };

            vkCreateFramebuffer(LogicDevice, &FramebufferInfo, nullptr, &SwapChainFramebuffers[i]);
        }
    }

    void VulkanContext::CreateModelBuffers()
    {
        VI_Buffer = new VulkanVertexIndexBuffer(
            static_cast<const void*>(Vertices.data()), static_cast<uint32_t>(sizeof(Vertices[0]) * Vertices.size()),
            static_cast<const void*>(Indices.data()), static_cast<uint32_t>(sizeof(Indices[0]) * Indices.size()));
    }

    void VulkanContext::CreateCommands()
    {
        VkDevice logicDevice = Device->GetDevice();

        VkCommandPoolCreateInfo PoolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = Device->GetQFamilyIndex(),
        };

        vkCreateCommandPool(logicDevice, &PoolInfo, nullptr, &CommandPool);

        VkCommandBufferAllocateInfo AllocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = CommandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        vkAllocateCommandBuffers(logicDevice, &AllocInfo, &CommandBuffer);
    }

    void VulkanContext::CreateSyncObjects()
    {
        VkDevice LogicDevice = Device->GetDevice();

        VkSemaphoreCreateInfo SemaphoreInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
        };

        VkFenceCreateInfo FenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

        vkCreateSemaphore(LogicDevice, &SemaphoreInfo, nullptr, &ImageAvailableSemaphore);
        vkCreateSemaphore(LogicDevice, &SemaphoreInfo, nullptr, &RenderFinishedSemaphore);
        vkCreateFence(LogicDevice, &FenceInfo, nullptr, &FlightFence);
    }

    VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
    {
        uint32_t FormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> AvailableFormats(FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, AvailableFormats.data());

        for (const auto& Format : AvailableFormats)
        {
            if (Format.format == VK_FORMAT_B8G8R8A8_SRGB && Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return Format;
            }
        }

        return AvailableFormats[0];
    }

    VkPresentModeKHR VulkanContext::ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
    {
        // Presentation mode.
        uint32_t PresentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount, nullptr);

        std::vector<VkPresentModeKHR> AvailablePresentModes(PresentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount,
                                                  AvailablePresentModes.data());

        for (const auto& PresentMode : AvailablePresentModes)
        {
            if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return PresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, GLFWwindow& Window)
    {
        if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return Capabilities.currentExtent;
        }
        else
        {
            int Width, Height;
            glfwGetFramebufferSize(&Window, &Width, &Height);

            VkExtent2D ActualExtent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height) };

            ActualExtent.width =
                std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
            ActualExtent.height =
                std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

            return ActualExtent;
        }
    }

    void VulkanContext::RecordCommandBuffer(VkCommandBuffer Buffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo BeginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = 0,
            .pInheritanceInfo = nullptr,
        };

        vkBeginCommandBuffer(Buffer, &BeginInfo);

        // #TODO : Pipeline static value?
        VkClearValue ClearColor = { { 0.f, 0.f, 0.f, 1.f } };

        VkRenderPassBeginInfo RenderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext           = nullptr,
            .renderPass      = RenderPass,
            .framebuffer     = SwapChainFramebuffers[imageIndex],
            .renderArea      = VkRect2D({ 0, 0 }, SwapChainExtent),
            .clearValueCount = 1,
            .pClearValues    = &ClearColor,
        };

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);

        // Draw
        {
            vkCmdPushConstants(CommandBuffer, PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant),
                               &PushConstant);

            VkBuffer MeshBuffer = VI_Buffer->GetVulkanBuffer();
            VkDeviceSize Offset = 0;
            vkCmdBindVertexBuffers(CommandBuffer, 0, 1, &MeshBuffer, &Offset);
            vkCmdBindIndexBuffer(CommandBuffer, MeshBuffer, VI_Buffer->GetIndexOffset(), VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(CommandBuffer, static_cast<uint32_t>(Indices.size()), 1, 0, 0, 0);
        }

        vkCmdEndRenderPass(CommandBuffer);

        vkEndCommandBuffer(CommandBuffer);
    }

} // namespace South
