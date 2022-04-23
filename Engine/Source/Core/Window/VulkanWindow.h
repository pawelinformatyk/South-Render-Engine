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
        VkSurfaceKHR Surface = VK_NULL_HANDLE;

        std::unique_ptr<VulkanSwapChain> SwapChain = nullptr;
    };

} // namespace South
