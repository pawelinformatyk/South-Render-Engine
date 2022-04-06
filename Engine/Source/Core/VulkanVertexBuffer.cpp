#include "sthpch.h"

#include "Core/VulkanVertexBuffer.h"

namespace South
{

    VkBuffer VulkanVertexBuffer::GetBuffer() const
    {
        return buffer;
    }

} // namespace South