#include "sthpch.h"

#include "Core/VulkanDevice.h"
#include "Core/VulkanContext.h"
#include <optional>

namespace South
{

    VulkanDevice::~VulkanDevice()
    {
        vkDestroyDevice(logicalDevice, nullptr);
    }

    VulkanDevice* VulkanDevice::Create()
    {
        return new VulkanDevice;
    }

    void VulkanDevice::Init(VkSurfaceKHR surface)
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

        VkInstance instance = VulkanContext::GetVulkanInstance();

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
            physicalDevice   = devicesScoreboard.rbegin()->second.first;
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

        vkCreateDevice(physicalDevice, &sCreateInfo, nullptr, &logicalDevice);

        vkGetDeviceQueue(logicalDevice, QueueFamilyIndex, 0, &Queue);
    }

    VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const
    {
        return physicalDevice;
    }

    VkDevice VulkanDevice::GetDevice() const
    {
        return logicalDevice;
    }

    uint32_t VulkanDevice::GetQFamilyIndex() const
    {
        return QueueFamilyIndex;
    }

    VkQueue VulkanDevice::GetQ() const
    {
        return Queue;
    }

} // namespace South