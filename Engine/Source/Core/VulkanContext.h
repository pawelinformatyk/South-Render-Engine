#pragma once

#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{
    class VulkanDevice;
    class VulkanVertexIndexBuffer;

    // Class holding all "global" vulkan related variables.
    class VulkanContext
    {
      public:
        VulkanContext(VulkanContext const&) = delete;
        void operator=(VulkanContext const&) = delete;

        void Init();
        void DeInit();
        void Tick();

        VkInstance GetVulkanInstance();
        VulkanDevice& GetCurrentDevice();

      private:
        VulkanContext(){};
        ~VulkanContext(){};

        void CreateInstance();
        void CreateSurface(GLFWwindow& window);
        void CreateSwapChain(GLFWwindow& window);
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();

        void CreateFramebuffers();
        void CreateModelBuffers();
        void CreateCommands();
        void CreateSyncObjects();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkPresentModeKHR ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

        void RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

        VkInstance vulkanInstance = VK_NULL_HANDLE;

        VkSurfaceKHR surface     = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass         = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline     = VK_NULL_HANDLE;

        VkCommandPool commandPool           = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer       = VK_NULL_HANDLE;
        VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence inFlightFence               = VK_NULL_HANDLE;

        VulkanDevice* device               = nullptr;
        VulkanVertexIndexBuffer* VI_Buffer = nullptr;

        bool bCanTick = false;

        // Static functions
      public:
        static VulkanContext& Get();
    };
}; // namespace South