#include "CommandPool.h"

#include "Core/Devices/GraphicCard.h"
#include "Renderer.h"

namespace South
{

CommandPool* CommandPool::Create(const VkDevice InLogicalDevice, const CreateInfo& Info)
{
    auto* OutPool               = new CommandPool();
    OutPool->m_Flags            = Info.Flags;
    OutPool->m_QueueFamilyIndex = Info.QueueFamilyIndex;

    const VkCommandPoolCreateInfo PoolInfo {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = nullptr,
        .flags            = OutPool->m_Flags,
        .queueFamilyIndex = OutPool->m_QueueFamilyIndex,
    };

    vkCreateCommandPool(InLogicalDevice, &PoolInfo, nullptr, &OutPool->m_VulkanPool);

    return OutPool;
}

void CommandPool::Destroy(CommandPool& InCmdPool)
{
    vkDestroyCommandPool(RendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), InCmdPool.m_VulkanPool, nullptr);
    InCmdPool.m_VulkanPool = nullptr;
}

VkCommandPool CommandPool::GetPool() const
{
    return m_VulkanPool;
}

} // namespace South