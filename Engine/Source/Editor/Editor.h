#pragma once

#include "Example/VulkanExampleApp.h"
#include "imconfig.h"

#include <vulkan/vulkan_core.h>

#define MAX_FRAMES_IN_FLIGHT 2

struct GLFWwindow;

namespace South
{
class EditorViewport;
class Camera;
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
    Editor(VkExtent2D InViewportExtent, GLFWwindow& InWindow);
    ~Editor();

    void OnEvent(const Event& InEvent);

    void Tick(double InFrameTime_Sec);

    void BeginFrame();
    void EndFrame();

    void Render();

    void BeginGui();
    void RenderGui();
    void EndGui();

    void Present();

private:
    double m_LastFrame_Sec = 0.;

    EditorViewport* m_MainViewport = nullptr;

    GLFWwindow* m_Window = nullptr;

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

    VkFormat   m_ViewportImageFormat = VK_FORMAT_R8G8B8A8_SRGB;
    VkExtent2D m_ViewportExtent      = {100, 100};

    std::vector<VkImage>        m_ViewportImages;
    std::vector<VkDeviceMemory> m_ViewportImagesMemories;
    std::vector<VkImageView>    m_ViewportImagesViews;
    std::vector<VkFramebuffer>  m_ViewportFramebuffers;

    VkImage        m_ViewportDepthImage       = nullptr;
    VkDeviceMemory m_ViewportDepthImageMemory = nullptr;
    VkImageView    m_ViewportDepthImageView   = nullptr;

    VkRenderPass     m_ViewportRenderPass       = nullptr;
    VkPipelineLayout m_ViewportPipelineLayout   = nullptr;
    VkPipeline       m_ViewportGraphicsPipeline = nullptr;

    std::vector<ImTextureID> m_ViewportTextures;
    ImTextureID              m_LastViewportTexture = nullptr;

    void CreateViewportImages();
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

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
};

} // namespace South