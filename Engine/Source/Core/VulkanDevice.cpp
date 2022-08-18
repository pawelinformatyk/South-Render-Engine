#include "sthpch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/VulkanDevice.h"
#include <optional>

namespace South
{
    GraphicCard* GraphicCard::Create(const CreateInfo& InCreateInfo)
    {
        // Get all gpus.
        uint32_t GpuCount = 0;
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GpuCount, nullptr);

        std::vector<VkPhysicalDevice> AvailableGpus(GpuCount);
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GpuCount, AvailableGpus.data());

        VkPhysicalDevice FoundGpu                          = nullptr;
        std::vector<VkExtensionProperties> FoundExtensions = {};
        VkPhysicalDeviceProperties FoundProperties         = VkPhysicalDeviceProperties();
        VkPhysicalDeviceFeatures FoundFeatures             = VkPhysicalDeviceFeatures();

        for (VkPhysicalDevice Gpu : AvailableGpus)
        {
            // Check queues flags.
            uint32_t QueueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(Gpu, &QueueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> QueueFamiliesProperties(QueueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(Gpu, &QueueFamilyCount, QueueFamiliesProperties.data());

            bool bQueuesFound = true;
            for (const VkQueueFamilyProperties& Properties : QueueFamiliesProperties)
            {
                if (!(Properties.queueFlags & InCreateInfo.RequiredQueueFlags))
                {
                    bQueuesFound = false;
                    break;
                }
            }

            if (!bQueuesFound) { continue; }

            // Check type.
            vkGetPhysicalDeviceProperties(Gpu, &FoundProperties);
            if (FoundProperties.deviceType != InCreateInfo.Type) { continue; }

            // Check required extensions.
            uint32_t ExtensionCount;
            vkEnumerateDeviceExtensionProperties(Gpu, nullptr, &ExtensionCount, nullptr);

            FoundExtensions = std::vector<VkExtensionProperties>(ExtensionCount);
            vkEnumerateDeviceExtensionProperties(Gpu, nullptr, &ExtensionCount, FoundExtensions.data());

            std::set<std::string> RequiredExtensionsSet(InCreateInfo.RequiredExtensions.begin(),
                                                        InCreateInfo.RequiredExtensions.end());

            for (const VkExtensionProperties& Extension : FoundExtensions)
            {
                RequiredExtensionsSet.erase(Extension.extensionName);
            }

            if (!RequiredExtensionsSet.empty()) { continue; }

            // Check required features.
            // vkGetPhysicalDeviceFeatures(GPU, &Features);

            // #TODO : Cannot be == cuz struct.
            // if(AvailableFeatures!=)

            FoundGpu = Gpu;

            break;
        }

        if (!FoundGpu) { return nullptr; }

        std::vector<const char*> ExtensionsNames;
        ExtensionsNames.reserve(FoundExtensions.size());
        for (const VkExtensionProperties& Extension : FoundExtensions)
        {
            ExtensionsNames.emplace_back(Extension.extensionName);
        }

        auto* Gpu              = new GraphicCard;
        Gpu->m_PhysicalDevice  = FoundGpu;
        Gpu->m_ExtensionsNames = InCreateInfo.RequiredExtensions;
        Gpu->m_Features        = FoundFeatures;
        Gpu->m_Properties      = FoundProperties;

        switch (FoundProperties.deviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                Gpu->m_TypeName = "Other";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                Gpu->m_TypeName = "Integrated";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                Gpu->m_TypeName = "Discrete";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                Gpu->m_TypeName = "Virtual";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                Gpu->m_TypeName = "CPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
                Gpu->m_TypeName = "Other";
                break;
        }

        return Gpu;
    }

    bool GraphicCard::CreateLogicalDevice(VkDevice& OutLogicalDevice, Queue& OutGraphicQueue) const
    {
        const std::optional<uint32_t> GraphicQueueFamilyIndex = FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        if (!GraphicQueueFamilyIndex.has_value()) { return false; }

        constexpr float QueuePrio = 1.f;
        const VkDeviceQueueCreateInfo GraphicQueueCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0x00,
            .queueFamilyIndex = GraphicQueueFamilyIndex.value(),
            .queueCount       = 1,
            .pQueuePriorities = &QueuePrio,
        };

        const VkDeviceCreateInfo LogicDeviceCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0x00,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &GraphicQueueCreateInfo,
            .enabledExtensionCount   = static_cast<uint32_t>(m_ExtensionsNames.size()),
            .ppEnabledExtensionNames = m_ExtensionsNames.data(),
            .pEnabledFeatures        = &m_Features,
        };

        vkCreateDevice(m_PhysicalDevice, &LogicDeviceCreateInfo, nullptr, &OutLogicalDevice);

        VkQueue GraphicQueue = nullptr;
        vkGetDeviceQueue(OutLogicalDevice, GraphicQueueFamilyIndex.value(), 0, &GraphicQueue);

        OutGraphicQueue.m_Queue       = GraphicQueue;
        OutGraphicQueue.m_QueueFamily = GraphicQueueFamilyIndex.value();

        return true;
    }

    VkPhysicalDevice GraphicCard::GetPhysicalDevice() const { return m_PhysicalDevice; }

    const VkPhysicalDeviceFeatures& GraphicCard::GetFeatures() const { return m_Features; }

    const VkPhysicalDeviceProperties& GraphicCard::GetProperties() const { return m_Properties; }

    const std::string& GraphicCard::GetTypeName() const { return m_TypeName; }

    std::optional<uint32_t> GraphicCard::FindQueueFamilyIndex(const VkQueueFlagBits InFlags) const
    {
        uint32_t QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &QueueFamilyCount, QueueFamilies.data());

        for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
        {
            if (QueueFamilies[QueueFamilyIndex].queueFlags & InFlags) { return std::optional(QueueFamilyIndex); }
        }

        return {};
    }

} // namespace South