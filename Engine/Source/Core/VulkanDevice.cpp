#include "sthpch.h"

#include "Core/VulkanDevice.h"
#include "Core/VulkanContext.h"
#include <optional>

namespace South
{

    void VulkanDevice::Init(VkSurfaceKHR Surface)
    {
        // Lambda to tell us if gpu is suitable. Function returns correct queue it is suitable.
        auto IsDeviceSuitable =
            [&RequiredDeviceExtensions = RequiredDeviceExtensions, &Surface = Surface](const VkPhysicalDevice& Device)
        {
            // Check if device supports swap chain.
            uint32_t ExtensionCount;
            vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);

            std::vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
            vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());

            std::set<std::string> RequiredExtensionsSet(RequiredDeviceExtensions.begin(),
                                                        RequiredDeviceExtensions.end());

            for (const auto& Extension : AvailableExtensions) { RequiredExtensionsSet.erase(Extension.extensionName); }

            if (!RequiredExtensionsSet.empty()) { return std::optional<uint32_t>(); }

            // #TODO : Look for specific gpu??
            uint32_t FormatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);
            if (!FormatCount) { return std::optional<uint32_t>(); }

            uint32_t PresentModesCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModesCount, nullptr);
            if (!PresentModesCount) { return std::optional<uint32_t>(); }

            // Check if device has needed queue family.
            uint32_t QueueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

            for (uint32_t i = 0; i < QueueFamilyCount; ++i)
            {
                VkBool32 bPresentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, Surface, &bPresentSupport);

                if (bPresentSupport && (QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    return std::optional<uint32_t>(i);
                }
            }

            return std::optional<uint32_t>();
        };

        // Lambda to rate device based on our needs.
        auto RateDevice = [](const VkPhysicalDevice& Device)
        {
            // Rate based on features and properties.
            int Score = 0;

            VkPhysicalDeviceProperties DeviceProperties;
            VkPhysicalDeviceFeatures DeviceFeatures;
            vkGetPhysicalDeviceProperties(Device, &DeviceProperties);
            vkGetPhysicalDeviceFeatures(Device, &DeviceFeatures);

            // Discrete GPUs have a significant performance advantage
            if (DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { Score += 1000; }
            if (DeviceFeatures.multiViewport) { Score += 1000; }

            return Score;
        };

        VkInstance VulkanInstance = VulkanContext::Get().GetVulkanInstance();

        // Get vector of devices.
        uint32_t DevicesCount = 0;
        vkEnumeratePhysicalDevices(VulkanInstance, &DevicesCount, nullptr);

        std::vector<VkPhysicalDevice> Devices(DevicesCount);
        vkEnumeratePhysicalDevices(VulkanInstance, &DevicesCount, Devices.data());

        // Find best gpu available. Reject those without correct queue and rate the rest.
        std::multimap<int, std::pair<VkPhysicalDevice, uint32_t>> DevicesScoreboard;

        for (const VkPhysicalDevice& Dev : Devices)
        {
            std::optional<uint32_t> QFamIndex = IsDeviceSuitable(Dev);
            if (!QFamIndex.has_value()) { continue; }

            DevicesScoreboard.emplace(RateDevice(Dev), std::make_pair(Dev, QFamIndex.value()));
        }

        // Get device if it was found.
        if (DevicesScoreboard.size())
        {
            PhysicalDevice   = DevicesScoreboard.rbegin()->second.first;
            QueueFamilyIndex = DevicesScoreboard.rbegin()->second.second;
        }
        else
        {
            std::cout << "physDevice Not found." << std::endl;
            // Throw error?
        }

        // Set logical device and queue.
        const float QueuePrio = 1.f;

        VkDeviceQueueCreateInfo QCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .queueFamilyIndex = QueueFamilyIndex,
            .queueCount       = 1,
            .pQueuePriorities = &QueuePrio,
        };

        VkPhysicalDeviceFeatures DeviceFeatures{};

        VkDeviceCreateInfo CreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &QCreateInfo,
            .enabledLayerCount       = 0,
            .enabledExtensionCount   = static_cast<uint32_t>(RequiredDeviceExtensions.size()),
            .ppEnabledExtensionNames = RequiredDeviceExtensions.data(),
            .pEnabledFeatures        = &DeviceFeatures,
        };

        vkCreateDevice(PhysicalDevice, &CreateInfo, nullptr, &LogicalDevice);

        vkGetDeviceQueue(LogicalDevice, QueueFamilyIndex, 0, &Queue);
    }

    void VulkanDevice::DeInit() { vkDestroyDevice(LogicalDevice, nullptr); }

    VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const { return PhysicalDevice; }

    VkDevice VulkanDevice::GetDevice() const { return LogicalDevice; }

    uint32_t VulkanDevice::GetQFamilyIndex() const { return QueueFamilyIndex; }

    VkQueue VulkanDevice::GetQ() const { return Queue; }

} // namespace South