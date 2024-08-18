#pragma once
#include <vulkan/vulkan_core.h>

namespace South
{

class SCommandPool
{
public:
    struct SCreateInfo
    {
        VkCommandPoolCreateFlags Flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        uint32_t                 QueueFamilyIndex = 0;
    };

    static SCommandPool* Create(VkDevice InLogicalDevice, const SCreateInfo& Info);
    static void          Destroy(SCommandPool& InCmdPool);

    VkCommandPool GetPool() const;

private:
    SCommandPool() = default;

    VkCommandPoolCreateFlags m_Flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    uint32_t                 m_QueueFamilyIndex = 0;
    VkCommandPool            m_VulkanPool       = nullptr;
};

} // namespace South