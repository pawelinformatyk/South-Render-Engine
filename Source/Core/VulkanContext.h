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

        const std::vector<const char*> requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        VkInstance instance         = VK_NULL_HANDLE;
        VkSurfaceKHR surface        = VK_NULL_HANDLE;
        VkPhysicalDevice physDevice = VK_NULL_HANDLE;
        uint32_t QueueFamilyIndex   = 0;
        VkDevice logicDevice        = VK_NULL_HANDLE;
        VkQueue graphicsQueue       = VK_NULL_HANDLE;
    };
}; // namespace South