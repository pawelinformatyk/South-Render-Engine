#include "sthpch.h"

#include "Core/IndexBuffer.h"
#include "Core/Renderer/Renderer.h"
#include "Core/VulkanDevice.h"

namespace South
{

    IndexBuffer::IndexBuffer(const void* data, uint32_t size)
    {
        // #TODO : Asserts
        const VulkanDevice& vulkanDev = Renderer::GetContext().GetGpuDevice();
        VkDevice logicalDev           = vulkanDev.GetDevice();
        VkPhysicalDevice physDev      = vulkanDev.GetPhysicalDevice();

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physDev, &memProperties);

        // Staging buffer.

        const VkBufferCreateInfo stagingBufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = size,
            .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VkBuffer stagingBuffer;
        vkCreateBuffer(logicalDev, &stagingBufferInfo, nullptr, &stagingBuffer);

        VkMemoryRequirements stagingMemRequirements;
        vkGetBufferMemoryRequirements(logicalDev, stagingBuffer, &stagingMemRequirements);

        VkMemoryAllocateInfo stagingAllocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = stagingMemRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(memProperties, stagingMemRequirements.memoryTypeBits,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),

        };

        VkDeviceMemory stagingBufferMemory;
        vkAllocateMemory(logicalDev, &stagingAllocInfo, nullptr, &stagingBufferMemory);

        vkBindBufferMemory(logicalDev, stagingBuffer, stagingBufferMemory, 0);

        void* staginBufferData;

        staginBufferData = nullptr;

        vkMapMemory(logicalDev, stagingBufferMemory, 0, stagingBufferInfo.size, 0, &staginBufferData);
        memcpy(staginBufferData, data, (size_t)stagingBufferInfo.size);
        vkUnmapMemory(logicalDev, stagingBufferMemory);

        // Actual buffer.

        const VkBufferCreateInfo bufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = stagingBufferInfo.size,
            .usage       = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        vkCreateBuffer(logicalDev, &bufferInfo, nullptr, &m_Buffer);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDev, m_Buffer, &memRequirements);

        const VkMemoryAllocateInfo allocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(memProperties, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        };

        vkAllocateMemory(logicalDev, &allocInfo, nullptr, &m_Memory);

        vkBindBufferMemory(logicalDev, m_Buffer, m_Memory, 0);

        // Copy data from staging buffer to actual buffer.

        // #TODO : Should create every time I create buffer?
        const VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
            .queueFamilyIndex = vulkanDev.GetQFamilyIndex(),
        };

        VkCommandPool commandPool = VK_NULL_HANDLE;
        vkCreateCommandPool(logicalDev, &poolInfo, nullptr, &commandPool);

        const VkCommandBufferAllocateInfo commandAllocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicalDev, &commandAllocInfo, &commandBuffer);

        const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        const VkBufferCopy copyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = stagingBufferInfo.size,
        };

        vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        const VkSubmitInfo submitInfo{
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext              = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers    = &commandBuffer,
        };

        VkQueue graphicsQueue = vulkanDev.GetQ();

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(logicalDev, commandPool, 1, &commandBuffer);

        vkDestroyBuffer(logicalDev, stagingBuffer, nullptr);
        vkFreeMemory(logicalDev, stagingBufferMemory, nullptr);
    }

    IndexBuffer::~IndexBuffer()
    {
        VkDevice logicalDev = Renderer::GetContext().GetGpuDevice().GetDevice();

        vkDestroyBuffer(logicalDev, m_Buffer, nullptr);
        vkFreeMemory(logicalDev, m_Memory, nullptr);
    }

    VkBuffer IndexBuffer::GetBuffer() const { return m_Buffer; }

    uint32_t IndexBuffer::FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter,
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