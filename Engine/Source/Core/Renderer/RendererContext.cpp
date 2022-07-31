#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RendererContext.h"
#include "Core/Shaders/ShadersLibrary.h"
#include "Core/Shaders/VulkanShader.h"
#include "Core/VulkanDevice.h"
#include "Editor/Mesh.h"
#include <GLFW/glfw3.h>

namespace South
{

    void RendererContext::Init()
    {
        auto* GlfwWindow = Application::Get().GetWindow().GetglfwWindow();
        if (!GlfwWindow) { return; }

        if (m_bEnableValidationLayers && CheckValidationLayers()) { STH_VK_WARN("ValidationsLayers not found."); }

        CreateInstance();

        if (m_bEnableValidationLayers) { CreateMessenger(); }

        CreateSurface(*GlfwWindow);

        m_Device = std::make_unique<VulkanDevice>();
        m_Device->Init(m_Surface);

        ShadersLibrary::Init();
        ShadersLibrary::AddShader("Base_V", "Resources/Shaders/Base.vert", VK_SHADER_STAGE_VERTEX_BIT);
        ShadersLibrary::AddShader("Base_F", "Resources/Shaders/Base.frag", VK_SHADER_STAGE_FRAGMENT_BIT);

        CreateSwapChain(*GlfwWindow);
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommands();
        CreateSyncObjects();

        STH_INFO("RendererContext Initialized.");
    }

    void RendererContext::DeInit()
    {
        ShadersLibrary::DeInit();

        VkDevice LogicDevice = m_Device->GetDevice();

        // #TODO : Error with commandbuffer still in use.
        vkDestroyCommandPool(LogicDevice, m_CommandPool, nullptr);

        for (auto framebuffer : m_SwapChainFramebuffers) { vkDestroyFramebuffer(LogicDevice, framebuffer, nullptr); }

        vkDestroyPipeline(LogicDevice, m_GraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(LogicDevice, m_PipelineLayout, nullptr);
        vkDestroyRenderPass(LogicDevice, m_RenderPass, nullptr);

        for (auto ImageView : m_SwapChainImageViews) { vkDestroyImageView(LogicDevice, ImageView, nullptr); }

        vkDestroySwapchainKHR(LogicDevice, m_SwapChain, nullptr);


        m_Device->DeInit();

        if (m_bEnableValidationLayers) { DestroyMessenger(); }

        vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);

        vkDestroyInstance(m_VulkanInstance, nullptr);

        STH_INFO("RendererContext Deinitialized.");
    }

    void RendererContext::CreateInstance()
    {
        const VkApplicationInfo sAppInfo{
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = nullptr,
            .pApplicationName   = Application::GetName(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "None",
            .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
            .apiVersion         = VK_MAKE_API_VERSION(0, 1, 3, 0),
        };

        const std::vector<const char*> Extensions = GetRequiredInstanceExtensions();

        const VkInstanceCreateInfo sCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext            = nullptr,
            .pApplicationInfo = &sAppInfo,
            .enabledLayerCount =
                m_bEnableValidationLayers ? static_cast<uint32_t>(m_RequiredValidationLayers.size()) : 0,
            .ppEnabledLayerNames     = m_bEnableValidationLayers ? m_RequiredValidationLayers.data() : nullptr,
            .enabledExtensionCount   = static_cast<uint32_t>(Extensions.size()),
            .ppEnabledExtensionNames = Extensions.data(),
        };

        vkCreateInstance(&sCreateInfo, nullptr, &m_VulkanInstance);
    }

    void RendererContext::CreateSurface(GLFWwindow& Window)
    {
        if (!glfwCreateWindowSurface(m_VulkanInstance, &Window, nullptr, &m_Surface)) { return /*error*/; }
    }

    void RendererContext::CreateSwapChain(GLFWwindow& window)
    {
        VkPhysicalDevice physDevice = m_Device->GetPhysicalDevice();
        VkDevice logicDevice        = m_Device->GetDevice();
        uint32_t QueueFamilyIndex   = m_Device->GetQFamilyIndex();

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, m_Surface, &capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(physDevice, m_Surface);
        const VkPresentModeKHR presentMode     = ChooseSwapPresentMode(physDevice, m_Surface);
        const VkExtent2D extent                = ChooseSwapExtent(capabilities, window);

        const VkSwapchainCreateInfoKHR createInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = nullptr,
            .surface               = m_Surface,
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

        vkCreateSwapchainKHR(logicDevice, &createInfo, nullptr, &m_SwapChain);

        vkGetSwapchainImagesKHR(logicDevice, m_SwapChain, &imageCount, nullptr);

        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicDevice, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = createInfo.imageFormat;
        m_SwapChainExtent      = createInfo.imageExtent;
    }

    void RendererContext::CreateImageViews()
    {
        VkDevice logicDevice = m_Device->GetDevice();

        m_SwapChainImageViews.resize(m_SwapChainImages.size());

        const VkImageSubresourceRange subresourceRange{
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        for (size_t i = 0; i < m_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo sCreateInfo{
                .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext            = nullptr,
                .image            = m_SwapChainImages[i],
                .viewType         = VK_IMAGE_VIEW_TYPE_2D,
                .format           = m_SwapChainImageFormat,
                .components       = VkComponentMapping(VK_COMPONENT_SWIZZLE_IDENTITY),
                .subresourceRange = subresourceRange,
            };

            vkCreateImageView(logicDevice, &sCreateInfo, nullptr, &m_SwapChainImageViews[i]);
        }
    }

    void RendererContext::CreateRenderPass()
    {
        const VkAttachmentDescription colorAttachment{
            .format         = m_SwapChainImageFormat,
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        const VkAttachmentReference colorAttachmentRef{
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

        const VkSubpassDescription subpass{
            .pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments    = &colorAttachmentRef,
        };

        const VkSubpassDependency dependency{
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        };

        const VkRenderPassCreateInfo renderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext           = nullptr,
            .attachmentCount = 1,
            .pAttachments    = &colorAttachment,
            .subpassCount    = 1,
            .pSubpasses      = &subpass,
            .dependencyCount = 1,
            .pDependencies   = &dependency,
        };

        vkCreateRenderPass(m_Device->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass);
    }

    void RendererContext::CreateGraphicsPipeline()
    {
        VkDevice logicDevice = m_Device->GetDevice();

        // #TODO : Check for errors;
        const auto& vertShader = *ShadersLibrary::GetShader("Base_V");
        const auto& fragShader = *ShadersLibrary::GetShader("Base_F");

        const VkPipelineShaderStageCreateInfo shaderStages[] = { vertShader.GetInfo(), fragShader.GetInfo() };

        const auto& bindingDesc     = Vertex::GetBindingDescription();
        const auto& attributesDescs = Vertex::GetAttributesDescriptions();

        // Inputs to vertext shader.
        const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .vertexBindingDescriptionCount   = 1,
            .pVertexBindingDescriptions      = &bindingDesc,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributesDescs.size()),
            .pVertexAttributeDescriptions    = attributesDescs.data(),
        };

        const VkPipelineInputAssemblyStateCreateInfo inputAssembly{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        // Viewport and scissors.
        const VkViewport viewport{
            .x        = 0.0f,
            .y        = 0.0f,
            .width    = (float)m_SwapChainExtent.width,
            .height   = (float)m_SwapChainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        const VkRect2D scissor{
            .offset = { 0, 0 },
            .extent = m_SwapChainExtent,
        };

        const VkPipelineViewportStateCreateInfo viewportState{
            .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext         = nullptr,
            .viewportCount = 1,
            .pViewports    = &viewport,
            .scissorCount  = 1,
            .pScissors     = &scissor,
        };

        // Rasterizer.
        const VkPipelineRasterizationStateCreateInfo rasterizer{
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
        const VkPipelineMultisampleStateCreateInfo multisampling{
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
        const VkPipelineColorBlendAttachmentState colorBlendAttachment{
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

        const VkPipelineColorBlendStateCreateInfo colorBlending{
            .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext           = nullptr,
            .logicOpEnable   = VK_FALSE,
            .logicOp         = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments    = &colorBlendAttachment,
            .blendConstants  = { 0.0f, 0.0f, 0.0f, 0.0f },
        };

        // Dynamic state - dynamically change pipeline paremeters.
        const std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

        const VkPipelineDynamicStateCreateInfo dynamicState{
            .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext             = nullptr,
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates    = dynamicStates.data(),
        };

        // Pipeline layout - uniforms in shaders.
        const VkPushConstantRange pushConstantRange{
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = sizeof(PushConstant),
        };

        const VkPipelineLayoutCreateInfo pipelineLayoutInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = nullptr,
            .setLayoutCount         = 0,
            .pSetLayouts            = nullptr,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &pushConstantRange,
        };

        vkCreatePipelineLayout(logicDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

        const VkGraphicsPipelineCreateInfo pipelineInfo{
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
            .layout             = m_PipelineLayout,
            .renderPass         = m_RenderPass,
            .subpass            = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex  = -1,
        };

        vkCreateGraphicsPipelines(logicDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);
    }

    void RendererContext::CreateFramebuffers()
    {
        VkDevice LogicDevice = m_Device->GetDevice();

        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

        for (auto i = 0; i < m_SwapChainImageViews.size(); i++)
        {
            const VkImageView Attachments[] = { m_SwapChainImageViews[i] };

            const VkFramebufferCreateInfo FramebufferInfo{
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext           = nullptr,
                .renderPass      = m_RenderPass,
                .attachmentCount = 1,
                .pAttachments    = Attachments,
                .width           = m_SwapChainExtent.width,
                .height          = m_SwapChainExtent.height,
                .layers          = 1,
            };

            vkCreateFramebuffer(LogicDevice, &FramebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);
        }
    }

    void RendererContext::CreateCommands()
    {
        VkDevice logicDevice = m_Device->GetDevice();

        const VkCommandPoolCreateInfo PoolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = m_Device->GetQFamilyIndex(),
        };

        vkCreateCommandPool(logicDevice, &PoolInfo, nullptr, &m_CommandPool);

        const VkCommandBufferAllocateInfo AllocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = m_CommandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        vkAllocateCommandBuffers(logicDevice, &AllocInfo, &m_CommandBuffer);
    }

    void RendererContext::CreateSyncObjects()
    {
        VkDevice LogicDevice = m_Device->GetDevice();

        const VkSemaphoreCreateInfo SemaphoreInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
        };

        const VkFenceCreateInfo FenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

        vkCreateSemaphore(LogicDevice, &SemaphoreInfo, nullptr, &m_ImageAvailableSemaphore);
        vkCreateSemaphore(LogicDevice, &SemaphoreInfo, nullptr, &m_RenderFinishedSemaphore);
        vkCreateFence(LogicDevice, &FenceInfo, nullptr, &m_FlightFence);
    }

    VkSurfaceFormatKHR RendererContext::ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
    {
        uint32_t FormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> AvailableFormats(FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, AvailableFormats.data());

        for (const auto& Format : AvailableFormats)
        {
            if (Format.format == VK_FORMAT_B8G8R8A8_UNORM && Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return Format;
            }
        }

        return AvailableFormats[0];
    }

    VkPresentModeKHR RendererContext::ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
    {
        // Presentation mode.
        uint32_t PresentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount, nullptr);

        std::vector<VkPresentModeKHR> AvailablePresentModes(PresentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount,
                                                  AvailablePresentModes.data());

        for (const auto& PresentMode : AvailablePresentModes)
        {
            if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return PresentMode; }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D RendererContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, GLFWwindow& Window)
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

    std::vector<const char*> RendererContext::GetRequiredInstanceExtensions()
    {
        uint32_t ExtensionCount     = 0;
        const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);

        std::vector<const char*> Extensions(GlfwExtensions, GlfwExtensions + ExtensionCount);

        if (m_bEnableValidationLayers) { Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }

        return Extensions;
    }

    void RendererContext::CreateMessenger()
    {
        const VkDebugUtilsMessengerCreateInfoEXT CreateInfo{
            .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext           = nullptr,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = ValidationMessageCallback,
            .pUserData       = nullptr,
        };

        const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanInstance,
                                                                                    "vkCreateDebugUtilsMessengerEXT");

        if (func) { func(m_VulkanInstance, &CreateInfo, nullptr, &m_Messenger); }
        else
        {
            STH_VK_WARN("vkCreateDebugUtilsMessengerEXT not found");
        }
    }

    void RendererContext::DestroyMessenger()
    {
        const auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanInstance,
                                                                                     "vkDestroyDebugUtilsMessengerEXT");
        if (func) { func(m_VulkanInstance, m_Messenger, nullptr); }
        else
        {
            STH_VK_WARN("vkDestroyDebugUtilsMessengerEXT not found");
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL RendererContext::ValidationMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        if (pCallbackData)
        {
            switch (messageSeverity)
            {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: STH_VK_ERR(pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: STH_VK_WARN(pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: STH_VK_INFO(pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: STH_VK_TRACE(pCallbackData->pMessage); break;
            }
        }

        // Return always false.
        return VK_FALSE;
    }

    bool RendererContext::CheckValidationLayers()
    {
        uint32_t Count;
        vkEnumerateInstanceLayerProperties(&Count, nullptr);

        std::vector<VkLayerProperties> AvailableLayers(Count);
        vkEnumerateInstanceLayerProperties(&Count, AvailableLayers.data());

        for (const char* ReqLayer : m_RequiredValidationLayers)
        {
            bool bFound = false;

            for (const VkLayerProperties& AvLayer : AvailableLayers)
            {
                if (AvLayer.layerName == ReqLayer)
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound) { return false; }
        }

        return true;
    }

} // namespace South