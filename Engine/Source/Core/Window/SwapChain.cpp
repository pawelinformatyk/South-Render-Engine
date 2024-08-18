#include "Core/Window/SwapChain.h"
#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Utils/VulkanUtils.h"

#include "GLFW/glfw3.h"

namespace South
{

SSwapChain* SSwapChain::Create(const SCreateInfo& InInfo)
{
    auto* OutSwapChain = new SSwapChain();

    OutSwapChain->CreateSwapchain(InInfo);

    return OutSwapChain;
}

void SSwapChain::CreateSwapchain(const SCreateInfo& InInfo)
{
    const SLogicalDeviceAndQueues& LogicalDevice  = SRendererContext::Get().GetDeviceAndQueues();
    const SGraphicCard&            Gpu            = SRendererContext::Get().GetGraphicCard();
    const VkPhysicalDevice        PhysicalDevice = Gpu.GetPhysicalDevice();

    m_PresentMode = InInfo.PresentMode;
    m_Size        = InInfo.Size;

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, InInfo.Surface, &SurfaceCapabilities);

    m_ImageFormat     = InInfo.SurfaceFormat.format;
    m_ImageColorSpace = InInfo.SurfaceFormat.colorSpace;

    uint32_t ImagesCount = SurfaceCapabilities.minImageCount + 1;
    if(SurfaceCapabilities.maxImageCount > 0 && ImagesCount > SurfaceCapabilities.maxImageCount)
    {
        ImagesCount = SurfaceCapabilities.maxImageCount;
    }

    std::array queueFamilyIndices = {LogicalDevice.GetGraphicQueueFamilyIndex(), LogicalDevice.GetPresentQueueFamilyIndex()};

    const VkSwapchainCreateInfoKHR VulkanSwapChainInfo {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = nullptr,
        .surface               = InInfo.Surface,
        .minImageCount         = ImagesCount,
        .imageFormat           = m_ImageFormat,
        .imageColorSpace       = m_ImageColorSpace,
        .imageExtent           = m_Size,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size()),
        .pQueueFamilyIndices   = queueFamilyIndices.data(),
        .preTransform          = SurfaceCapabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = m_PresentMode,
        .clipped               = true,
    };

    const VkDevice Device = LogicalDevice.GetLogicalDevice();

    vkCreateSwapchainKHR(Device, &VulkanSwapChainInfo, nullptr, &m_VulkanSwapChain);

    vkGetSwapchainImagesKHR(Device, m_VulkanSwapChain, &ImagesCount, nullptr);
    m_Images.resize(ImagesCount);
    vkGetSwapchainImagesKHR(Device, m_VulkanSwapChain, &ImagesCount, m_Images.data());

    m_ImagesViews.reserve(m_ImagesViews.size());
    for(const VkImage& Image : m_Images)
    {
        m_ImagesViews.emplace_back(VulkanUtils::CreateImageView(Device, Image, m_ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT));
    }

    m_Framebuffers.resize(m_ImagesViews.size());

    for(size_t i = 0; i < m_ImagesViews.size(); i++)
    {
        const VkFramebufferCreateInfo FramebufferInfo {
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .renderPass      = SRendererContext::Get().GetRenderPass().GetVulkanPass(),
            .attachmentCount = 1,
            .pAttachments    = &m_ImagesViews[i],
            .width           = m_Size.width,
            .height          = m_Size.height,
            .layers          = 1,
        };
        vkCreateFramebuffer(Device, &FramebufferInfo, nullptr, &m_Framebuffers[i]);
    }
}

void SSwapChain::Destroy(SSwapChain& InSwapChain)
{
    const VkDevice Device = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();

    for(const VkImageView& ImageView : InSwapChain.m_ImagesViews)
    {
        vkDestroyImageView(Device, ImageView, nullptr);
    }
    InSwapChain.m_ImagesViews.clear();

    for(const VkFramebuffer& Buffer : InSwapChain.m_Framebuffers)
    {
        vkDestroyFramebuffer(Device, Buffer, nullptr);
    }
    InSwapChain.m_Framebuffers.clear();

    vkDestroySwapchainKHR(Device, InSwapChain.m_VulkanSwapChain, nullptr);

    InSwapChain.m_VulkanSwapChain = nullptr;
}

void SSwapChain::RecreateSwapChain(const uint32_t InWidth, const uint32_t InHeight)
{
    // #TODO: Change without waiting
    vkDeviceWaitIdle(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice());

    Destroy(*this);

    CreateSwapchain(SCreateInfo {
        .Surface       = SRendererContext::Get().GetSurface(),
        .SurfaceFormat = {m_ImageFormat, m_ImageColorSpace},
        .Size          = {InWidth, InHeight},
        .PresentMode   = m_PresentMode,
    });
}


VkSwapchainKHR SSwapChain::GetVulkanSwapChain() const
{
    return m_VulkanSwapChain;
}

VkFramebuffer SSwapChain::GetFramebuffer(const uint32_t InIndex) const
{
    return m_Framebuffers[InIndex];
}

uint32_t SSwapChain::GetFramebuffersCount() const
{
    return static_cast<uint32_t>(m_Framebuffers.size());
}

VkExtent2D SSwapChain::GetSize() const
{
    return m_Size;
}

} // namespace South