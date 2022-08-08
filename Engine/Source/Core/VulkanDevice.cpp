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

        //#TODO : Rate and pick the best gpu.

        const std::optional<uint32_t> QueueFamily = FindQueueFamily(GPUs[0], InInfo.Surface, InInfo.QueueFlags);
        if (!QueueFamily.has_value()) { return nullptr; }

        // Set logical device and queue.
        const float QueuePrio = 1.f;

        const VkDeviceQueueCreateInfo QCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0x0,
            .queueFamilyIndex = QueueFamily.value(),
            .queueCount       = 1,
            .pQueuePriorities = &QueuePrio,
        };

        const VkDeviceCreateInfo LogicDeviceCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0x0,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &QCreateInfo,
            .enabledExtensionCount   = static_cast<uint32_t>(InInfo.RequiredGPUExtensions.size()),
            .ppEnabledExtensionNames = InInfo.RequiredGPUExtensions.data(),
            .pEnabledFeatures        = InInfo.RequiredGPUFeatures.data(),
        };

        VulkanDevice* NewVulkanDevice  = new VulkanDevice;
        NewVulkanDevice->m_GraphicCard = GPUs[0];
        NewVulkanDevice->m_QueueFamily = QueueFamily.value();

        vkCreateDevice(NewVulkanDevice->m_GraphicCard,
                       &LogicDeviceCreateInfo,
                       nullptr,
                       &NewVulkanDevice->m_LogicalDevice);

        vkGetDeviceQueue(NewVulkanDevice->m_LogicalDevice,
                         NewVulkanDevice->m_QueueFamily,
                         0,
                         &NewVulkanDevice->m_Queue);

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

    uint32_t VulkanDevice::GetQFamilyIndex() const { return m_QueueFamily; }

    VkQueue VulkanDevice::GetQ() const { return m_Queue; }

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
            VkBool32 bPresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(GPU, QueueFamilyIndex, Surface, &bPresentSupport);

            if (bPresentSupport && (QueueFamilies[QueueFamilyIndex].queueFlags & Flags))
            {
                return std::optional<uint32_t>(QueueFamilyIndex);
            }
        }

        return std::optional<uint32_t>();
    }

    VkPhysicalDevice VulkanDevice::PickBestGPU(const std::vector<VkPhysicalDevice>& GPUs)
    {
        // #TODO : Rate GPUs.
        return GPUs[0];
    }

} // namespace South