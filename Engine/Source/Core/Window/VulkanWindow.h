#pragma once

#include "Core/Window/Window.h"

#include "Core/VulkanSwapChain.h"
#include "vulkan/vulkan_core.h"

namespace South
{

    class VulkanWindow : Window
    {
      public:
        virtual void Init() override;
        virtual void DeInit() override;

      private:
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        std::unique_ptr<VulkanSwapChain> swapChain = nullptr;
    };

} // namespace South
