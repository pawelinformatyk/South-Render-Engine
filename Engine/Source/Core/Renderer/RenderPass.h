#pragma once

#include <vulkan/vulkan_core.h>

namespace South
{

class SRenderPass
{
public:
    struct SCreateInfo
    {
        VkFormat Format;
    };

    static SRenderPass* Create(const SCreateInfo& InInfo);
    static void         Destroy(SRenderPass& InRenderPass);

    VkRenderPass GetVulkanPass() const;

private:
    VkRenderPass m_VulkanRenderPass = nullptr;
};

} // namespace South