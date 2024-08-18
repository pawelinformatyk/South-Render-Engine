#pragma once

#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{
class SRendererContext;

class SLogicalDeviceAndQueues;

// #TODO: Swapchain frambuffer
class SFramebuffer
{
    struct SCreateInfo
    {
        VkDevice         Device;
        VkExtent2D       Size;
        VkPresentModeKHR PresentMode;
    };

    explicit SFramebuffer(const SCreateInfo& InInfo);
    ~        SFramebuffer();

    VkImage       m_VulkanImage       = nullptr;
    VkImageView   m_VulkanImageView   = nullptr;
    VkFramebuffer m_VulkanFramebuffer = nullptr;
};

class SSwapChain
{
public:
    struct SCreateInfo
    {
        VkSurfaceKHR       Surface;
        VkSurfaceFormatKHR SurfaceFormat;
        VkExtent2D         Size;
        VkPresentModeKHR   PresentMode;
    };

    static SSwapChain* Create(const SCreateInfo& InInfo);
    static void        Destroy(SSwapChain& InSwapChain);

    void RecreateSwapChain(uint32_t InWidth, uint32_t InHeight);

    VkSwapchainKHR GetVulkanSwapChain() const;
    VkFramebuffer  GetFramebuffer(uint32_t InIndex) const;
    uint32_t       GetFramebuffersCount() const;
    VkExtent2D     GetSize() const;

private:
    void CreateSwapchain(const SCreateInfo& InInfo);

    VkSwapchainKHR m_VulkanSwapChain = nullptr;

    // #TODO: Should be in one struct.
    std::vector<VkImage>       m_Images;
    std::vector<VkImageView>   m_ImagesViews;
    std::vector<VkFramebuffer> m_Framebuffers;

    VkFormat        m_ImageFormat     = VK_FORMAT_R8G8B8_UNORM;
    VkColorSpaceKHR m_ImageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    VkExtent2D       m_Size        = {1280, 720};
    VkPresentModeKHR m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
};

} // namespace South