#include "sthpch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/VertexIndexBuffer.h"
#include "Core/VulkanDevice.h"

namespace South
{

    VertexIndexBuffer::VertexIndexBuffer(const void* vData, uint32_t vSize, const void* iData, uint32_t iSize)
    {
        const RendererContext& Context = Renderer::GetContext();
        const VulkanDevice& VulkanDev  = Context.GetGpuDevice();
        VkDevice LogicalDev            = VulkanDev.GetDevice();
        VkPhysicalDevice PhysDev       = VulkanDev.GetPhysicalDevice();
        VkCommandPool CmdPool          = Context.GetCommandPool();
        VkCommandBuffer CmdBuffer      = Context.GetCommandBuffer();

        VkPhysicalDeviceMemoryProperties MemProperties;
        vkGetPhysicalDeviceMemoryProperties(PhysDev, &MemProperties);

        m_IndexOffset = vSize;

        const VkBufferCreateInfo StagingBufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = vSize + iSize,
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

        vkMapMemory(LogicalDev, StagingBufferMemory, 0, vSize, 0, &StaginBufferData);
        memcpy(StaginBufferData, vData, static_cast<size_t>(vSize));
        vkUnmapMemory(LogicalDev, StagingBufferMemory);

        StaginBufferData = nullptr;

        vkMapMemory(LogicalDev, StagingBufferMemory, m_IndexOffset, iSize, 0, &StaginBufferData);
        memcpy(StaginBufferData, iData, static_cast<size_t>(iSize));
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

        const VkCommandBufferBeginInfo BeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

        const VkBufferCopy CopyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = StagingBufferInfo.size,
        };

        vkCmdCopyBuffer(CmdBuffer, StagingBuffer, m_Buffer, 1, &CopyRegion);

        vkEndCommandBuffer(CmdBuffer);

        const VkSubmitInfo SubmitInfo{
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext              = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers    = &CmdBuffer,
        };

        VkQueue GraphicsQueue = VulkanDev.GetQ();

        vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(GraphicsQueue);

        vkDestroyBuffer(LogicalDev, StagingBuffer, nullptr);
        vkFreeMemory(LogicalDev, StagingBufferMemory, nullptr);
    }

    VertexIndexBuffer::~VertexIndexBuffer()
    {
        VkDevice logicalDev = Renderer::GetContext().GetGpuDevice().GetDevice();

        vkDestroyBuffer(logicalDev, m_Buffer, nullptr);
        vkFreeMemory(logicalDev, m_Memory, nullptr);
    }

    VkBuffer VertexIndexBuffer::GetVulkanBuffer() const { return m_Buffer; }

    uint32_t VertexIndexBuffer::GetIndexOffset() const { return m_IndexOffset; }

    uint32_t VertexIndexBuffer::FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties,
                                               uint32_t typeFilter,
                                               VkMemoryPropertyFlags properties) const
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
