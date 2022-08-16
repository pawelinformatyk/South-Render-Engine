#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    class Queue
    {
    public:
        uint32_t m_QueueFamily = 0;
        VkQueue m_Queue        = nullptr;
    };

    // Class representing one graphic card specified by requirements.
    // After creation can create logical devices.
    class GraphicCard
    {
    public:
        using QueueConditionCallback = std::function<bool(const VkQueueFamilyProperties&, int Index)>;

        struct CreateInfo
        {
            VkInstance VulkanInstance = nullptr;
            std::vector<const char*> RequiredExtensions;
            VkPhysicalDeviceFeatures RequiredFeatures;
            VkPhysicalDeviceType Type;
            int RequiredQueueFlags;
        };

        static GraphicCard* Create(const CreateInfo& InCreateInfo);

        // Creates new logical device with one graphic queue.
        bool CreateLogicalDevice(VkDevice& OutLogicalDevice, Queue& OutGraphicQueue) const;

        // Creates new logical device with queues that met conditions.
        // One queue one condition.
        void CreateLogicalDevice(VkDevice OutLogicalDevice,
                                 std::vector<Queue*>& OutQueues,
                                 const std::vector<QueueConditionCallback>& InConditions) const;

        VkPhysicalDevice GetPhysicalDevice() const;
        const VkPhysicalDeviceFeatures& GetFeatures() const;
        const VkPhysicalDeviceProperties& GetProperties() const;
        const std::string& GetTypeName() const;

    private:
        std::optional<uint32_t> FindQueueFamilyIndex(VkQueueFlagBits InFlags) const;

        std::vector<const char*> m_ExtensionsNames;
        VkPhysicalDeviceFeatures m_Features;
        VkPhysicalDeviceProperties m_Properties;
        std::string TypeName;

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
