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
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
    };

} // namespace South