#pragma once

#include "Buffer.h"

namespace South
{
    // One buffer for vertex and indices data.
    // (vertices...indices)
    // #TODO : Check if actually is more performant than two separated buffers. Check
    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    class VertexIndexBuffer : Buffer
    {
    public:
        struct CreateInfo
        {
            const void* Data = nullptr;
            // Size of one element.
            uint32_t Size  = 0;
            uint32_t Count = 0;
        };

        static VertexIndexBuffer* Create(const CreateInfo& InVertexInfo, const CreateInfo& InIndexInfo);
        // #TODO : Should here be input Device.
        static void Destroy(VkDevice InlogicalDev, VertexIndexBuffer& InBuffer);

        VkBuffer GetVulkanBuffer() const;

        uint32_t GetVerticesSize() const;
        uint32_t GetIndicesSize() const;

        uint32_t GetVerticesCount() const;
        uint32_t GetIndicesCount() const;

    private:
        uint32_t m_VerticesSize = 0;
        uint32_t m_IndicesSize  = 0;

        uint32_t m_VerticesCount = 0;
        uint32_t m_IndicesCount  = 0;
    };
} // namespace South
