#include "sthpch.h"

#include "Core/Window/VulkanWindow.h"

#include "Core/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace South
{

    void VulkanWindow::Init()
    {
        Window::Init();

        if (!m_glfwWindow) { return; }

        VkInstance VulkanInstance = VulkanContext::Get().GetVulkanInstance();

        // Initialize vulkan surface.
        if (!glfwCreateWindowSurface(VulkanInstance, m_glfwWindow, nullptr, &m_Surface))
        {
            STH_ERR("Surface couldnt be created");
            return;
        }

        m_SwapChain.Init(*m_glfwWindow, m_Surface);
    }

    void VulkanWindow::DeInit()
    {
        Window::DeInit();

        VkInstance VulkanInstance = VulkanContext::Get().GetVulkanInstance();

        m_SwapChain.DeInit();
        vkDestroySurfaceKHR(VulkanInstance, m_Surface, nullptr);
    }

} // namespace South