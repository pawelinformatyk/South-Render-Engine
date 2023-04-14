#include "sthpch.h"

#include "RendererContext.h"

#include <vulkan/vulkan_core.h>

namespace South
{

RenderPass* RenderPass::Create(const CreateInfo& InInfo)
{
    auto* OutRenderPass = new RenderPass;

    const VkAttachmentDescription ColorAttachment {
        .format         = InInfo.Format,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    const VkAttachmentReference ColorAttachmentRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    const VkSubpassDescription Subpass {
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount    = 1,
        .pColorAttachments       = &ColorAttachmentRef,
        .pDepthStencilAttachment = nullptr,
    };

    const VkSubpassDependency Dependency {
        .srcSubpass    = VK_SUBPASS_EXTERNAL,
        .dstSubpass    = 0,
        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    const std::array Attachments = {ColorAttachment};

    const VkRenderPassCreateInfo RenderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(Attachments.size()),
        .pAttachments    = Attachments.data(),
        .subpassCount    = 1,
        .pSubpasses      = &Subpass,
        .dependencyCount = 1,
        .pDependencies   = &Dependency,
    };
    vkCreateRenderPass(RendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                       &RenderPassInfo,
                       nullptr,
                       &OutRenderPass->m_VulkanRenderPass);

    return OutRenderPass;
}

void RenderPass::Destroy(RenderPass& InRenderPass)
{
    vkDestroyRenderPass(RendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), InRenderPass.m_VulkanRenderPass, nullptr);
    InRenderPass.m_VulkanRenderPass = nullptr;
}

VkRenderPass RenderPass::GetVulkanPass() const
{
    return m_VulkanRenderPass;
}

}