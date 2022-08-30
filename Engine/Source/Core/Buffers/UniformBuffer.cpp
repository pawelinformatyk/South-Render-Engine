#include "sthpch.h"

#include "UniformBuffer.h"

namespace South
{
    UniformBuffer* UniformBuffer::Create(const CreateInfo& InCreateInfo)
    {
        // VkBufferCreateInfo bufferInfo{
        //     .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        //     .pNext       = nullptr,
        //     .size        = size,
        //     .usage       = usage,
        //     .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        // };
        // vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
        //
        // VkMemoryRequirements memRequirements;
        // vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
        //
        // VkMemoryAllocateInfo allocInfo{
        //     .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        //     .allocationSize  = memRequirements.size,
        //     .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties),
        // };
        // vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
        //
        // vkBindBufferMemory(device, buffer, bufferMemory, 0);

        return nullptr;
    }

    void UniformBuffer::Destroy(VkDevice InlogicalDev, UniformBuffer& InBuffer) {}

    VkBuffer UniformBuffer::GetVulkanBuffer() const { return m_Buffer; }
} // namespace South
