#pragma once
#include <vulkan/vulkan_core.h>

namespace South
{

class CommandPool
{
public:
    struct CreateInfo
    {
        VkCommandPoolCreateFlags Flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        uint32_t                 QueueFamilyIndex = 0;
    };

    static CommandPool* Create(VkDevice InLogicalDevice, const CreateInfo& Info);
    static void         Destroy(CommandPool& InCmdPool);

    VkCommandPool GetPool() const;

private:
    CommandPool() = default;

    VkCommandPoolCreateFlags m_Flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    uint32_t                 m_QueueFamilyIndex = 0;
    VkCommandPool            m_VulkanPool       = nullptr;
};

} // namespace South