#pragma once

#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

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
        LogicalDeviceAndQueues& LogicalDevice;
        VkPhysicalDevice        PhysDevice;
        VkSurfaceKHR            Surface;
        VkExtent2D              Size;
        VkPresentModeKHR        PresentMode;
    };

    explicit SwapChain(const CreateInfo& InInfo);
    ~SwapChain();

    VkSwapchainKHR GetVulkanSwapChain() const;
    VkRenderPass   GetRenderPass() const;
    VkFramebuffer  GetFramebuffer(uint32_t InIndex) const;

private:
    VkSwapchainKHR m_VulkanSwapChain = nullptr;

    // #TODO: Should be in one struct.
    std::vector<VkImage>       m_Images;
    std::vector<VkImageView>   m_ImageViews;
    std::vector<VkFramebuffer> m_Framebuffers;

    VkRenderPass m_RenderPass = nullptr;

    VkFormat        m_ImageFormat;
    VkColorSpaceKHR m_ImageColorSpace;

    VkExtent2D       m_Size        = {1280, 720};
    VkPresentModeKHR m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
};

} // namespace South