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

    static GraphicCard* Create(VkInstance InVulkanInstance, const CreateInfo& InCreateInfo);

    VkPhysicalDevice                  GetPhysicalDevice() const;
    const std::vector<const char*>&   GetExtensionsNames() const;
    const VkPhysicalDeviceFeatures&   GetFeatures() const;
    const VkPhysicalDeviceProperties& GetProperties() const;
    std::string_view                  GetTypeName() const;

private:
    VkPhysicalDevice           m_PhysicalDevice  = nullptr;
    std::vector<const char*>   m_ExtensionsNames = {};
    VkPhysicalDeviceFeatures   m_Features        = {};
    VkPhysicalDeviceProperties m_Properties      = {};
    std::string                m_TypeName        = {};
};


/*
 * Most operations in Vulkan, anything from drawing to uploading textures, requires commands to be submitted to a queue.
 * Queues originate from different queue families that allow only a certain subset of commands (Example: queue family that only allows
 * processing of compute commands or one that only allows memory transfer related commands.
 *
 * Logical device purpose it to interface with physical device. We also need to specify which
 * queues to create now that we've queried which queue families are available. You can even create multiple logical devices from
 * the same physical device if you have varying requirements.
 */
class LogicalDeviceAndQueues
{
public:
    static LogicalDeviceAndQueues* Create(const GraphicCard& InGpu, VkSurfaceKHR InSurface);
    static void                    Destroy(LogicalDeviceAndQueues& InDeviceAndQueues);

    VkDevice GetLogicalDevice() const;
    VkQueue  GetGraphicQueue() const;
    uint32_t GetGraphicQueueFamilyIndex() const;
    VkQueue  GetPresentQueue() const;
    uint32_t GetPresentQueueFamilyIndex() const;

private:
    VkDevice m_LogicalDevice = nullptr;

    VkQueue  m_GraphicQueue            = nullptr;
    uint32_t m_GraphicQueueFamilyIndex = 0;

    VkQueue  m_PresentQueue            = nullptr;
    uint32_t m_PresentQueueFamilyIndex = 0;
};

} // namespace South
