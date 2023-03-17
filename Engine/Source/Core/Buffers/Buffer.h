#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{
// #TODO : Add custom allocator.
class Buffer
{
public:
    static void Destroy(VkDevice InlogicalDev, Buffer* InBuffer);

    VkBuffer       GetBuffer() const;
    VkDeviceMemory GetMemory() const;

protected:
    VkBuffer       m_Buffer = nullptr;
    VkDeviceMemory m_Memory = nullptr;
};
} // namespace South