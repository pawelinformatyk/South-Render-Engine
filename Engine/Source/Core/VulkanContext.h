#pragma once

#include "Core/Context.h"
#include "vulkan/vulkan_core.h"

namespace South
{
    class VulkanDevice;
    class VulkanVertexIndexBuffer;

    class VulkanContext : public Context
    {
      public:
        virtual void Init() override;
        virtual void DeInit() override;
        virtual void Tick() override;

        VkInstance GetVulkanInstance();
        VulkanDevice& GetCurrentDevice();

        static VulkanContext& Get();

      private:
        static inline VulkanContext* instance   = nullptr;
        static inline VkInstance vulkanInstance = VK_NULL_HANDLE;

      private:
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
    };
}; // namespace South