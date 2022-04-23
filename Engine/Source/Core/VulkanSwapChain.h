#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    class VulkanSwapChain
    {
      public:
        VulkanSwapChain();
        ~VulkanSwapChain();

        void Init();
        void DeInit();

      private:
        VkSwapchainKHR SwapChain = VK_NULL_HANDLE;
        std::vector<VkImage> SwapChainImages;
        VkFormat SwapChainImageFormat;
        VkExtent2D SwapChainExtent;
        std::vector<VkImageView> SwapChainImageViews;
        std::vector<VkFramebuffer> SwapChainFramebuffers;
    };

} // namespace South