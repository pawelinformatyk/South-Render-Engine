#include "Buffer.h"
#include <Core/Renderer/RendererContext.h>

namespace South
{


void SBuffer::Destroy(const VkDevice InlogicalDev, SBuffer* InBuffer)
{
    if(!InBuffer)
    {
        return;
    }

    vkDestroyBuffer(InlogicalDev, InBuffer->m_Buffer, nullptr);
    vkFreeMemory(InlogicalDev, InBuffer->m_Memory, nullptr);

    delete InBuffer;
}

VkBuffer SBuffer::GetBuffer() const
{
    return m_Buffer;
}

VkDeviceMemory SBuffer::GetMemory() const
{
    return m_Memory;
}

SBuffer::~SBuffer()
{
    VkDevice LogicalDevice = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    vkDestroyBuffer(LogicalDevice, m_Buffer, nullptr);
    vkFreeMemory(LogicalDevice, m_Memory, nullptr);
}

} // namespace South
