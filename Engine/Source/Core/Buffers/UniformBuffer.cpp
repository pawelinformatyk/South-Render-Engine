#include "UniformBuffer.h"

namespace South
{

SUniformBuffer* SUniformBuffer::Create(const VkDevice InLogicalDev)
{
    VkBuffer                 Buffer;
    const VkBufferCreateInfo BufferCi {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = nullptr,
        .size        = sizeof(UniformBufferObject),
        .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    vkCreateBuffer(InLogicalDev, &BufferCi, nullptr, &Buffer);

    VkMemoryRequirements MemRequirements;
    vkGetBufferMemoryRequirements(InLogicalDev, Buffer, &MemRequirements);

    VkDeviceMemory             BufferMemory;
    const VkMemoryAllocateInfo MemoryAllocateInfo {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = MemRequirements.size,
        .memoryTypeIndex = 2 // #TODO : Should not be hardcoded.,
    };
    vkAllocateMemory(InLogicalDev, &MemoryAllocateInfo, nullptr, &BufferMemory);

    vkBindBufferMemory(InLogicalDev, Buffer, BufferMemory, 0);

    auto* Ub = new SUniformBuffer;

    Ub->m_Buffer = Buffer;
    Ub->m_Memory = BufferMemory;

    return Ub;
}

void SUniformBuffer::SetData(const VkDevice InLogicalDev, const void* InData)
{
    void* BufferData = nullptr;

    vkMapMemory(InLogicalDev, m_Memory, 0, sizeof(UniformBufferObject), 0, &BufferData);
    memcpy(BufferData, InData, sizeof(UniformBufferObject));
    vkUnmapMemory(InLogicalDev, m_Memory);
}

} // namespace South
