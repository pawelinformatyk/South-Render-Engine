#pragma once

#include "Buffer.h"

namespace South
{
    class UniformBuffer : Buffer
    {
    public:
        struct CreateInfo
        {
            const void* Data = nullptr;
            uint32_t Size    = 0;
        };

        static UniformBuffer* Create(const CreateInfo& InCreateInfo);
        static void Destroy(VkDevice InlogicalDev, UniformBuffer& InBuffer);
    };
} // namespace South
