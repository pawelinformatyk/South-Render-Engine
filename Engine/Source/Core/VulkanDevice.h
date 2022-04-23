#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    // Class wrapper around vulkan devices : physical and logical.
    // Has also variables related to queues, because they are retrieved from devices.
    class VulkanDevice
    {
      public:
        VulkanDevice(){};
        ~VulkanDevice(){};

        void Init(VkSurfaceKHR surface);
        void DeInit();

        VkPhysicalDevice GetPhysicalDevice() const;
        VkDevice GetDevice() const;
        uint32_t GetQFamilyIndex() const;
        VkQueue GetQ() const;

      private:
        const std::vector<const char*> requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice          = VK_NULL_HANDLE;

        VkQueue Queue             = VK_NULL_HANDLE;
        uint32_t QueueFamilyIndex = 0;
    };


} // namespace South
