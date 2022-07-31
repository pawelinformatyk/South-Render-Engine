#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // #TODO #PERF : Combine with VertexBuffer for being more cache friendly.
    class IndexBuffer
    {
    public:
        IndexBuffer(const void* data, uint32_t size);
        ~IndexBuffer();

        VkBuffer GetBuffer() const;

    private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter,
                                VkMemoryPropertyFlags properties) const;

        VkBuffer m_Buffer       = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
    };

} // namespace South