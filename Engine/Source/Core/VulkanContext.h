#pragma once

#include "Core/Context.h"
#include "vulkan/vulkan_core.h"
#include "glm.hpp"

namespace South
{
    class VulkanDevice;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static const VkVertexInputBindingDescription& GetBindingDescription();
        static const std::array<VkVertexInputAttributeDescription, 2>& GetAttributesDescriptions();

      private:
        static VkVertexInputBindingDescription bindingDesc;
        static std::array<VkVertexInputAttributeDescription, 2> attributesDescs;
    };

    class VulkanContext : public Context
    {
      public:
        virtual void Init(GLFWwindow& window) override;
        virtual void DeInit() override;
        virtual void Tick() override;

        static VkInstance GetInstance();

      private:
        void CreateInstance();
        void CreateSurface(GLFWwindow& window);
        void CreateSwapChain(GLFWwindow& window);
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateVertexBuffers();
        void CreateCommands();
        void CreateSyncObjects();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkPresentModeKHR ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

        void RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

        static inline VkInstance instance = VK_NULL_HANDLE;

        VkSurfaceKHR surface     = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkBuffer vertexBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

        VkRenderPass renderPass         = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline     = VK_NULL_HANDLE;

        VkCommandPool commandPool           = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer       = VK_NULL_HANDLE;
        VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence inFlightFence               = VK_NULL_HANDLE;

        VulkanDevice* device;
    };
}; // namespace South