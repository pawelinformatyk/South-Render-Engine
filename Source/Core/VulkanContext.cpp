#include "Core/VulkanContext.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>
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
    }

    void VulkanContext::DeInit()
    {
        vkDestroyInstance(instance, nullptr);
        vkDestroyDevice(logicDevice, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
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
            [&requiredDeviceExtensions = requiredDeviceExtensions, &surface = surface](const VkPhysicalDevice& dev)
        {
            // Check if device supports swap chain.
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(dev, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(dev, nullptr, &extensionCount, availableExtensions.data());

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

            // Check if device has needed queue family.
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, queueFamilies.data());

            for (uint32_t i = 0; i < queueFamilyCount; ++i)
            {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &presentSupport);

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
} // namespace South
