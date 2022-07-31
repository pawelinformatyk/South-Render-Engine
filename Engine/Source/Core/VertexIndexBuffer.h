#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // One buffer for vertex and indices data.
    // (vertices...indices)
    // #TODO : Check if actually is more performant than two separated buffers. Check
    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    class VertexIndexBuffer
    {
    public:
        VertexIndexBuffer(const void* vData, uint32_t vSize, const void* iData, uint32_t iSize);
        ~VertexIndexBuffer();

        VkBuffer GetVulkanBuffer() const;
        uint32_t GetIndexOffset() const;

    private:
        uint32_t FindMemoryType(VkPhysicalDeviceMemoryProperties MemProperties, uint32_t typeFilter,
                                VkMemoryPropertyFlags Properties) const;

        uint32_t m_IndexOffset = 0;

        VkBuffer m_Buffer       = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
    };

} // namespace South
