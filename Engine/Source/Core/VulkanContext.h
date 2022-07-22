#pragma once

#include "Core/VulkanDevice.h"
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
        static VulkanContext& Get();

    public:
        VulkanContext(VulkanContext const&) = delete;
        void operator=(VulkanContext const&) = delete;

        void Init();
        void DeInit();
        void Tick();

        VkInstance GetVulkanInstance();
        VulkanDevice& GetCurrentDevice();

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationMessageCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    private:
        VulkanContext(){};
        ~VulkanContext(){};

        void CreateInstance();
        void CreateMessenger();

        void CreateSurface(GLFWwindow& Window);
        void CreateSwapChain(GLFWwindow& Window);
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

        void DestroyMessenger();

        std::vector<const char*> GetRequiredInstanceExtensions();

        bool CheckValidationLayers();

        VkInstance VulkanInstance = VK_NULL_HANDLE;

        const std::vector<const char*> RequiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
        const bool bEnableValidationLayers = false;
#else
        const bool bEnableValidationLayers = true;
#endif
        VkDebugUtilsMessengerEXT Messenger;

        VkSurfaceKHR Surface     = VK_NULL_HANDLE;
        VkSwapchainKHR SwapChain = VK_NULL_HANDLE;
        std::vector<VkImage> SwapChainImages;
        VkFormat SwapChainImageFormat;
        VkExtent2D SwapChainExtent;
        std::vector<VkImageView> SwapChainImageViews;
        std::vector<VkFramebuffer> SwapChainFramebuffers;

        VkRenderPass RenderPass         = VK_NULL_HANDLE;
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
        VkPipeline GraphicsPipeline     = VK_NULL_HANDLE;

        VkCommandPool CommandPool           = VK_NULL_HANDLE;
        VkCommandBuffer CommandBuffer       = VK_NULL_HANDLE;
        VkSemaphore ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore RenderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence FlightFence                 = VK_NULL_HANDLE;

        std::unique_ptr<VulkanDevice> Device = nullptr;
        VulkanVertexIndexBuffer* VI_Buffer   = nullptr;

        bool bCanTick = false;
    };
}; // namespace South