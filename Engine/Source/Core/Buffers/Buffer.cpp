#include "Buffer.h"

void South::Buffer::Destroy(const VkDevice InlogicalDev, Buffer* InBuffer)
{
    if(!InBuffer)
    {
        return;
    }

    vkDestroyBuffer(InlogicalDev, InBuffer->m_Buffer, nullptr);
    vkFreeMemory(InlogicalDev, InBuffer->m_Memory, nullptr);

    delete InBuffer;
}

VkBuffer South::Buffer::GetBuffer() const
{
    return m_Buffer;
}

VkDeviceMemory South::Buffer::GetMemory() const
{
    return m_Memory;
}
