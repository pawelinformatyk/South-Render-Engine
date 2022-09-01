#include "sthpch.h"

#include "Buffer.h"

void South::Buffer::Destroy(const VkDevice InlogicalDev, Buffer* InBuffer)
{
    if (!InBuffer) { return; }

    vkDestroyBuffer(InlogicalDev, InBuffer->m_Buffer, nullptr);
    vkFreeMemory(InlogicalDev, InBuffer->m_Memory, nullptr);

    delete InBuffer;
    InBuffer = nullptr;
}

VkBuffer South::Buffer::GetBuffer() const
{
    return m_Buffer;
}

VkDeviceMemory South::Buffer::GetMemory() const
{
    return m_Memory;
}

void South::Buffer::SetData(const VkDevice InLogicalDev,
                            const VkDeviceSize& InOffset,
                            const VkDeviceSize& InSize,
                            const void* InData)
{
    void* BufferData = nullptr;

    vkMapMemory(InLogicalDev, m_Memory, InOffset, InSize, 0, &BufferData);
    memcpy(BufferData, InData, static_cast<size_t>(InSize));
    vkUnmapMemory(InLogicalDev, m_Memory);
}