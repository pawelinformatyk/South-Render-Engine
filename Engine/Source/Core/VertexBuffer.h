#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // #TODO #PERF : Combine with VertexBuffer for being more cache friendly.
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* Data, uint32_t Size);
        ~VertexBuffer();

        VkBuffer GetBuffer() const;

    private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties MemProperties, uint32_t TypeFilter,
                                VkMemoryPropertyFlags Properties) const;

        VkBuffer m_Buffer       = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
    };

} // namespace South