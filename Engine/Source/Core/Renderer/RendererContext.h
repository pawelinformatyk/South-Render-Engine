#pragma once

#include "CommandPool.h"
#include "Core/Window/Window.h"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

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

    static RendererContext& Get();

    RendererContext(RendererContext const&) = delete;
    void operator=(RendererContext const&)  = delete;

    void Init(const CreateInfo& Info);
    void Deinit();

    VkInstance                    GetVulkanInstance() const;
    VkSurfaceKHR                  GetSurface() const;
    VkCommandPool                 GetCommandPool() const;
    const GraphicCard&            GetGraphicCard() const;
    const LogicalDeviceAndQueues& GetDeviceAndQueues() const;
    SwapChain&                    GetSwapChain() const;
    VkRenderPass                  GetRenderPass() const;

private:
    RendererContext() = default;

    void CreateInstance();
    void CreateSurface(GLFWwindow& InGlfWwindow);
    void CreateDevices();

    VkInstance                              m_VulkanInstance      = nullptr;
    VkSurfaceKHR                            m_Surface             = nullptr;
    std::unique_ptr<CommandPool>            m_CommandPool         = nullptr;
    std::unique_ptr<GraphicCard>            m_Gpu                 = nullptr;
    std::unique_ptr<LogicalDeviceAndQueues> m_LogicalDevice       = nullptr;
    std::unique_ptr<SwapChain>              m_SwapChain           = nullptr;
    VkRenderPass                            m_SwapChainRenderPass = nullptr;

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