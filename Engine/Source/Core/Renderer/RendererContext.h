#pragma once

#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/CommandPool.h"
#include "Core/Renderer/RenderPass.h"
#include "Core/Window/SwapChain.h"
#include "Core/Window/Window.h"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

// Class holding all "global" vulkan related variables.
class SRendererContext
{
public:
    struct SCreateInfo
    {
        GLFWwindow& GlfwWindow;
    };

    static SRendererContext& Get();

    SRendererContext(SRendererContext const&) = delete;
    void operator=(SRendererContext const&)  = delete;

    void Init(const SCreateInfo& Info);
    void Deinit();

    VkInstance                    GetVulkanInstance() const;
    VkSurfaceKHR                  GetSurface() const;
    VkCommandPool                 GetCommandPool() const;
    const SGraphicCard&            GetGraphicCard() const;
    const SLogicalDeviceAndQueues& GetDeviceAndQueues() const;
    SSwapChain&                    GetSwapChain() const;
    SRenderPass&                   GetRenderPass() const;

private:
    SRendererContext() = default;

    void CreateInstance();
    void CreateSurface(GLFWwindow& InGlfWwindow);
    void CreateDevices();

    VkInstance                              m_VulkanInstance      = nullptr;
    VkSurfaceKHR                            m_Surface             = nullptr;
    std::unique_ptr<SCommandPool>            m_CommandPool         = nullptr;
    std::unique_ptr<SGraphicCard>            m_Gpu                 = nullptr;
    std::unique_ptr<SLogicalDeviceAndQueues> m_LogicalDevice       = nullptr;
    std::unique_ptr<SSwapChain>              m_SwapChain           = nullptr;
    std::unique_ptr<SRenderPass>             m_SwapChainRenderPass = nullptr;

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