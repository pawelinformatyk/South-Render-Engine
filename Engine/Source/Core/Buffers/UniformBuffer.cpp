#include "sthpch.h"

#include "UniformBuffer.h"

namespace South
{
    UniformBuffer* UniformBuffer::Create(const VkDevice InLogicalDev, const CreateInfo& InCreateInfo)
    {
        VkBuffer Buffer;
        const VkBufferCreateInfo BufferCi{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = InCreateInfo.Size,
            .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };
        vkCreateBuffer(InLogicalDev, &BufferCi, nullptr, &Buffer);

        VkMemoryRequirements MemRequirements;
        vkGetBufferMemoryRequirements(InLogicalDev, Buffer, &MemRequirements);

        VkDeviceMemory BufferMemory;
        const VkMemoryAllocateInfo MemoryAllocateInfo{
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext           = nullptr,
            .allocationSize  = MemRequirements.size,
            .memoryTypeIndex = 2 // #TODO : Should not be hardcoded.,
        };
        vkAllocateMemory(InLogicalDev, &MemoryAllocateInfo, nullptr, &BufferMemory);

        vkBindBufferMemory(InLogicalDev, Buffer, BufferMemory, 0);

        auto* Ub = new UniformBuffer;

        Ub->m_Buffer = Buffer;
        Ub->m_Memory = BufferMemory;
        Ub->SetData(InLogicalDev, 0, InCreateInfo.Size, InCreateInfo.Data);

        return Ub;
    }
} // namespace South
