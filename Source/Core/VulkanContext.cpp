#include "Core/VulkanContext.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>
#include <algorithm>
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
    }

    void VulkanContext::DeInit()
    {
        vkDestroyInstance(instance, nullptr);
        vkDestroyDevice(logicDevice, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroySwapchainKHR(logicDevice, swapChain, nullptr);
        for (auto imageView : swapChainImageViews)
        {
            vkDestroyImageView(logicDevice, imageView, nullptr);
        }
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
        VkSwapchainCreateInfoKHR sCreateInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = nullptr,
            .surface               = surface,
            .imageArrayLayers      = 1,
            .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices   = &QueueFamilyIndex,
            .preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .clipped               = VK_TRUE,
            .oldSwapchain          = VK_NULL_HANDLE,
        };

        // Swap extent - Image size.

        // Presentation mode.
        uint32_t presentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModesCount, nullptr);

        std::vector<VkPresentModeKHR> availablePresentModes(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModesCount,
                                                  availablePresentModes.data());

        sCreateInfo.presentMode = /*Default*/ VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& mode : availablePresentModes)
        {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                sCreateInfo.presentMode = mode;
            }
        }

        // Surface format.

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, availableFormats.data());

        sCreateInfo.imageFormat     = availableFormats[0].format;
        sCreateInfo.imageColorSpace = availableFormats[0].colorSpace;
        for (const auto& format : availableFormats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                sCreateInfo.imageFormat     = VK_FORMAT_B8G8R8A8_SRGB;
                sCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            }
        }

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &capabilities);

        int width, height;
        glfwGetFramebufferSize(&window, &width, &height);

        sCreateInfo.imageExtent =
            VkExtent2D(std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width,
                                  capabilities.maxImageExtent.width),
                       std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.width,
                                  capabilities.maxImageExtent.width));

        sCreateInfo.minImageCount = (capabilities.minImageCount + 1 > capabilities.maxImageCount)
                                        ? capabilities.minImageCount
                                        : capabilities.minImageCount + 1;

        vkCreateSwapchainKHR(logicDevice, &sCreateInfo, nullptr, &swapChain);

        uint32_t imageCount;
        vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, nullptr);

        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = sCreateInfo.imageFormat;
        swapChainExtent      = sCreateInfo.imageExtent;
    }

    void VulkanContext::CreateImageViews()
    {
        swapChainImageViews.reserve(swapChainImages.size());

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

    void VulkanContext::CreateGraphicsPipeline()
    {
        auto readFile = [](const std::string& fileName)
        {
            std::ifstream file(fileName, std::ios::ate | std::ios::binary);

            if (!file.is_open())
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

        std::vector<char> vertShaderCode = readFile("shaders/vert.spv");
        std::vector<char> fragShaderCode = readFile("shaders/frag.spv");

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

        vkDestroyShaderModule(logicDevice, vertShaderMod, nullptr);
        vkDestroyShaderModule(logicDevice, fragShaderMod, nullptr);
    }

} // namespace South
