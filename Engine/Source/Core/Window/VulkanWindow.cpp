#include "sthpch.h"

#include "Core/Window/VulkanWindow.h"

#include "Core/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace South
{

    void VulkanWindow::Init()
    {
        Window::Init();

        if (!glfwWindow) { return; }

        VkInstance VulkanInstance = VulkanContext::Get().GetVulkanInstance();

        // Initialize surface.
        if (!glfwCreateWindowSurface(VulkanInstance, glfwWindow, nullptr, &Surface))
        {
            STH_ERR("Surface couldnt be created");
            return;
        }

        SwapChain.Init(*glfwWindow, Surface);
    }

    void VulkanWindow::DeInit()
    {
        Window::DeInit();

        VkInstance VulkanInstance = VulkanContext::Get().GetVulkanInstance();

        SwapChain.DeInit();
        vkDestroySurfaceKHR(VulkanInstance, Surface, nullptr);
    }

} // namespace South