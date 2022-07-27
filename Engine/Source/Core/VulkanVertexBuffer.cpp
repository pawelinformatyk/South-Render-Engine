#include "sthpch.h"

#include "Core/VulkanVertexBuffer.h"
#include "Core/VulkanContext.h"
#include "Core/VulkanDevice.h"

namespace South
{

    VulkanVertexBuffer::VulkanVertexBuffer(const void* Data, uint32_t Size)
    {
        // #TODO : Asserts
        const VulkanDevice& VulkanDev = VulkanContext::Get().GetCurrentDevice();
        VkDevice LogicalDev           = VulkanDev.GetDevice();
        VkPhysicalDevice PhysDev      = VulkanDev.GetPhysicalDevice();

        VkPhysicalDeviceMemoryProperties MemProperties;
        vkGetPhysicalDeviceMemoryProperties(PhysDev, &MemProperties);

        // Staging buffer.

        VkBufferCreateInfo StagingBufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = Size,
            .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VkBuffer StagingBuffer;
        vkCreateBuffer(LogicalDev, &StagingBufferInfo, nullptr, &StagingBuffer);

        VkMemoryRequirements StagingMemRequirements;
        vkGetBufferMemoryRequirements(LogicalDev, StagingBuffer, &StagingMemRequirements);

        VkMemoryAllocateInfo StagingAllocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = StagingMemRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(MemProperties, StagingMemRequirements.memoryTypeBits,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),

        };

        VkDeviceMemory StagingBufferMemory;
        vkAllocateMemory(LogicalDev, &StagingAllocInfo, nullptr, &StagingBufferMemory);

        vkBindBufferMemory(LogicalDev, StagingBuffer, StagingBufferMemory, 0);

        void* StaginBufferData;

        vkMapMemory(LogicalDev, StagingBufferMemory, 0, StagingBufferInfo.size, 0, &StaginBufferData);
        memcpy(StaginBufferData, Data, (size_t)StagingBufferInfo.size);
        vkUnmapMemory(LogicalDev, StagingBufferMemory);

        // Actual buffer.

        VkBufferCreateInfo BufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = Size,
            .usage       = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        vkCreateBuffer(LogicalDev, &BufferInfo, nullptr, &m_Buffer);

        VkMemoryRequirements MemRequirements;
        vkGetBufferMemoryRequirements(LogicalDev, m_Buffer, &MemRequirements);

        VkMemoryAllocateInfo AllocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = MemRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(MemProperties, MemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        };

        vkAllocateMemory(LogicalDev, &AllocInfo, nullptr, &m_Memory);

        vkBindBufferMemory(LogicalDev, m_Buffer, m_Memory, 0);

        // Copy data from staging buffer to actual buffer.

        // #TODO : Should create every time I create buffer?
        VkCommandPoolCreateInfo PoolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
            .queueFamilyIndex = VulkanDev.GetQFamilyIndex(),
        };

        VkCommandPool CommandPool = VK_NULL_HANDLE;
        vkCreateCommandPool(LogicalDev, &PoolInfo, nullptr, &CommandPool);

        VkCommandBufferAllocateInfo CommandAllocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = CommandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkCommandBuffer CommandBuffer;
        vkAllocateCommandBuffers(LogicalDev, &CommandAllocInfo, &CommandBuffer);

        VkCommandBufferBeginInfo BeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        vkBeginCommandBuffer(CommandBuffer, &BeginInfo);

        VkBufferCopy CopyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = Size,
        };

        vkCmdCopyBuffer(CommandBuffer, StagingBuffer, m_Buffer, 1, &CopyRegion);

        vkEndCommandBuffer(CommandBuffer);

        VkSubmitInfo SubmitInfo{
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext              = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers    = &CommandBuffer,
        };

        VkQueue GraphicsQueue = VulkanDev.GetQ();

        vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(GraphicsQueue);

        vkFreeCommandBuffers(LogicalDev, CommandPool, 1, &CommandBuffer);

        vkDestroyBuffer(LogicalDev, StagingBuffer, nullptr);
        vkFreeMemory(LogicalDev, StagingBufferMemory, nullptr);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        VkDevice LogicalDev = VulkanContext::Get().GetCurrentDevice().GetDevice();

        vkDestroyBuffer(LogicalDev, m_Buffer, nullptr);
        vkFreeMemory(LogicalDev, m_Memory, nullptr);
    }

    VkBuffer VulkanVertexBuffer::GetBuffer() const { return m_Buffer; }

    uint32_t VulkanVertexBuffer::FindMemoryType(VkPhysicalDeviceMemoryProperties MemProperties, uint32_t TypeFilter,
                                                VkMemoryPropertyFlags Properties) const
    {
        for (uint32_t i = 0; i < MemProperties.memoryTypeCount; i++)
        {
            if ((TypeFilter & (1 << i)) && (MemProperties.memoryTypes[i].propertyFlags & Properties) == Properties)
            {
                return i;
            }
        }

        return TypeFilter;
    }

} // namespace South