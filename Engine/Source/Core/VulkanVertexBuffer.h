#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    class VulkanVertexBuffer
    {
        // ????/??
        void Create();

        void Map(void* data, size_t size, size_t sizeOfElement);

        VkBuffer GetBuffer() const;


      private:
        VkBuffer buffer       = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };

} // namespace South