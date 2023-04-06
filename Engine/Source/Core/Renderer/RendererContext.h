#pragma once

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

    VkInstance         GetVulkanInstance() const;
    VkQueue            GetGraphicQueue() const;
    uint32_t           GetGraphicQueueFamilyIndex() const;
    VkDevice           GetLogicalDevice() const;
    const GraphicCard& GetGraphicCard() const;

private:
    void CreateInstance();
    void CreateSurface(GLFWwindow& InGlfWwindow);
    void CreateDevices();

    VkInstance m_VulkanInstance = nullptr;

    GraphicCard*            m_Gpu           = nullptr;
    LogicalDeviceAndQueues* m_LogicalDevice = nullptr;

    VkSurfaceKHR m_Surface   = nullptr;
    SwapChain*   m_SwapChain = nullptr;

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