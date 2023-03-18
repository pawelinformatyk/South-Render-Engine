#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

class GraphicCard
{
public:
    struct CreateInfo
    {
        std::vector<const char*> RequiredExtensions;
        VkPhysicalDeviceFeatures RequiredFeatures;
    };

    static GraphicCard* Create(VkInstance VulkanInstance, const CreateInfo& InCreateInfo);

    // #TODO: StringView

    VkPhysicalDevice                  GetPhysicalDevice() const;
    const std::vector<const char*>&   GetExtensionsNames() const;
    const VkPhysicalDeviceFeatures&   GetFeatures() const;
    const VkPhysicalDeviceProperties& GetProperties() const;
    const std::string&                GetTypeName() const;

private:
    GraphicCard() = default;

    VkPhysicalDevice           m_PhysicalDevice  = nullptr;
    std::vector<const char*>   m_ExtensionsNames = {};
    VkPhysicalDeviceFeatures   m_Features        = {};
    VkPhysicalDeviceProperties m_Properties      = {};
    std::string                m_TypeName        = {};
};


/**
 * Logical device with graphic and present queues.
 */
class LogicalDeviceAndQueues
{
public:
    static LogicalDeviceAndQueues* Create(const GraphicCard& InGPU, VkSurfaceKHR InSurface);

    VkDevice GetLogicalDevice() const;
    VkQueue  GetGraphicQueue() const;
    uint32_t GetGraphicQueueFamilyIndex() const;
    VkQueue  GetPresentQueue() const;
    uint32_t GetPresentQueueFamilyIndex() const;

private:
    LogicalDeviceAndQueues() = default;

    VkDevice m_LogicalDevice = nullptr;

    VkQueue  m_GraphicQueue            = nullptr;
    uint32_t m_GraphicQueueFamilyIndex = 0;

    VkQueue  m_PresentQueue            = nullptr;
    uint32_t m_PresentQueueFamilyIndex = 0;
};

} // namespace South
