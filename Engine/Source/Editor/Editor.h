#pragma once

// #include "imconfig.h"
#include "Camera.h"
#include "Scene/Scene.h"
#include "vulkan/vulkan_core.h"

#define MAX_FRAMES_IN_FLIGHT 2

namespace South
{
class SSceneViewport;
}
struct GLFWwindow;

namespace South
{

class SEvent;
class SUniformBuffer;
class SVertexIndexBuffer;

// #TODO: Editor is kinda lower level application class?
/**
 * Viewport (scene) + UI
 */
class SEditor
{
public:
     SEditor(VkExtent2D InViewportExtent, GLFWwindow& InWindow);
    ~SEditor();

    void OnEvent(const SEvent& InEvent);

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

    std::shared_ptr<SScene> Scene;

    std::unique_ptr<SSceneViewport> Viewport;

    std::vector<VkCommandBuffer> m_CommandBuffers;
    VkDescriptorSetLayout        m_DescriptorSetLayout = nullptr;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence>     m_InFlightFences;
    uint32_t                 m_CurrentFrameIndex = 0;
    uint32_t                 m_CurrentImageIndex = 0;

    VkSampler m_TextureSampler = nullptr;

    VkImage        m_SceneTextureImage       = nullptr;
    VkDeviceMemory m_SceneTextureImageMemory = nullptr;
    VkImageView    m_SceneTextureImageView   = nullptr;

    std::array<SUniformBuffer*, MAX_FRAMES_IN_FLIGHT> m_CameraUbos;

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

    VkRenderPass     m_ViewportRenderPass           = nullptr;
    VkPipelineLayout m_ViewportPipelineLayout       = nullptr;
    VkPipeline       m_ViewportGraphicsPipelineMesh = nullptr;
    VkPipeline       m_ViewportGraphicsPipelineGrid = nullptr;

    std::vector<void*> m_ViewportTextures;
    void*              m_LastViewportTexture = nullptr;

    void CreateViewportImages();
    void CreateViewportRenderPass();
    void CreateViewportGraphicsPipeline();
    void CreateViewportFramebuffers();

    void CreateDescriptorSetLayout();

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

    SCamera Camera;

    bool    bMoveCameraForward  = false;
    bool    bMoveCameraBackward = false;
    bool    bMoveCameraLeft     = false;
    bool    bMoveCameraRight    = false;
    bool    bMoveCameraUp       = false;
    bool    bMoveCameraDown     = false;
    bool    bCameraCanRotate    = false;
    SVector PreviousMouseMove;
};

} // namespace South