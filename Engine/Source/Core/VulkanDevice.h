#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    class Queue
    {
    public:
        VkQueue GetQueue() const { return m_Queue; };
        uint32_t GetQueueFamily() const { return m_QueueFamily; };

    private:
        uint32_t m_QueueFamily = 0;
        VkQueue m_Queue        = nullptr;
    };

    // Class representing one graphic card specified by requirements.
    class GraphicCard
    {
    public:
        using QueueConditionCallback = std::function<bool(const VkQueueFamilyProperties&, int Index)>;

        struct CreateInfo
        {
            VkInstance VulkanInstance = nullptr;
            std::vector<const char*> RequiredExtensions;
            std::vector<VkPhysicalDeviceFeatures> RequiredFeatures;
            VkPhysicalDeviceType Type;
        };

        static GraphicCard* Create(const CreateInfo& InCreateInfo);

        void CreateLogicalDeviceWithQueues(VkDevice OutLogicalDevice,
                                           std::vector<Queue*>& OutQueues,
                                           const std::vector<QueueConditionCallback>& InConditions);

    private:
        // Vulkan handle to graphic card.
        VkPhysicalDevice m_PhysicalDevice = nullptr;
    };

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
        VkQueue GetGraphicQueue() const;

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

        // Interface for GPU. Queues are created along LogicalDevice.
        VkDevice m_LogicalDevice = VK_NULL_HANDLE;

        // Family queue that allows only a subset of commands specified in CreateInfo.QueueFlags.
        uint32_t m_GraphicQueueFamily = 0;

        // Family queue that allows only a subset of commands specified in CreateInfo.QueueFlags.
        uint32_t m_TransferQueueFamily = 0;

        // Family queue that allows only a subset of commands specified in CreateInfo.QueueFlags.
        uint32_t m_PresentQueueFamily = 0;

        // Handle to interface with queues from LogicalDevice.
        VkQueue m_GraphicQueue = VK_NULL_HANDLE;

        // Handle to interface with queues from LogicalDevice.
        VkQueue m_PresentQueue = VK_NULL_HANDLE;

        // Handle to interface with queues from LogicalDevice.
        VkQueue m_TransferQueue = VK_NULL_HANDLE;
    };


} // namespace South
