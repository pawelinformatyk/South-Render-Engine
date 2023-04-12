#pragma once

#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{
class RendererContext;

class LogicalDeviceAndQueues;

// #TODO: Swapchain frambuffer
class Framebuffer
{
    struct CreateInfo
    {
        VkDevice         Device;
        VkExtent2D       Size;
        VkPresentModeKHR PresentMode;
    };

    explicit Framebuffer(const CreateInfo& InInfo);
    ~Framebuffer();

    VkImage       m_VulkanImage       = nullptr;
    VkImageView   m_VulkanImageView   = nullptr;
    VkFramebuffer m_VulkanFramebuffer = nullptr;
};

class SwapChain
{
public:
    struct CreateInfo
    {
        VkSurfaceKHR       Surface;
        VkSurfaceFormatKHR SurfaceFormat;
        VkExtent2D         Size;
        VkPresentModeKHR   PresentMode;
    };

    static SwapChain* Create(const CreateInfo& InInfo);
    static void       Destroy(SwapChain& InSwapChain);

    void RecreateSwapChain(uint32_t InWidth, uint32_t InHeight);

    VkSwapchainKHR GetVulkanSwapChain() const;
    VkFramebuffer  GetFramebuffer(uint32_t InIndex) const;
    uint32_t       GetFramebuffersCount() const;

private:
    void CreateSwapchain(const CreateInfo& InInfo);

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