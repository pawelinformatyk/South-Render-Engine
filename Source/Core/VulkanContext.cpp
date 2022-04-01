#include "Core/VulkanContext.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <utility>

namespace South
{
    void VulkanContext::Init(GLFWwindow& window)
    {
        CreateInstance();
        CreateSurface(window);
        CreateDevices();
        CreateSwapChain(window);
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommands();
        CreateSyncObjects();
    }

    void VulkanContext::DeInit()
    {
        // #TODO : Order?

        vkDestroySurfaceKHR(instance, surface, nullptr);
        for (auto imageView : swapChainImageViews)
        {
            vkDestroyImageView(logicDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(logicDevice, swapChain, nullptr);

        vkDestroyPipelineLayout(logicDevice, pipelineLayout, nullptr);
        vkDestroyRenderPass(logicDevice, renderPass, nullptr);

        vkDestroyPipeline(logicDevice, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(logicDevice, pipelineLayout, nullptr);

        for (auto framebuffer : swapChainFramebuffers)
        {
            vkDestroyFramebuffer(logicDevice, framebuffer, nullptr);
        }

        vkDestroyCommandPool(logicDevice, commandPool, nullptr);

        vkDestroySemaphore(logicDevice, imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(logicDevice, renderFinishedSemaphore, nullptr);
        vkDestroyFence(logicDevice, inFlightFence, nullptr);

        vkDestroyDevice(logicDevice, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    void VulkanContext::Tick()
    {
        // Wait for the previous frame to finish
        vkWaitForFences(logicDevice, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(logicDevice, 1, &inFlightFence);

        // Acquire an image from the swap chain
        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(logicDevice, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        // Submit the recorded command buffer
        vkResetCommandBuffer(commandBuffer, 0);
        RecordCommandBuffer(commandBuffer, imageIndex);

        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore waitSemaphores[]      = { imageAvailableSemaphore };
        VkSemaphore signalSemaphores[]    = { renderFinishedSemaphore };

        VkSubmitInfo submitInfo{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                = nullptr,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = waitSemaphores,
            .pWaitDstStageMask    = waitStages,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = signalSemaphores,
        };

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence);

        VkSwapchainKHR swapChains[] = { swapChain };

        // Present the swap chain image
        VkPresentInfoKHR presentInfo{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = signalSemaphores,
            .swapchainCount     = 1,
            .pSwapchains        = swapChains,
            .pImageIndices      = &imageIndex,
            .pResults           = nullptr,
        };

        vkQueuePresentKHR(graphicsQueue, &presentInfo);
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

        vkCreateInstance(&sCreateInfo, nullptr, &instance);
    }

    void VulkanContext::CreateSurface(GLFWwindow& window)
    {
        if (!glfwCreateWindowSurface(instance, &window, nullptr, &surface))
        {
            return /*error*/;
        }
    }

    void VulkanContext::CreateDevices()
    {
        // Lambda to tell us if gpu is suitable. Function returns correct queue it is suitable.
        auto IsDeviceSuitable =
            [&requiredDeviceExtensions = requiredDeviceExtensions, &surface = surface](const VkPhysicalDevice& device)
        {
            // Check if device supports swap chain.
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> requiredExtensionsSet(requiredDeviceExtensions.begin(),
                                                        requiredDeviceExtensions.end());

            for (const auto& extension : availableExtensions)
            {
                requiredExtensionsSet.erase(extension.extensionName);
            }

            if (!requiredExtensionsSet.empty())
            {
                return std::optional<uint32_t>();
            }

            // #TODO : Look for specific gpu??
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
            if (!formatCount)
            {
                return std::optional<uint32_t>();
            }

            uint32_t presentModesCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);
            if (!presentModesCount)
            {
                return std::optional<uint32_t>();
            }

            // Check if device has needed queue family.
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for (uint32_t i = 0; i < queueFamilyCount; ++i)
            {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

                if (presentSupport && (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    return std::optional<uint32_t>(i);
                }
            }

            return std::optional<uint32_t>();
        };

        // Lambda to rate device based on our needs.
        auto RateDevice = [](const VkPhysicalDevice& device)
        {
            // Rate based on features and properties.
            int score = 0;

            VkPhysicalDeviceProperties deviceProperties;
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            // Discrete GPUs have a significant performance advantage
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                score += 1000;
            }
            if (deviceFeatures.multiViewport)
            {
                score += 1000;
            }

            return score;
        };

        // Get vector of devices.
        uint32_t devicesCount = 0;
        vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);

        std::vector<VkPhysicalDevice> devices(devicesCount);
        vkEnumeratePhysicalDevices(instance, &devicesCount, devices.data());

        // Find best gpu available. Reject those without correct queue and rate the rest.
        std::multimap<int, std::pair<VkPhysicalDevice, uint32_t>> devicesScoreboard;

        for (const VkPhysicalDevice& dev : devices)
        {
            std::optional<uint32_t> qFamIndex = IsDeviceSuitable(dev);
            if (!qFamIndex.has_value())
            {
                continue;
            }

            devicesScoreboard.emplace(RateDevice(dev), std::make_pair(dev, qFamIndex.value()));
        }

        // Get device if it was found.
        if (devicesScoreboard.size())
        {
            physDevice       = devicesScoreboard.rbegin()->second.first;
            QueueFamilyIndex = devicesScoreboard.rbegin()->second.second;
        }
        else
        {
            std::cout << "physDevice Not found." << std::endl;
            // Throw error?
        }

        // Set logical device and queue.
        const float queuePrio = 1.f;

        VkDeviceQueueCreateInfo sQCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .queueFamilyIndex = QueueFamilyIndex,
            .queueCount       = 1,
            .pQueuePriorities = &queuePrio,
        };

        VkPhysicalDeviceFeatures sDeviceFeatures{};

        VkDeviceCreateInfo sCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &sQCreateInfo,
            .enabledLayerCount       = 0,
            .enabledExtensionCount   = static_cast<uint32_t>(requiredDeviceExtensions.size()),
            .ppEnabledExtensionNames = requiredDeviceExtensions.data(),
            .pEnabledFeatures        = &sDeviceFeatures,
        };

        vkCreateDevice(physDevice, &sCreateInfo, nullptr, &logicDevice);

        vkGetDeviceQueue(logicDevice, QueueFamilyIndex, 0, &graphicsQueue);
    }

    void VulkanContext::CreateSwapChain(GLFWwindow& window)
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(physDevice, surface);
        VkPresentModeKHR presentMode     = ChooseSwapPresentMode(physDevice, surface);
        VkExtent2D extent                = ChooseSwapExtent(capabilities, window);

        VkSwapchainCreateInfoKHR createInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = nullptr,
            .surface               = surface,
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

        vkCreateSwapchainKHR(logicDevice, &createInfo, nullptr, &swapChain);

        vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, nullptr);

        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = createInfo.imageFormat;
        swapChainExtent      = createInfo.imageExtent;
    }

    void VulkanContext::CreateImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());

        VkImageSubresourceRange subresourceRange{
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo sCreateInfo{
                .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext            = nullptr,
                .image            = swapChainImages[i],
                .viewType         = VK_IMAGE_VIEW_TYPE_2D,
                .format           = swapChainImageFormat,
                .components       = VkComponentMapping(VK_COMPONENT_SWIZZLE_IDENTITY),
                .subresourceRange = subresourceRange,
            };

            vkCreateImageView(logicDevice, &sCreateInfo, nullptr, &swapChainImageViews[i]);
        }
    }

    void VulkanContext::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{
            .format         = swapChainImageFormat,
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

        vkCreateRenderPass(logicDevice, &renderPassInfo, nullptr, &renderPass);
    }

    void VulkanContext::CreateGraphicsPipeline()
    {
        auto readFile = [](const std::string& fileName)
        {
            std::ifstream file(fileName, std::ios::ate | std::ios::binary);

            if (file.fail() || !file.is_open())
            {
                throw std::runtime_error("failed to open file!");
            }

            size_t fileSize = (size_t)file.tellg();

            std::vector<char> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);

            file.close();

            return buffer;
        };

        auto CreateShaderModule = [&device = logicDevice](const std::vector<char>& code)
        {
            VkShaderModuleCreateInfo createInfo{
                .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext    = nullptr,
                .codeSize = code.size(),
                .pCode    = reinterpret_cast<const uint32_t*>(code.data()),
            };

            VkShaderModule shaderModule;
            if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create shader module!");
            }

            return shaderModule;
        };

        std::vector<char> vertShaderCode = readFile("Source/Shaders/vert.spv");
        std::vector<char> fragShaderCode = readFile("Source/Shaders/frag.spv");

        VkShaderModule vertShaderMod = CreateShaderModule(vertShaderCode);
        VkShaderModule fragShaderMod = CreateShaderModule(fragShaderCode);

        // Assign shaders to specific pipeline
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertShaderMod,
            .pName  = "main",
        };

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragShaderMod,
            .pName  = "main",
        };

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        // Inputs to vertext shader.
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                           = nullptr,
            .vertexBindingDescriptionCount   = 0,
            .pVertexBindingDescriptions      = nullptr,
            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions    = nullptr,
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
            .width    = (float)swapChainExtent.width,
            .height   = (float)swapChainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor{
            .offset = { 0, 0 },
            .extent = swapChainExtent,
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
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = nullptr,
            .setLayoutCount         = 0,
            .pSetLayouts            = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges    = nullptr,
        };

        vkCreatePipelineLayout(logicDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout);

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
            .layout             = pipelineLayout,
            .renderPass         = renderPass,
            .subpass            = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex  = -1,
        };

        vkCreateGraphicsPipelines(logicDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

        vkDestroyShaderModule(logicDevice, vertShaderMod, nullptr);
        vkDestroyShaderModule(logicDevice, fragShaderMod, nullptr);
    }

    void VulkanContext::CreateFramebuffers()
    {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (auto i = 0; i < swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = { swapChainImageViews[i] };

            VkFramebufferCreateInfo framebufferInfo{
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext           = nullptr,
                .renderPass      = renderPass,
                .attachmentCount = 1,
                .pAttachments    = attachments,
                .width           = swapChainExtent.width,
                .height          = swapChainExtent.height,
                .layers          = 1,
            };

            vkCreateFramebuffer(logicDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
        }
    }

    void VulkanContext::CreateCommands()
    {
        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = QueueFamilyIndex,
        };

        vkCreateCommandPool(logicDevice, &poolInfo, nullptr, &commandPool);

        VkCommandBufferAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        vkAllocateCommandBuffers(logicDevice, &allocInfo, &commandBuffer);
    }

    void VulkanContext::CreateSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
        };

        VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

        vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
        vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
        vkCreateFence(logicDevice, &fenceInfo, nullptr, &inFlightFence);
    }

    VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const VkPhysicalDevice& inDevice,
                                                              const VkSurfaceKHR& inSurface)
    {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &formatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &formatCount, availableFormats.data());

        for (const auto& format : availableFormats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return format;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanContext::ChooseSwapPresentMode(const VkPhysicalDevice& inDevice,
                                                          const VkSurfaceKHR& inSurface)
    {
        // Presentation mode.
        uint32_t presentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &presentModesCount, nullptr);

        std::vector<VkPresentModeKHR> availablePresentModes(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &presentModesCount,
                                                  availablePresentModes.data());

        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(&window, &width, &height);

            VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            actualExtent.width =
                std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height =
                std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanContext::RecordCommandBuffer(const VkCommandBuffer& buffer, const uint32_t& imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = 0,
            .pInheritanceInfo = nullptr,
        };

        vkBeginCommandBuffer(buffer, &beginInfo);

        VkClearValue clearColor = { { 0.f, 0.f, 0.f, 1.f } };

        VkRenderPassBeginInfo renderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext           = nullptr,
            .renderPass      = renderPass,
            .framebuffer     = swapChainFramebuffers[imageIndex],
            .renderArea      = VkRect2D({ 0, 0 }, swapChainExtent),
            .clearValueCount = 1,
            .pClearValues    = &clearColor,
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        vkEndCommandBuffer(commandBuffer);
    }

} // namespace South
