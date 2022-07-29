#pragma once

#include "Core/Window/Window.h"

#include "Core/VulkanSwapChain.h"
#include "vulkan/vulkan_core.h"

namespace South
{

    // Shouldbe there exists sth as vulkan window - code should be moved to renderer or smt.
    class VulkanWindow : Window
    {
    public:
        virtual void Init() override;
        virtual void DeInit() override;

    private:
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VulkanSwapChain m_SwapChain;
    };

} // namespace South
