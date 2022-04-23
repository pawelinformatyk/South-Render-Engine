#pragma once

#include "Core/Window/Window.h"

#include "vulkan/vulkan_core.h"

namespace South
{

    class VulkanWindow : Window
    {
      public:
        virtual void Init() override;
        virtual void DeInit() override;

      private:
        VkSurfaceKHR surface     = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
    };

} // namespace South
