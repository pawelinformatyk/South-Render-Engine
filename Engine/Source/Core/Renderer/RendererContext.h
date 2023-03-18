#pragma once

#include "glm.hpp"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{
class GraphicCard;
class LogicalDeviceAndQueues;

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
    void Init();
    void DeInit();

    VkInstance         GetVulkanInstance() const;
    VkQueue            GetGraphicQueue() const;
    uint32_t           GetGraphicQueueFamilyIndex() const;
    VkDevice           GetLogicalDevice() const;
    const GraphicCard& GetGraphicCard() const;
    VkRenderPass       GetRenderPass() const;
    VkCommandBuffer    GetCommandBuffer() const;
    VkCommandPool      GetCommandPool() const;
    VkDescriptorPool   GetDescriptorPool() const;
    VkPipelineLayout   GetPipelineLayout() const;

private:
    void CreateInstance();

    void CreateSurface(GLFWwindow& InWindow);
    void CreateSwapChain(GLFWwindow& InWindow);
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateCommandPool();
    void CreateFramebuffers();
    void CreateCommandBuffers();
    void CreateSyncObjects();
    void CreateDescriptorPool();

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice InDevice, VkSurfaceKHR InSurface);
    VkPresentModeKHR   ChooseSwapPresentMode(VkPhysicalDevice InDevice, VkSurfaceKHR InSurface);
    VkExtent2D         ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& InCapabilities, GLFWwindow& InWindow);

    std::vector<const char*> GetRequiredInstanceExtensions();

    VkInstance m_VulkanInstance = nullptr;

    VkSurfaceKHR               m_Surface   = nullptr;
    VkSwapchainKHR             m_SwapChain = nullptr;
    std::vector<VkImage>       m_SwapChainImages;
    VkFormat                   m_SwapChainImageFormat;
    VkExtent2D                 m_SwapChainExtent;
    std::vector<VkImageView>   m_SwapChainImageViews;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    VkImage        m_DepthImage;
    VkDeviceMemory m_DepthImageMemory;
    VkImageView    m_DepthImageView;

    VkRenderPass     m_RenderPass     = nullptr;
    VkPipelineLayout m_PipelineLayout = nullptr;

private:
    VkPipeline m_GraphicsPipeline = nullptr;

    VkDescriptorPool m_DescriptorPool;

    VkCommandPool   m_CommandPool             = nullptr;
    VkCommandBuffer m_CommandBuffer           = nullptr;
    VkSemaphore     m_ImageAvailableSemaphore = nullptr;
    VkSemaphore     m_RenderFinishedSemaphore = nullptr;
    VkFence         m_FlightFence             = nullptr;

    GraphicCard*            m_GPU           = nullptr;
    LogicalDeviceAndQueues* m_LogicalDevice = nullptr;

    //~ Validations layers.
private:
    void CreateMessenger();
    void DestroyMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      InMessageSeverity,
                                                                    VkDebugUtilsMessageTypeFlagsEXT             InMessageType,
                                                                    const VkDebugUtilsMessengerCallbackDataEXT* InCallbackData,
                                                                    void*                                       InUserData);

    bool CheckValidationLayers() const;

    const std::vector<const char*> m_RequiredValidationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool m_bEnableValidationLayers = false;
#else
    const bool m_bEnableValidationLayers = true;
#endif

    VkDebugUtilsMessengerEXT m_Messenger;
    //~ Validations layers.
};
}; // namespace South