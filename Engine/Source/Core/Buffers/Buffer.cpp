#include "Buffer.h"
#include <Core/Renderer/RendererContext.h>

namespace South
{


void Buffer::Destroy(const VkDevice InlogicalDev, Buffer* InBuffer)
{
    if(!InBuffer)
    {
        return;
    }

    vkDestroyBuffer(InlogicalDev, InBuffer->m_Buffer, nullptr);
    vkFreeMemory(InlogicalDev, InBuffer->m_Memory, nullptr);

    delete InBuffer;
}

VkBuffer Buffer::GetBuffer() const
{
    return m_Buffer;
}

VkDeviceMemory Buffer::GetMemory() const
{
    return m_Memory;
}

Buffer::~Buffer()
{
    VkDevice LogicalDevice = RendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    vkDestroyBuffer(LogicalDevice, m_Buffer, nullptr);
    vkFreeMemory(LogicalDevice, m_Memory, nullptr);
}

} // namespace South
