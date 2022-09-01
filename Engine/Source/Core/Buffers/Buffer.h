#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{
    // #TODO : Add allocator.
    class Buffer
    {
    public:
        static void Destroy(VkDevice InlogicalDev, Buffer* InBuffer);

        VkBuffer GetBuffer() const;
        VkDeviceMemory GetMemory() const;

        void SetData(const VkDevice InLogicalDev,
                     const VkDeviceSize& InOffset,
                     const VkDeviceSize& InSize,
                     const void* InData);

    protected:
        VkBuffer m_Buffer       = nullptr;
        VkDeviceMemory m_Memory = nullptr;
    };
} // namespace South