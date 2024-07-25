#pragma once

#include <vulkan/vulkan_core.h>

namespace South
{

class RenderPass
{
public:
    struct CreateInfo
    {
        VkFormat Format;
    };

    static RenderPass* Create(const CreateInfo& InInfo);
    static void        Destroy(RenderPass& InRenderPass);

    VkRenderPass GetVulkanPass() const;

private:
    VkRenderPass m_VulkanRenderPass = nullptr;
};

}