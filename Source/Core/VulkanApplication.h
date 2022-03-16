#pragma once

#include "vulkan/vulkan_core.h"

#include <vector>

namespace South
{
    class AppWindow;

    // Input, window resize,close,open, events,
    // run loop, what decides what keep app going
    class VulkanApplication
    {
      public:
        VulkanApplication(int argc, char** argv);
        ~VulkanApplication();

        void Run();

      private:
        // #TODO : Change to vulkan/opengl context
        // Vulkan
        void CreateInstance();
        void CreateSurface();
        void CreateDevices();

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        VkInstance instance         = VK_NULL_HANDLE;
        VkSurfaceKHR surface        = VK_NULL_HANDLE;
        VkPhysicalDevice physDevice = VK_NULL_HANDLE;
        uint32_t QueueFamilyIndex   = 0;
        VkDevice logicDevice        = VK_NULL_HANDLE;
        VkQueue graphicsQueue       = VK_NULL_HANDLE;

        // ~Vulkan

        AppWindow* pWindow = nullptr;
        bool bRunning      = false;
    };

    // Renderer -> move vulkan functions to this. I mean like instance etc.


    // I want to have multiple viewports. How do I do it. Do I need layers or just
    // two imgui windows with rendered scenes with different camera.

    // Multiple viewports -> multiple textures rendered and shown from different VIEW matrix.
    // I thjink I need to implement vulcan code so i can then decide how to do it.
    // What is framebuffer exactly?

    // Editor layer from cherno -> showcase layer
} // namespace South