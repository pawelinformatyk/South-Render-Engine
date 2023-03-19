#pragma once

#include <glm.hpp>
#include <vulkan/vulkan_core.h>

#define MAX_FRAMES_IN_FLIGHT 2

struct GLFWwindow;

namespace South
{
class GraphicCard;
class LogicalDeviceAndQueues;
class UniformBuffer;
class VertexIndexBuffer;

class Window;

namespace VulkanTutorial
{

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities {};
    std::vector<VkSurfaceFormatKHR> formats {};
    std::vector<VkPresentModeKHR>   presentModes {};
};

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void ProcessEvents();

    float m_FrameTime_Sec = 0.f;

    std::unique_ptr<Window> m_Window = nullptr;

    VkInstance   m_VulkanInstance = nullptr;
    VkSurfaceKHR surface          = nullptr;

    GraphicCard*            m_GPU           = nullptr;
    LogicalDeviceAndQueues* m_LogicalDevice = nullptr;

    VkSwapchainKHR             swapChain = nullptr;
    std::vector<VkImage>       swapChainImages;
    VkFormat                   swapChainImageFormat = {};
    VkExtent2D                 m_SwapChainExtent    = {};
    std::vector<VkImageView>   swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass          renderPass          = nullptr;
    VkDescriptorSetLayout descriptorSetLayout = nullptr;
    VkPipelineLayout      pipelineLayout      = nullptr;
    VkPipeline            graphicsPipeline    = nullptr;

    VkCommandPool commandPool = nullptr;

    VkImage        depthImage       = nullptr;
    VkDeviceMemory depthImageMemory = nullptr;
    VkImageView    depthImageView   = nullptr;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
    uint32_t                 currentFrame = 0;

    VkImage        textureImage       = nullptr;
    VkDeviceMemory textureImageMemory = nullptr;
    VkImageView    textureImageView   = nullptr;
    VkSampler      textureSampler     = nullptr;

    VertexIndexBuffer* m_QuadBuffer = nullptr;

    std::array<UniformBuffer*, MAX_FRAMES_IN_FLIGHT> m_CameraUbos;

    VkDescriptorPool             descriptorPool = nullptr;
    std::vector<VkDescriptorSet> descriptorSets;


    bool framebufferResized = false;


    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void cleanupSwapChain();

    void recreateSwapChain();

    void createInstance();

    void createSurface();

    void CreateDevices();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();

    void createCommandPool();

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void createTextureImage();

    void createTextureImageView();

    void createTextureSampler();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void createImage(uint32_t              width,
                     uint32_t              height,
                     VkFormat              format,
                     VkImageTiling         tiling,
                     VkImageUsageFlags     usage,
                     VkMemoryPropertyFlags properties,
                     VkImage&              image,
                     VkDeviceMemory&       imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void CreateCameraUbos();

    void createDescriptorPool();

    void createDescriptorSets();

    void createBuffer(VkDeviceSize          size,
                      VkBufferUsageFlags    usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer&             buffer,
                      VkDeviceMemory&       bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void UpdateCameraBuffer(uint32_t currentImage);

    void DrawFrame();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

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

} // namespace VulkanTutorial

} // namespace South
