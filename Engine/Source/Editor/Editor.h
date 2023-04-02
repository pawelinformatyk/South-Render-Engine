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

    void BeginGui();
    void RenderGui();
    void EndGui();

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

    VkRenderPass m_SwapchainRenderPass = nullptr;

    VkExtent2D                  m_ViewportExtent = {100, 100};
    std::vector<VkImage>        m_ViewportImages;
    std::vector<VkDeviceMemory> m_ViewportImagesMemories;
    std::vector<VkImageView>    m_ViewportImagesViews;
    std::vector<VkFramebuffer>  m_ViewportFramebuffers;

    VkRenderPass     m_ViewportRenderPass       = nullptr;
    VkPipelineLayout m_ViewportPipelineLayout   = nullptr;
    VkPipeline       m_ViewportGraphicsPipeline = nullptr;

    VkImage        m_ViewportDepthImage       = nullptr;
    VkDeviceMemory m_ViewportDepthImageMemory = nullptr;
    VkImageView    m_ViewportDepthImageView   = nullptr;

    VkCommandPool                m_CommandPool = nullptr;
    std::vector<VkCommandBuffer> m_CommandBuffers;
    VkDescriptorSetLayout        m_DescriptorSetLayout = nullptr;

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

    std::vector<ImTextureID> m_ViewportTextures;
    ImTextureID              m_LastViewportTexture = nullptr;

    void CleanupSwapChain();

    void RecreateSwapChain();

    void CreateInstance();

    void CreateSurface();

    void CreateDevices();

    void CreateSwapchain();
    void CreateSwapchainImageViews();
    void CreateSwapchainRenderPass();
    void CreateSwapchainFramebuffers();

    void CreateViewportImages();
    void CreateViewportImageViews();
    void CreateViewportRenderPass();
    void CreateViewportGraphicsPipeline();
    void CreateViewportFramebuffers();

    void CreateDescriptorSetLayout();

    void CreateCommandPool();

    void CreateViewportDepthResources();

    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat FindDepthFormat();

    bool HasStencilComponent(VkFormat format);

    void CreateTextureImage();

    void CreateTextureImageView();

    void CreateTextureSampler();

    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

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

    void CreateCommandBuffers();

    void CreateSyncObjects();

    void UpdateCamera(uint32_t currentImage);

    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& InAvailableFormats);

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