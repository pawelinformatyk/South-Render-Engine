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
            std::cout << "Queue number: " + std::to_string(QueueFamilies[QueueFamilyIndex].queueCount) << std::endl;
            std::cout << "Queue flags: " + std::to_string(QueueFamilies[QueueFamilyIndex].queueFlags) << std::endl;

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

    South::GraphicCard* GraphicCard::Create(const CreateInfo& InCreateInfo) { return nullptr; }

    void GraphicCard::CreateLogicalDeviceWithQueues(VkDevice OutLogicalDevice,
                                                    std::vector<Queue*>& OutQueues,
                                                    const std::vector<QueueConditionCallback>& InConditions)
    {
    }

} // namespace South