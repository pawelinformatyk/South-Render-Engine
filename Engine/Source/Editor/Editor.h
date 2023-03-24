#pragma once

#include "imconfig.h"

#include <vulkan/vulkan_core.h>

#define MAX_FRAMES_IN_FLIGHT 2

struct GLFWwindow;

namespace South
{
class EditorViewport;
class Camera;
class GraphicCard;
class LogicalDeviceAndQueues;
class UniformBuffer;
class VertexIndexBuffer;

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities {};
    std::vector<VkSurfaceFormatKHR> formats {};
    std::vector<VkPresentModeKHR>   presentModes {};
};

class Editor
{
public:
    Editor(GLFWwindow& InWindow);
    ~Editor();

    void Tick();

    void BeginFrame();
    void EndFrame();

    void Render();

    void BeginGUI();
    void RenderGUI();
    void EndGUI();

    void Present();

private:
    EditorViewport* m_MainViewport = nullptr;

    VkInstance m_VulkanInstance = nullptr;

    GraphicCard*            m_GPU           = nullptr;
    LogicalDeviceAndQueues* m_LogicalDevice = nullptr;

    GLFWwindow*  m_Window  = nullptr;
    VkSurfaceKHR m_Surface = nullptr;

    VkSwapchainKHR             m_SwapChain = nullptr;
    std::vector<VkImage>       m_SwapChainImages;
    VkFormat                   m_SwapChainImageFormat = {};
    VkExtent2D                 m_SwapChainExtent      = {};
    std::vector<VkImageView>   m_SwapChainImageViews;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    VkImageView m_LastFrame = nullptr;

    VkRenderPass          m_RenderPass          = nullptr;
    VkDescriptorSetLayout m_DescriptorSetLayout = nullptr;
    VkPipelineLayout      m_PipelineLayout      = nullptr;
    VkPipeline            m_GraphicsPipeline    = nullptr;

    VkCommandPool m_CommandPool = nullptr;

    VkImage        m_DepthImage       = nullptr;
    VkDeviceMemory m_DepthImageMemory = nullptr;
    VkImageView    m_DepthImageView   = nullptr;

    std::vector<VkCommandBuffer> m_CommandBuffers;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence>     m_InFlightFences;
    uint32_t                 m_CurrentFrameIndex = 0;
    uint32_t                 m_CurrentImageIndex = 0;

    VkImage        m_TextureImage       = nullptr;
    VkDeviceMemory m_TextureImageMemory = nullptr;
    VkImageView    m_TextureImageView   = nullptr;
    VkSampler      m_TextureSampler     = nullptr;

    VertexIndexBuffer* m_QuadBuffer = nullptr;

    std::array<UniformBuffer*, MAX_FRAMES_IN_FLIGHT> m_CameraUbos {};

    VkDescriptorPool             m_DescriptorPool = nullptr;
    std::vector<VkDescriptorSet> m_DescriptorSets;

    ImTextureID m_TextureId;

    void CleanupSwapChain();

    void RecreateSwapChain();

    void CreateInstance();

    void CreateSurface();

    void CreateDevices();

    void CreateSwapChain();

    void CreateImageViews();

    void CreateRenderPass();

    void CreateDescriptorSetLayout();

    void CreateGraphicsPipeline();

    void CreateFramebuffers();

    void CreateCommandPool();

    void CreateDepthResources();

    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat FindDepthFormat();

    bool HasStencilComponent(VkFormat format);

    void CreateTextureImage();

    void CreateTextureImageView();

    void CreateTextureSampler();

    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void CreateImage(uint32_t              width,
                     uint32_t              height,
                     VkFormat              format,
                     VkImageTiling         tiling,
                     VkImageUsageFlags     usage,
                     VkMemoryPropertyFlags properties,
                     VkImage&              image,
                     VkDeviceMemory&       imageMemory);

    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void CreateCameraUbos();

    void CreateDescriptorPool();

    void CreateDescriptorSets();

    void CreateBuffer(VkDeviceSize          size,
                      VkBufferUsageFlags    usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer&             buffer,
                      VkDeviceMemory&       bufferMemory);

    VkCommandBuffer BeginSingleTimeCommands();

    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void CreateCommandBuffers();

    void CreateSyncObjects();

    void UpdateCamera(uint32_t currentImage);

    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    std::vector<const char*> GetRequiredInstanceExtensions() const;

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

    VkDebugUtilsMessengerEXT m_Messenger = nullptr;
    //~ Validations layers.
};

} // namespace South