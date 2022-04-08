#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    class VulkanVertexBuffer
    {
      public:
        //  Create vertex buffer using staging.
        VulkanVertexBuffer(const void* data, uint32_t size);
        ~VulkanVertexBuffer();

        VkBuffer GetBuffer() const;

      private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter,
                                VkMemoryPropertyFlags properties) const;

        VkBuffer buffer       = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };

} // namespace South