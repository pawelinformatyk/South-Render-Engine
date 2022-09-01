#pragma once

#include "Buffer.h"

namespace South
{
    class UniformBuffer : public Buffer
    {
    public:
        struct CreateInfo
        {
            const void* Data = nullptr;
            uint32_t Size    = 0;
        };

        static UniformBuffer* Create(const VkDevice InLogicalDev, const CreateInfo& InCreateInfo);
    };
} // namespace South
