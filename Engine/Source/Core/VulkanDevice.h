#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{
    class VulkanDevice
    {
      public:
        ~VulkanDevice();

        static VulkanDevice* Create();

        void Init(const VkInstance& instance, const VkSurfaceKHR& surface);

        // #TODO : are const reference make sense?
        const VkPhysicalDevice& GetPhysicalDevice() const;
        const VkDevice& GetDevice() const;
        const uint32_t& GetQFamilyIndex() const;
        const VkQueue& GetQ() const;

      private:
        const std::vector<const char*> requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice          = VK_NULL_HANDLE;

        VkQueue graphicsQueue     = VK_NULL_HANDLE;
        uint32_t QueueFamilyIndex = 0;
    };


} // namespace South
