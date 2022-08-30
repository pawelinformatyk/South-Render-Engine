#include "sthpch.h"

#include "Core/Buffers/VertexIndexBuffer.h"
#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"

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
        VkCommandBuffer CmdBuffer      = Context.GetCommandBuffer();
        VkQueue GraphicsQueue          = Context.GetGraphicQueue().m_Queue;

        const uint32_t VerticesSize = InVertexInfo.Count * InVertexInfo.Size;
        const uint32_t IndicesSize  = InIndexInfo.Count * InIndexInfo.Size;

        VkPhysicalDeviceMemoryProperties MemProperties;
        vkGetPhysicalDeviceMemoryProperties(PhysDev, &MemProperties);

        VkBuffer StagingBuffer;
        VkDeviceMemory StagingBufferMemory;
        CreateBufferAndMemory(LogicalDev,
                              StagingBuffer,
                              StagingBufferMemory,
                              VerticesSize + IndicesSize,
                              VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                              VK_SHARING_MODE_EXCLUSIVE,
                              MemProperties,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        MapMemory(LogicalDev, StagingBufferMemory, 0, VerticesSize, InVertexInfo.Data);
        MapMemory(LogicalDev, StagingBufferMemory, VerticesSize, IndicesSize, InVertexInfo.Data);

        // Actual buffer.

        VkBuffer Buffer;
        VkDeviceMemory Memory;
        CreateBufferAndMemory(LogicalDev,
                              Buffer,
                              Memory,
                              VerticesSize + IndicesSize,
                              VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                                  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                              VK_SHARING_MODE_EXCLUSIVE,
                              MemProperties,
                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy data from staging buffer to actual buffer.

        const VkCommandBufferBeginInfo BeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        const VkBufferCopy CopyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = VerticesSize + IndicesSize,
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

        auto* ViBuffer = new VertexIndexBuffer;

        ViBuffer->m_Buffer        = Buffer;
        ViBuffer->m_Memory        = Memory;
        ViBuffer->m_VerticesSize  = VerticesSize;
        ViBuffer->m_VerticesCount = InVertexInfo.Count;
        ViBuffer->m_IndicesSize   = IndicesSize;
        ViBuffer->m_IndicesCount  = InIndexInfo.Count;

        return ViBuffer;
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
} // namespace South
