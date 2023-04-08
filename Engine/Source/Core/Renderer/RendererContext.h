#pragma once

#include "CommandPool.h"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

class SwapChain;
class GraphicCard;
class LogicalDeviceAndQueues;

// Class holding all "global" vulkan related variables.
class RendererContext
{
public:
    struct CreateInfo
    {
        GLFWwindow& GlfwWindow;
    };

    explicit RendererContext(const CreateInfo& Info);
    ~RendererContext();

    VkInstance                    GetVulkanInstance() const;
    VkSurfaceKHR                  GetSurface() const;
    VkQueue                       GetGraphicQueue() const;
    uint32_t                      GetGraphicQueueFamilyIndex() const;
    VkDevice                      GetLogicalDevice() const;
    VkPhysicalDevice              GetPhysicalDevice();
    const GraphicCard&            GetGraphicCard() const;
    const LogicalDeviceAndQueues& GetDeviceAndQueues() const;

    VkSwapchainKHR GetSwapChain() const;
    VkRenderPass   GetSwapchainRenderPass() const;
    VkFramebuffer  GetSwapChainFramebuffer(uint32_t InIndex) const;

    void RecreateSwapChain(int InWidth, int InHeight);

private:
    void CreateInstance();
    void CreateSurface(GLFWwindow& InGlfWwindow);
    void CreateDevices();

    VkInstance m_VulkanInstance = nullptr;

    VkSurfaceKHR m_Surface = nullptr;

    std::unique_ptr<GraphicCard>            m_Gpu           = nullptr;
    std::unique_ptr<LogicalDeviceAndQueues> m_LogicalDevice = nullptr;

    SwapChain* m_SwapChain = nullptr;

    std::unique_ptr<CommandPool> m_CommandPool;

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

    VkDebugUtilsMessengerEXT m_Messenger {};
    //~ Validations layers.
};
}; // namespace South