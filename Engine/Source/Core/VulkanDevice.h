#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // #TODO : Move queues and logical device to another class? There can be more of them sooo unlike GPU.
    class VulkanDevice
    {
    public:
        struct CreateInfo
        {
            VkInstance VulkanInstance = nullptr;
            VkSurfaceKHR Surface      = nullptr;
            std::vector<const char*> RequiredGPUExtensions;
            std::vector<VkPhysicalDeviceFeatures> RequiredGPUFeatures;
            VkPhysicalDeviceType GPUType;
            VkQueueFlagBits QueueFlags;
        };

        static VulkanDevice* Create(const CreateInfo& InCreateInfo);
        static void Destroy(VulkanDevice* InDevice);

        VkPhysicalDevice GetPhysicalDevice() const;
        VkDevice GetDevice() const;
        uint32_t GetQFamilyIndex() const;
        VkQueue GetQ() const;

    private:
        static std::vector<VkPhysicalDevice>
            FindSuitableGraphicCards(VkInstance VulkanInstance,
                                     VkSurfaceKHR Surface,
                                     const std::vector<const char*>& RequiredDeviceExtensions,
                                     VkPhysicalDeviceType GPUType);
        static std::optional<uint32_t>
            FindQueueFamily(VkPhysicalDevice GPU, VkSurfaceKHR Surface, VkQueueFlagBits Flags);
        static VkPhysicalDevice PickBestGPU(const std::vector<VkPhysicalDevice>& GPUs);

        // Graphic card that was found based on requirements from CreateInfo.
        VkPhysicalDevice m_GraphicCard = VK_NULL_HANDLE;

        // Interface for GPU.
        VkDevice m_LogicalDevice = VK_NULL_HANDLE;

        // Family queue that allows only a subset of commands specified in CreateInfo.QueueFlags.
        uint32_t m_QueueFamily = 0;

        // Handle to interface with queues from LogicalDevice.
        VkQueue m_Queue = VK_NULL_HANDLE;
    };


} // namespace South
