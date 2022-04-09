#include "sthpch.h"

#include "Core/VulkanVertexIndexBuffer.h"
#include "Core/VulkanContext.h"
#include "Core/VulkanDevice.h"

namespace South
{

    VulkanVertexIndexBuffer::VulkanVertexIndexBuffer(const void* vData, uint32_t vSize, const void* iData,
                                                     uint32_t iSize)
    {
        const VulkanDevice& vulkanDev = VulkanContext::Get().GetCurrentDevice();
        VkDevice logicalDev           = vulkanDev.GetDevice();
        VkPhysicalDevice physDev      = vulkanDev.GetPhysicalDevice();

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physDev, &memProperties);

        indexOffset = vSize;

        VkBufferCreateInfo stagingBufferInfo{
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext       = nullptr,
            .size        = vSize + iSize,
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

        vkMapMemory(logicalDev, stagingBufferMemory, 0, vSize, 0, &staginBufferData);
        memcpy(staginBufferData, vData, static_cast<size_t>(vSize));
        vkUnmapMemory(logicalDev, stagingBufferMemory);

        staginBufferData = nullptr;

        vkMapMemory(logicalDev, stagingBufferMemory, indexOffset, iSize, 0, &staginBufferData);
        memcpy(staginBufferData, iData, static_cast<size_t>(iSize));
        vkUnmapMemory(logicalDev, stagingBufferMemory);

        // Actual buffer.

        VkBufferCreateInfo bufferInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size  = stagingBufferInfo.size,
            .usage =
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        vkCreateBuffer(logicalDev, &bufferInfo, nullptr, &buffer);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDev, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{
            .sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext          = nullptr,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex =
                FindMemoryType(memProperties, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        };

        vkAllocateMemory(logicalDev, &allocInfo, nullptr, &memory);

        vkBindBufferMemory(logicalDev, buffer, memory, 0);

        // Copy data from staging buffer to actual buffer.

        // #TODO : Should create every time I create buffer?
        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
            .queueFamilyIndex = vulkanDev.GetQFamilyIndex(),
        };

        VkCommandPool commandPool = VK_NULL_HANDLE;
        vkCreateCommandPool(logicalDev, &poolInfo, nullptr, &commandPool);

        VkCommandBufferAllocateInfo commandAllocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicalDev, &commandAllocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        };

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size      = stagingBufferInfo.size,
        };

        vkCmdCopyBuffer(commandBuffer, stagingBuffer, buffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{
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

    VulkanVertexIndexBuffer::~VulkanVertexIndexBuffer()
    {
        VkDevice logicalDev = VulkanContext::Get().GetCurrentDevice().GetDevice();

        vkDestroyBuffer(logicalDev, buffer, nullptr);
        vkFreeMemory(logicalDev, memory, nullptr);
    }

    VkBuffer VulkanVertexIndexBuffer::GetVulkanBuffer() const
    {
        return buffer;
    }

    uint32_t VulkanVertexIndexBuffer::GetIndexOffset() const
    {
        return indexOffset;
    }

    uint32_t VulkanVertexIndexBuffer::FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties,
                                                     uint32_t typeFilter, VkMemoryPropertyFlags properties) const
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
