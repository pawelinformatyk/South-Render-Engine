#pragma once

#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain();
        ~VulkanSwapChain();

        void Init(GLFWwindow& Window, VkSurfaceKHR InSurface);
        void DeInit();

    private:
        void CreateImages(VkDevice LogicDevice);
        void CreateSwapChain(VkPhysicalDevice PhysDevice, VkDevice LogicDevice, uint32_t QueueFamilyIndex);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkPresentModeKHR ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_SwapChainImages;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
    };

} // namespace South