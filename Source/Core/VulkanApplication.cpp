#include "Core/VulkanApplication.h"

#include "Core/Window.h"

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

namespace South
{
    VulkanApplication::VulkanApplication(int argc, char** argv)
    {
        pWindow  = AppWindow::Create();
        bRunning = true;

        CreateInstance();
        PickGraphicCard();
    }

    VulkanApplication::~VulkanApplication()
    {
        delete pWindow;
        vkDestroyInstance(instance, nullptr);
    }

    void VulkanApplication::Run()
    {
        while (bRunning)
        {
            if (pWindow)
            {
                pWindow->tick();
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

    void VulkanApplication::PickGraphicCard()
    {
        uint32_t devicesCount = 0;
        vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);

        std::vector<VkPhysicalDevice> devices(devicesCount);
        vkEnumeratePhysicalDevices(instance, &devicesCount, devices.data());

        auto RateDevice = [](const VkPhysicalDevice& device)
        {
            // Reject devices without VK_QUEUE_GRAPHICS_BIT.
            {
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                for (const VkQueueFamilyProperties& prop : queueFamilies)
                {
                    if (!(prop.queueFlags & VK_QUEUE_GRAPHICS_BIT))
                    {
                        return -1;
                    }
                }
            }

            // Rate based on features.
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

        // Graphic card candidates.
        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& dev : devices)
        {
            candidates.emplace(RateDevice(dev), dev);
        }

        if (candidates.rbegin()->first > 0)
        {
            graphicCard = candidates.rbegin()->second;
        }
        else
        {
        }
    }
} // namespace South