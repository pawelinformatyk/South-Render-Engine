#include "sthpch.h"

#include "Core/Buffers/VertexIndexBuffer.h"

namespace South
{

VertexIndexBuffer* VertexIndexBuffer::Create(const VkDevice        InLogicalDev,
                                             const VkCommandBuffer InCmdBuffer,
                                             const VkQueue         InQueue,
                                             const CreateInfo&     InVertexInfo,
                                             const CreateInfo&     InIndexInfo)
{
    // #TODO : Check if actually is more performant than two separated buffers. Check
    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT

    const uint32_t VerticesSize = InVertexInfo.Count * InVertexInfo.Size;
    const uint32_t IndicesSize  = InIndexInfo.Count * InIndexInfo.Size;

    VkBuffer           StagingBuffer;
    VkBufferCreateInfo BufferCi {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = nullptr,
        .size        = VerticesSize + IndicesSize,
        .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    vkCreateBuffer(InLogicalDev, &BufferCi, nullptr, &StagingBuffer);

    VkMemoryRequirements MemRequirements;
    vkGetBufferMemoryRequirements(InLogicalDev, StagingBuffer, &MemRequirements);

    VkDeviceMemory       StagingBufferMemory;
    VkMemoryAllocateInfo MemoryAllocateInfo {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = MemRequirements.size,
        .memoryTypeIndex = 2, // #TODO : Should not be hardcoded.
    };
    vkAllocateMemory(InLogicalDev, &MemoryAllocateInfo, nullptr, &StagingBufferMemory);

    vkBindBufferMemory(InLogicalDev, StagingBuffer, StagingBufferMemory, 0);

    void* BufferData = nullptr;

    vkMapMemory(InLogicalDev, StagingBufferMemory, 0, VerticesSize, 0, &BufferData);
    memcpy(BufferData, InVertexInfo.Data, static_cast<size_t>(VerticesSize));
    vkUnmapMemory(InLogicalDev, StagingBufferMemory);

    BufferData = nullptr;

    vkMapMemory(InLogicalDev, StagingBufferMemory, VerticesSize, IndicesSize, 0, &BufferData);
    memcpy(BufferData, InIndexInfo.Data, static_cast<size_t>(IndicesSize));
    vkUnmapMemory(InLogicalDev, StagingBufferMemory);

    VkBuffer TargetBuffer;
    BufferCi.usage       = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    BufferCi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(InLogicalDev, &BufferCi, nullptr, &TargetBuffer);

    vkGetBufferMemoryRequirements(InLogicalDev, TargetBuffer, &MemRequirements);

    VkDeviceMemory TargetBufferMemory;
    MemoryAllocateInfo.allocationSize  = MemRequirements.size;
    MemoryAllocateInfo.memoryTypeIndex = 1; // #TODO : Should not be hardcoded.
    vkAllocateMemory(InLogicalDev, &MemoryAllocateInfo, nullptr, &TargetBufferMemory);

    vkBindBufferMemory(InLogicalDev, TargetBuffer, TargetBufferMemory, 0);

    // Copy data from staging buffer to actual buffer.

    // #TODO : Move to renderer or context as function idk.
    const VkCommandBufferBeginInfo BeginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    const VkBufferCopy CopyRegion {
        .srcOffset = 0,
        .dstOffset = 0,
        .size      = VerticesSize + IndicesSize,
    };

    vkBeginCommandBuffer(InCmdBuffer, &BeginInfo);
    vkCmdCopyBuffer(InCmdBuffer, StagingBuffer, TargetBuffer, 1, &CopyRegion);
    vkEndCommandBuffer(InCmdBuffer);

    const VkSubmitInfo SubmitInfo {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext              = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers    = &InCmdBuffer,
    };

    vkQueueSubmit(InQueue, 1, &SubmitInfo, nullptr);
    vkQueueWaitIdle(InQueue);

    vkDestroyBuffer(InLogicalDev, StagingBuffer, nullptr);
    vkFreeMemory(InLogicalDev, StagingBufferMemory, nullptr);

    auto* ViBuffer = new VertexIndexBuffer;

    ViBuffer->m_Buffer        = TargetBuffer;
    ViBuffer->m_Memory        = TargetBufferMemory;
    ViBuffer->m_VerticesSize  = VerticesSize;
    ViBuffer->m_VerticesCount = InVertexInfo.Count;
    ViBuffer->m_IndicesSize   = IndicesSize;
    ViBuffer->m_IndicesCount  = InIndexInfo.Count;

    return ViBuffer;
}


uint32_t VertexIndexBuffer::GetVerticesCount() const
{
    return m_VerticesCount;
}

uint32_t VertexIndexBuffer::GetIndicesCount() const
{
    return m_IndicesCount;
}

uint32_t VertexIndexBuffer::GetVerticesSize() const
{
    return m_VerticesSize;
}

uint32_t VertexIndexBuffer::GetIndicesSize() const
{
    return m_IndicesSize;
}
} // namespace South
