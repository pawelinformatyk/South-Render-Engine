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
        };

        static GraphicCard* Create(const CreateInfo& InCreateInfo);
        // #TODO :
        // static std::vector<GraphicCard*> Create(const CreateInfo& InCreateInfo);

        // Creates new logical device with one graphic queue.
        bool CreateLogicalDevice(VkDevice& OutLogicalDevice, Queue& OutGraphicQueue) const;

        // Creates new logical device with queues that met conditions.
        // One queue one condition.
        void CreateLogicalDevice(VkDevice OutLogicalDevice,
                                 std::vector<Queue*>& OutQueues,
                                 const std::vector<QueueConditionCallback>& InConditions) const;

        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const;
        [[nodiscard]] const VkPhysicalDeviceFeatures& GetFeatures() const;
        [[nodiscard]] const VkPhysicalDeviceProperties& GetProperties() const;
        [[nodiscard]] const std::string& GetTypeName() const;

    private:
        static VkPhysicalDevice FindPhysicalDevice(const CreateInfo& InCreateInfo);
        static bool CheckFeatures(const VkPhysicalDevice InGpu, const VkPhysicalDeviceFeatures& InRequiredFeatures);
        static bool CheckExtensions(const VkPhysicalDevice InGpu, const std::vector<const char*>& InRequiredExtensions);
        static std::optional<uint32_t> FindQueueFamilyIndex(const VkPhysicalDevice InGpu, VkQueueFlagBits InFlags);

        VkPhysicalDevice m_PhysicalDevice          = nullptr;
        std::vector<const char*> m_ExtensionsNames = {};
        VkPhysicalDeviceFeatures m_Features        = {};
        VkPhysicalDeviceProperties m_Properties    = {};
        std::string m_TypeName;
    };

} // namespace South
