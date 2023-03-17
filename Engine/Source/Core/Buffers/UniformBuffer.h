#pragma once

#include "Buffer.h"
#include "mat4x4.hpp"

namespace South
{

struct UniformBufferObject
{
    alignas(16) glm::mat4 m_Model = glm::mat4(1.f);
    alignas(16) glm::mat4 m_View  = glm::mat4(1.f);
    alignas(16) glm::mat4 m_Proj  = glm::mat4(1.f);
};

// #TODO: Only for UniformBufferObject - for now ok. Think if I have more examples.
class UniformBuffer : public Buffer
{
public:
    static UniformBuffer* Create(VkDevice InLogicalDev);

    void SetData(VkDevice InLogicalDev, const void* InData);
};

} // namespace South
