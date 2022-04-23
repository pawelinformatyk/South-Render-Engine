#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // #TODO #PERF : Combine with VertexBuffer for being more cache friendly.
    class VulkanVertexBuffer
    {
      public:
        VulkanVertexBuffer(const void* Data, uint32_t Size);
        ~VulkanVertexBuffer();

        VkBuffer GetBuffer() const;

      private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties MemProperties, uint32_t TypeFilter,
                                VkMemoryPropertyFlags Properties) const;

        VkBuffer Buffer       = VK_NULL_HANDLE;
        VkDeviceMemory Memory = VK_NULL_HANDLE;
    };

} // namespace South