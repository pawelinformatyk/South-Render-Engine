#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // #TODO #PERF : Combine with VertexBuffer for being more cache friendly.
    class VulkanIndexBuffer
    {
    public:
        VulkanIndexBuffer(const void* data, uint32_t size);
        ~VulkanIndexBuffer();

        VkBuffer GetBuffer() const;

    private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter,
                                VkMemoryPropertyFlags properties) const;

        VkBuffer buffer       = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };

} // namespace South