#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{
    class Buffer
    {
    protected:
        static void MapMemory(const VkDevice InLogicalDev,
                              const VkDeviceMemory InBufferMemory,
                              const VkDeviceSize& InOffset,
                              const VkDeviceSize& InSize,
                              const void* InData);

        static void CreateBufferAndMemory(const VkDevice InLogicalDev,
                                          VkBuffer& OutBuffer,
                                          VkDeviceMemory& OutBufferMemory,
                                          const VkDeviceSize& InSize,
                                          VkBufferUsageFlags InUsage,
                                          VkSharingMode InSharingMode,
                                          const VkPhysicalDeviceMemoryProperties InMemoryProperties,
                                          int InMemoryFlags);

        static uint32_t FindMemoryType(const VkPhysicalDeviceMemoryProperties InMemProperties,
                                       const uint32_t InTypeFilter,
                                       const VkMemoryPropertyFlags InProperties);

        VkBuffer m_Buffer       = nullptr;
        VkDeviceMemory m_Memory = nullptr;
    };
} // namespace South