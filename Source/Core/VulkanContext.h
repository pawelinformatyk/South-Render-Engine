#pragma once

#include "Core/Context.h"
#include "vulkan/vulkan_core.h"

#include <vector>

namespace South
{

    class VulkanContext : public Context
    {
      public:
        virtual void Init(GLFWwindow& window) override;
        virtual void DeInit() override;

      private:
        void CreateInstance();
        void CreateSurface(GLFWwindow& window);
        void CreateDevices();
        void CreateSwapChain(GLFWwindow& window);
        void CreateImageViews();
        void CreateGraphicsPipeline();

        const std::vector<const char*> requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        VkInstance instance         = VK_NULL_HANDLE;
        VkSurfaceKHR surface        = VK_NULL_HANDLE;
        uint32_t QueueFamilyIndex   = 0;
        VkPhysicalDevice physDevice = VK_NULL_HANDLE;
        VkDevice logicDevice        = VK_NULL_HANDLE;
        VkQueue graphicsQueue       = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain    = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
    };
}; // namespace South