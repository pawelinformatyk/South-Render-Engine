#include "SceneRenderPass.h"

#include "Core/Renderer/RendererContext.h"

namespace South
{


SSceneRenderPass::SSceneRenderPass()
{
    constexpr VkAttachmentDescription ColorAttachment {
        .format         = VK_FORMAT_R8G8B8A8_SRGB,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_GENERAL,
    };

    constexpr VkAttachmentDescription DepthAttachment {
        .format         = VK_FORMAT_D32_SFLOAT,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    constexpr VkAttachmentReference ColorAttachmentRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    constexpr VkAttachmentReference DepthAttachmentRef {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    const VkSubpassDescription Subpass {
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount    = 1,
        .pColorAttachments       = &ColorAttachmentRef,
        .pDepthStencilAttachment = &DepthAttachmentRef,
    };

    constexpr VkSubpassDependency Dependency {
        .srcSubpass    = VK_SUBPASS_EXTERNAL,
        .dstSubpass    = 0,
        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };

    const std::array Attachments = {ColorAttachment, DepthAttachment};

    const VkRenderPassCreateInfo CreateInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(Attachments.size()),
        .pAttachments    = Attachments.data(),
        .subpassCount    = 1,
        .pSubpasses      = &Subpass,
        .dependencyCount = 1,
        .pDependencies   = &Dependency,
    };
    vkCreateRenderPass(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &CreateInfo, nullptr, &RenderPass);
}

SSceneRenderPass::~SSceneRenderPass()
{
    // #TODO: Uncomment when I figure out where to put renderer global object.
    // vkDestroyRenderPass(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), RenderPass, nullptr);
}

VkRenderPass SSceneRenderPass::Get() const
{
    return RenderPass;
}

} // namespace South