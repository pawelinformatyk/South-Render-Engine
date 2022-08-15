#include "sthpch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/VulkanDevice.h"
#include <optional>

namespace South
{

    VulkanDevice* VulkanDevice::Create(const CreateInfo& InInfo)
    {
        const std::vector<VkPhysicalDevice> GPUs = FindSuitableGraphicCards(InInfo.VulkanInstance,
                                                                            InInfo.Surface,
                                                                            InInfo.RequiredGPUExtensions,
                                                                            InInfo.GPUType);
        if (!GPUs.size()) { return nullptr; }

        const float QueuePrio[3] = { 1.f, 1.f, 1.f };

        const std::optional<uint32_t> GQueueFamily = FindQueueFamily(GPUs[0], nullptr, VK_QUEUE_GRAPHICS_BIT);

        const std::optional<uint32_t> PQueueFamily =
            FindQueueFamily(GPUs[0], InInfo.Surface, VK_QUEUE_FLAG_BITS_MAX_ENUM);

        const std::optional<uint32_t> TQueueFamily = FindQueueFamily(GPUs[0], nullptr, VK_QUEUE_TRANSFER_BIT);

        const VkDeviceQueueCreateInfo GraphicCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0x00,
            .queueFamilyIndex = GQueueFamily.value(),
            .queueCount       = 3,
            .pQueuePriorities = QueuePrio,
        };

        // Q families - count

        const std::vector<VkDeviceQueueCreateInfo> QueuesCreateInfo = { GraphicCreateInfo };

        const VkDeviceCreateInfo LogicDeviceCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0x00,
            .queueCreateInfoCount    = static_cast<uint32_t>(QueuesCreateInfo.size()),
            .pQueueCreateInfos       = QueuesCreateInfo.data(),
            .enabledExtensionCount   = static_cast<uint32_t>(InInfo.RequiredGPUExtensions.size()),
            .ppEnabledExtensionNames = InInfo.RequiredGPUExtensions.data(),
            .pEnabledFeatures        = InInfo.RequiredGPUFeatures.data(),
        };

        VulkanDevice* NewVulkanDevice          = new VulkanDevice;
        NewVulkanDevice->m_GraphicCard         = GPUs[0];
        NewVulkanDevice->m_GraphicQueueFamily  = GQueueFamily.value();
        NewVulkanDevice->m_PresentQueueFamily  = PQueueFamily.value();
        NewVulkanDevice->m_TransferQueueFamily = TQueueFamily.value();

        vkCreateDevice(NewVulkanDevice->m_GraphicCard,
                       &LogicDeviceCreateInfo,
                       nullptr,
                       &NewVulkanDevice->m_LogicalDevice);

        vkGetDeviceQueue(NewVulkanDevice->m_LogicalDevice,
                         NewVulkanDevice->m_GraphicQueueFamily,
                         0,
                         &NewVulkanDevice->m_GraphicQueue);

        vkGetDeviceQueue(NewVulkanDevice->m_LogicalDevice,
                         NewVulkanDevice->m_PresentQueueFamily,
                         1,
                         &NewVulkanDevice->m_PresentQueue);

        vkGetDeviceQueue(NewVulkanDevice->m_LogicalDevice,
                         NewVulkanDevice->m_TransferQueueFamily,
                         2,
                         &NewVulkanDevice->m_TransferQueue);

        return NewVulkanDevice;
    }

    void VulkanDevice::Destroy(VulkanDevice* InDevice)
    {
        if (!InDevice) { return; }

        vkDestroyDevice(InDevice->m_LogicalDevice, nullptr);
        delete InDevice;
    }

    VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const { return m_GraphicCard; }

    VkDevice VulkanDevice::GetDevice() const { return m_LogicalDevice; }

    uint32_t VulkanDevice::GetQFamilyIndex() const { return m_GraphicQueueFamily; }

    VkQueue VulkanDevice::GetGraphicQueue() const { return m_GraphicQueue; }

    std::vector<VkPhysicalDevice>
        VulkanDevice::FindSuitableGraphicCards(VkInstance VulkanInstance,
                                               VkSurfaceKHR Surface,
                                               const std::vector<const char*>& RequiredGPUExtensions,
                                               VkPhysicalDeviceType GPUType)
    {
        // Get all gpus.
        uint32_t GPUCount = 0;
        vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, nullptr);

        std::vector<VkPhysicalDevice> AvailableGPUs(GPUCount);
        vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, AvailableGPUs.data());

        std::vector<VkPhysicalDevice> SuitableGPUs;

        for (VkPhysicalDevice GPU : AvailableGPUs)
        {
            // Check if device has all required extensions.
            uint32_t ExtensionCount;
            vkEnumerateDeviceExtensionProperties(GPU, nullptr, &ExtensionCount, nullptr);

            std::vector<VkExtensionProperties> AvailableGPUExtensions(ExtensionCount);
            vkEnumerateDeviceExtensionProperties(GPU, nullptr, &ExtensionCount, AvailableGPUExtensions.data());

            std::set<std::string> RequiredGPUExtensionsSet(RequiredGPUExtensions.begin(), RequiredGPUExtensions.end());

            for (const VkExtensionProperties& Extension : AvailableGPUExtensions)
            {
                RequiredGPUExtensionsSet.erase(Extension.extensionName);
            }

            if (!RequiredGPUExtensionsSet.empty()) { continue; }

            VkPhysicalDeviceProperties GPUProperties;
            vkGetPhysicalDeviceProperties(GPU, &GPUProperties);
            if (GPUProperties.deviceType != GPUType) { continue; }

            // #TODO : Check for optional features - 64bit flaots, multi viewport rendering
            // VkPhysicalDeviceFeatures deviceFeatures;
            // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            uint32_t FormatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(GPU, Surface, &FormatCount, nullptr);
            if (!FormatCount) { continue; }

            uint32_t PresentModesCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(GPU, Surface, &PresentModesCount, nullptr);
            if (!PresentModesCount) { continue; }

            SuitableGPUs.emplace_back(GPU);
        }

        return SuitableGPUs;
    }
    std::optional<uint32_t>
        VulkanDevice::FindQueueFamily(VkPhysicalDevice GPU, VkSurfaceKHR Surface, VkQueueFlagBits Flags)
    {
        uint32_t QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, QueueFamilies.data());

        for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
        {
            if (Surface)
            {
                VkBool32 bPresentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(GPU, QueueFamilyIndex, Surface, &bPresentSupport);

                if (bPresentSupport) { return std::optional<uint32_t>(QueueFamilyIndex); }
            }
            else
            {
                // Present and Graphic queue in one.
                if (QueueFamilies[QueueFamilyIndex].queueFlags & Flags)
                {
                    return std::optional<uint32_t>(QueueFamilyIndex);
                }
            }
        }

        return {};
    }

    GraphicCard* GraphicCard::Create(const CreateInfo& InCreateInfo)
    {
        // Get all gpus.
        uint32_t GPUCount = 0;
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GPUCount, nullptr);

        std::vector<VkPhysicalDevice> AvailableGPUs(GPUCount);
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GPUCount, AvailableGPUs.data());

        std::vector<VkPhysicalDevice> FoundGPUs;

        for (VkPhysicalDevice GPU : AvailableGPUs)
        {
            // Check queues flags.
            uint32_t QueueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> QueueFamiliesProperties(QueueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, QueueFamiliesProperties.data());

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
            VkPhysicalDeviceProperties Properties;
            vkGetPhysicalDeviceProperties(GPU, &Properties);
            if (Properties.deviceType != InCreateInfo.Type) { continue; }

            // Check required extensions.
            uint32_t ExtensionCount;
            vkEnumerateDeviceExtensionProperties(GPU, nullptr, &ExtensionCount, nullptr);

            std::vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
            vkEnumerateDeviceExtensionProperties(GPU, nullptr, &ExtensionCount, AvailableExtensions.data());

            std::set<std::string> RequiredExtensionsSet(InCreateInfo.RequiredExtensions.begin(),
                                                        InCreateInfo.RequiredExtensions.end());

            for (const VkExtensionProperties& Extension : AvailableExtensions)
            {
                RequiredExtensionsSet.erase(Extension.extensionName);
            }

            if (!RequiredExtensionsSet.empty()) { continue; }

            // Check required features.
            VkPhysicalDeviceFeatures AvailableFeatures;
            vkGetPhysicalDeviceFeatures(GPU, &AvailableFeatures);

            // #TODO : Cannot be == cuz struct.
            // if(AvailableFeatures!=)

            FoundGPUs.emplace_back(GPU);
        }

        if (!FoundGPUs.size()) { return nullptr; }

        GraphicCard* GPU       = new GraphicCard;
        GPU->m_PhysicalDevice  = FoundGPUs[0];
        GPU->m_ExtensionsNames = InCreateInfo.RequiredExtensions;
        GPU->m_Features        = InCreateInfo.RequiredFeatures;

        return GPU;
    }

    bool GraphicCard::CreateLogicalDevice(VkDevice OutLogicalDevice, Queue* OutGraphicQueue) const
    {
        const std::optional<uint32_t> GraphicQueueFamilyIndex = FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        if (!GraphicQueueFamilyIndex.has_value()) { return false; }

        const float QueuePrio = 1.f;
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

        OutGraphicQueue                = new Queue;
        OutGraphicQueue->m_Queue       = GraphicQueue;
        OutGraphicQueue->m_QueueFamily = GraphicQueueFamilyIndex.value();

        return true;
    }

    std::optional<uint32_t> GraphicCard::FindQueueFamilyIndex(VkQueueFlagBits InFlags) const
    {
        uint32_t QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &QueueFamilyCount, QueueFamilies.data());

        for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
        {
            if (QueueFamilies[QueueFamilyIndex].queueFlags & InFlags)
            {
                return std::optional<uint32_t>(QueueFamilyIndex);
            }
        }

        return {};
    }

} // namespace South