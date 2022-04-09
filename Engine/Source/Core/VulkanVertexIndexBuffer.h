#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // One buffer for vertex and indices data.
    // (vertices...indices)
    // #TODO : Check if actually is more performant than two separated buffers. Check
    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    class VulkanVertexIndexBuffer
    {
      public:
        VulkanVertexIndexBuffer(const void* vData, uint32_t vSize, const void* iData, uint32_t iSize);
        ~VulkanVertexIndexBuffer();

        VkBuffer GetVulkanBuffer() const;
        uint32_t GetIndexOffset() const;

      private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter,
                                VkMemoryPropertyFlags properties) const;

        uint32_t indexOffset = 0;

        VkBuffer buffer       = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };

} // namespace South
