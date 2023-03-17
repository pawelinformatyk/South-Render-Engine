#pragma once

#include <glm.hpp>
#include <vulkan/vulkan_core.h>

struct GLFWwindow;

namespace South
{

class Window;

namespace VulkanTutorial
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities {};
    std::vector<VkSurfaceFormatKHR> formats {};
    std::vector<VkPresentModeKHR>   presentModes {};
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    std::unique_ptr<Window> m_Window = nullptr;

    VkInstance   m_VulkanInstance = nullptr;
    VkSurfaceKHR surface          = nullptr;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice         device         = nullptr;

    VkQueue graphicsQueue = nullptr;
    VkQueue presentQueue  = nullptr;

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

    VkImage        textureImage       = nullptr;
    VkDeviceMemory textureImageMemory = nullptr;
    VkImageView    textureImageView   = nullptr;
    VkSampler      textureSampler     = nullptr;

    VkBuffer       vertexBuffer       = nullptr;
    VkDeviceMemory vertexBufferMemory = nullptr;
    VkBuffer       indexBuffer        = nullptr;
    VkDeviceMemory indexBufferMemory  = nullptr;

    std::vector<VkBuffer>       uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*>          uniformBuffersMapped;

    VkDescriptorPool             descriptorPool = nullptr;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
    uint32_t                 currentFrame = 0;

    bool framebufferResized = false;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void cleanupSwapChain();

    void recreateSwapChain();

    void createInstance();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

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

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

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

    void updateUniformBuffer(uint32_t currentImage);

    void DrawFrame();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

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
