#include "Core/VulkanApplication.h"

#include "Core/Window.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

namespace South
{
    VulkanApplication::VulkanApplication(int argc, char** argv)
    {
        pWindow  = AppWindow::Create();
        bRunning = true;

        CreateInstance();
        CreateDevices();
    }

    VulkanApplication::~VulkanApplication()
    {
        delete pWindow;
        vkDestroyInstance(instance, nullptr);
        vkDestroyDevice(logicDevice, nullptr);
    }

    void VulkanApplication::Run()
    {
        while (bRunning)
        {
            if (pWindow)
            {
                pWindow->Tick();
            }
        }
    }

    void VulkanApplication::CreateInstance()
    {
        VkApplicationInfo sAppInfo{
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = nullptr,
            .pApplicationName   = "SouthRenderEngine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "None",
            .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
            .apiVersion         = VK_API_VERSION_1_0,
        };

        // Extensions
        uint32_t extensionCount = 0;
        const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        // #TODO : Check for exenstesions support.

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

    void VulkanApplication::CreateDevices()
    {
        uint32_t devicesCount = 0;
        vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);

        std::vector<VkPhysicalDevice> devices(devicesCount);
        vkEnumeratePhysicalDevices(instance, &devicesCount, devices.data());

        auto FindQueueFamiliesIndex = [](const VkPhysicalDevice& device)
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for (uint32_t i = 0; i < queueFamilyCount; ++i)
            {
                if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    return std::optional<uint32_t>(i);
                }
            }

            return std::optional<uint32_t>();
        };

        auto RateDevice = [FindQueueFamiliesIndex](const VkPhysicalDevice& device)
        {
            if (!FindQueueFamiliesIndex(device).has_value())
            {
                return -1;
            }

            // Rate based on features and properties.
            int score = 0;
            {
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
            }

            return score;
        };

        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& dev : devices)
        {
            candidates.emplace(RateDevice(dev), dev);
        }

        if (candidates.rbegin()->first > 0)
        {
            physDevice = candidates.rbegin()->second;
        }
        else
        {
            // Throw error?
        }

        const float queuePrio = 1.f;
        VkDeviceQueueCreateInfo sQCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .queueFamilyIndex = FindQueueFamiliesIndex(physDevice).value(),
            .queueCount       = 1,
            .pQueuePriorities = &queuePrio,
        };

        VkPhysicalDeviceFeatures sDeviceFeatures{};

        VkDeviceCreateInfo sCreateInfo{
            .sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                 = nullptr,
            .queueCreateInfoCount  = 1,
            .pQueueCreateInfos     = &sQCreateInfo,
            .enabledLayerCount     = 0,
            .enabledExtensionCount = 0,
            .pEnabledFeatures      = &sDeviceFeatures,
        };

        vkCreateDevice(physDevice, &sCreateInfo, nullptr, &logicDevice);
    }
} // namespace South