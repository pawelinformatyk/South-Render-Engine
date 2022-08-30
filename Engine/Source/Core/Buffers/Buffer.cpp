#include "sthpch.h"

#include "Buffer.h"

void South::Buffer::MapMemory(const VkDevice InLogicalDev,
                              const VkDeviceMemory InBufferMemory,
                              const VkDeviceSize& InOffset,
                              const VkDeviceSize& InSize,
                              const void* InData)
{
    void* BufferData = nullptr;

    vkMapMemory(InLogicalDev, InBufferMemory, InOffset, InSize, 0, &BufferData);
    memcpy(BufferData, InData, static_cast<size_t>(InSize));
    vkUnmapMemory(InLogicalDev, InBufferMemory);
}

void South::Buffer::CreateBufferAndMemory(const VkDevice InLogicalDev,
                                          VkBuffer& OutBuffer,
                                          VkDeviceMemory& OutBufferMemory,
                                          const VkDeviceSize& InSize,
                                          VkBufferUsageFlags InUsage,
                                          VkSharingMode InSharingMode,
                                          const VkPhysicalDeviceMemoryProperties InMemoryProperties,
                                          int InMemoryFlags)
{
    const VkBufferCreateInfo BufferCi{
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = nullptr,
        .size        = InSize,
        .usage       = InUsage,
        .sharingMode = InSharingMode,
    };
    vkCreateBuffer(InLogicalDev, &BufferCi, nullptr, &OutBuffer);

    VkMemoryRequirements MemRequirements;
    vkGetBufferMemoryRequirements(InLogicalDev, OutBuffer, &MemRequirements);

    const VkMemoryAllocateInfo MemoryAllocateInfo{
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = MemRequirements.size,
        .memoryTypeIndex = FindMemoryType(InMemoryProperties, MemRequirements.memoryTypeBits, InMemoryFlags),

    };
    vkAllocateMemory(InLogicalDev, &MemoryAllocateInfo, nullptr, &OutBufferMemory);

    vkBindBufferMemory(InLogicalDev, OutBuffer, OutBufferMemory, 0);
}

uint32_t South::Buffer::FindMemoryType(const VkPhysicalDeviceMemoryProperties InMemProperties,
                                       const uint32_t InTypeFilter,
                                       const VkMemoryPropertyFlags InProperties)
{
    for (uint32_t i = 0; i < InMemProperties.memoryTypeCount; i++)
    {
        if ((InTypeFilter & (1 << i)) && (InMemProperties.memoryTypes[i].propertyFlags & InProperties) == InProperties)
        {
            return i;
        }
    }

    return InTypeFilter;
}
