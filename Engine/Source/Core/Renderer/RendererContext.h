#pragma once

#include "glm.hpp"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{
    class GraphicCard;
    class Queue;

    // Model/projection are not changing every frame - should be in uniform (desriptor buffer)
    // Projection too.
    struct PushConstant
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    };

    // Class holding all "global" vulkan related variables.
    class RendererContext
    {
        friend class Renderer;

    public:
        virtual void Init();
        virtual void DeInit();

        VkInstance GetVulkanInstance() const;

        Queue& GetGraphicQueue() const;
        VkDevice GetLogicalDevice() const;
        GraphicCard& GetGraphicCard() const;

        VkRenderPass GetRenderPass() const;
        VkCommandBuffer GetCommandBuffer() const;
        VkCommandPool GetCommandPool() const;
        VkDescriptorPool GetDescriptorPool() const;

    private:
        void CreateInstance();

        void CreateSurface(GLFWwindow& Window);
        void CreateSwapChain(GLFWwindow& Window);
        void CreateImageViews();

        void CreateRenderPass();
        void CreateGraphicsPipeline();

        void CreateFramebuffers();
        void CreateModelBuffers();
        void CreateCommands();
        void CreateSyncObjects();

        void CreateDescriptorPool();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkPresentModeKHR ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

        std::vector<const char*> GetRequiredInstanceExtensions();

        VkInstance m_VulkanInstance = VK_NULL_HANDLE;

        VkSurfaceKHR m_Surface     = VK_NULL_HANDLE;
        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_SwapChainImages;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;

        VkRenderPass m_RenderPass         = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_GraphicsPipeline     = VK_NULL_HANDLE;

        VkDescriptorPool m_DescriptorPool;

        VkCommandPool m_CommandPool           = VK_NULL_HANDLE;
        VkCommandBuffer m_CommandBuffer       = VK_NULL_HANDLE;
        VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_FlightFence                 = VK_NULL_HANDLE;

        GraphicCard* m_GraphicCard = nullptr;
        VkDevice m_Device          = nullptr;
        Queue* m_GraphicQueue      = nullptr;

        //~ Validations layers.
    private:
        void CreateMessenger();
        void DestroyMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL
            ValidationMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                      void* pUserData);

        bool CheckValidationLayers();

        const std::vector<const char*> m_RequiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
        const bool m_bEnableValidationLayers = false;
#else
        const bool m_bEnableValidationLayers = true;
#endif

        VkDebugUtilsMessengerEXT m_Messenger;

        //~ Validations layers.
    };
}; // namespace South