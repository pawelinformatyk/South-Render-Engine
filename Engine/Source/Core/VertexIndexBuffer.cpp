#include "sthpch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/VertexIndexBuffer.h"
#include "Core/GraphicCard.h"

namespace South
{

    VertexIndexBuffer* VertexIndexBuffer::Create(const CreateInfo& InVertexInfo, const CreateInfo& InIndexInfo)
    {
        // #TODO :
        // Move some of this fucntion to RendererContext - it should handle most of that stuff
        // + less getters
        // + more condensed/centralised/autonomy renderer.
        // Abstract buffer, staging buffer.

        const RendererContext& Context = Renderer::GetContext();
        VkDevice LogicalDev            = Context.GetLogicalDevice();
        VkPhysicalDevice PhysDev       = Context.GetGraphicCard().GetPhysicalDevice();
        VkCommandPool CmdPool          = Context.GetCommandPool();
        VkCommandBuffer CmdBuffer      = Context.GetCommandBuffer();
        VkQueue GraphicsQueue          = Context.GetGraphicQueue().m_Queue;

        const uint32_t VerticesSize = InVertexInfo.Count * InVertexInfo.Size;
        const uint32_t IndicesSize  = InIndexInfo.Count * InIndexInfo.Size;

        VkPhysicalDeviceMemoryProperties MemProperties;
        vkGetPhysicalDeviceMemoryProperties(PhysDev, &MemProperties);

        const VkBufferCreateInfo StagingBufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = VerticesSize + IndicesSize,
            .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VkBuffer StagingBuffer;
        vkCreateBuffer(LogicalDev, &StagingBufferInfo, nullptr, &StagingBuffer);

        VkMemoryRequirements StagingMemRequirements;
        vkGetBufferMemoryRequirements(LogicalDev, StagingBuffer, &StagingMemRequirements);

        const VkMemoryAllocateInfo StagingAllocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = StagingMemRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(MemProperties,
                               StagingMemRequirements.memoryTypeBits,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),

        };

        VkDeviceMemory StagingBufferMemory;
        vkAllocateMemory(LogicalDev, &StagingAllocInfo, nullptr, &StagingBufferMemory);

        vkBindBufferMemory(LogicalDev, StagingBuffer, StagingBufferMemory, 0);

        void* StaginBufferData;

        StaginBufferData = nullptr;

        vkMapMemory(LogicalDev, StagingBufferMemory, 0, VerticesSize, 0, &StaginBufferData);
        memcpy(StaginBufferData, InVertexInfo.Data, static_cast<size_t>(VerticesSize));
        vkUnmapMemory(LogicalDev, StagingBufferMemory);

        StaginBufferData = nullptr;

        vkMapMemory(LogicalDev, StagingBufferMemory, VerticesSize, IndicesSize, 0, &StaginBufferData);
        memcpy(StaginBufferData, InIndexInfo.Data, static_cast<size_t>(IndicesSize));
        vkUnmapMemory(LogicalDev, StagingBufferMemory);

        // Actual buffer.

        const VkBufferCreateInfo BufferInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size  = StagingBufferInfo.size,
            .usage =
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VkBuffer Buffer;
        vkCreateBuffer(LogicalDev, &BufferInfo, nullptr, &Buffer);

        VkMemoryRequirements MemRequirements;
        vkGetBufferMemoryRequirements(LogicalDev, Buffer, &MemRequirements);

        VkMemoryAllocateInfo AllocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = MemRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(MemProperties, MemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        };

        VkDeviceMemory Memory;
        vkAllocateMemory(LogicalDev, &AllocInfo, nullptr, &Memory);

        vkBindBufferMemory(LogicalDev, Buffer, Memory, 0);

        // Copy data from staging buffer to actual buffer.

        const VkCommandBufferBeginInfo BeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        const VkBufferCopy CopyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = StagingBufferInfo.size,
        };

        // Move to renderer (submit).
        vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

        vkCmdCopyBuffer(CmdBuffer, StagingBuffer, Buffer, 1, &CopyRegion);

        vkEndCommandBuffer(CmdBuffer);

        const VkSubmitInfo SubmitInfo{
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext              = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers    = &CmdBuffer,
        };

        vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(GraphicsQueue);

        vkDestroyBuffer(LogicalDev, StagingBuffer, nullptr);
        vkFreeMemory(LogicalDev, StagingBufferMemory, nullptr);

        VertexIndexBuffer* VIBuffer = new VertexIndexBuffer;

        VIBuffer->m_Buffer        = Buffer;
        VIBuffer->m_Memory        = Memory;
        VIBuffer->m_VerticesSize  = VerticesSize;
        VIBuffer->m_VerticesCount = InVertexInfo.Count;
        VIBuffer->m_IndicesSize   = IndicesSize;
        VIBuffer->m_IndicesCount  = InIndexInfo.Count;

        return VIBuffer;
    }

    void VertexIndexBuffer::Destroy(VkDevice InlogicalDev, VertexIndexBuffer& InVIBuffer)
    {
        vkDestroyBuffer(InlogicalDev, InVIBuffer.m_Buffer, nullptr);
        vkFreeMemory(InlogicalDev, InVIBuffer.m_Memory, nullptr);

        delete &InVIBuffer;
    }

    VkBuffer VertexIndexBuffer::GetVulkanBuffer() const { return m_Buffer; }

    uint32_t VertexIndexBuffer::GetVerticesCount() const { return m_VerticesCount; }

    uint32_t VertexIndexBuffer::GetIndicesCount() const { return m_IndicesCount; }

    uint32_t VertexIndexBuffer::GetVerticesSize() const { return m_VerticesSize; }

    uint32_t VertexIndexBuffer::GetIndicesSize() const { return m_IndicesSize; }

    uint32_t VertexIndexBuffer::FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties,
                                               uint32_t typeFilter,
                                               VkMemoryPropertyFlags properties)
    {
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        return typeFilter;
    }

} // namespace South
