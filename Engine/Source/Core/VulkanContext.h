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
        // ~Static
    public:
        static VulkanContext& Get();

        // ~Static
    public:
        VulkanContext(VulkanContext const&) = delete;
        void operator=(VulkanContext const&) = delete;

        void Init();
        void DeInit();
        void Tick();

        VkInstance GetVulkanInstance() const { return m_VulkanInstance; }
        VulkanDevice& GetCurrentDevice() const { return *m_Device; };
        VkRenderPass GetRenderPass() const { return m_RenderPass; };
        VkCommandBuffer GetCommandBuffer() const { return m_CommandBuffer; }
        VkCommandPool GetCommandPool() const { return m_CommandPool; }

    private:
        VulkanContext(){};
        ~VulkanContext(){};

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

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkPresentModeKHR ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

        void RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

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

        VkCommandPool m_CommandPool           = VK_NULL_HANDLE;
        VkCommandBuffer m_CommandBuffer       = VK_NULL_HANDLE;
        VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_FlightFence                 = VK_NULL_HANDLE;

        std::unique_ptr<VulkanDevice> m_Device = nullptr;
        VulkanVertexIndexBuffer* m_VI_Buffer   = nullptr;

        bool m_bCanTick = false;

        //~ Validations layers.
    private:
        void CreateMessenger();
        void DestroyMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationMessageCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

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