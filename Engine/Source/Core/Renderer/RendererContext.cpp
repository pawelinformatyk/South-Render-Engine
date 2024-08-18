#include "Core/Renderer/RendererContext.h"
#include "Core/Utils/VulkanUtils.h"
#include "GLFW/glfw3.h"

namespace South
{

SRendererContext& SRendererContext::Get()
{
    static SRendererContext s_Context;

    return s_Context;
}

void SRendererContext::Init(const SCreateInfo& Info)
{
    CreateInstance();

    if(m_bEnableValidationLayers)
    {
        if(!CheckValidationLayers())
        {
            STH_VK_WARN("ValidationsLayers not found.");
        }
        else
        {
            CreateMessenger();
        }
    }

    CreateSurface(Info.GlfwWindow);
    CreateDevices();

    const VkSurfaceFormatKHR SwapChainSurfaceFormat = VulkanUtils::ChooseSwapSurfaceFormat(m_Gpu->GetPhysicalDevice(), m_Surface);

    m_SwapChainRenderPass = std::unique_ptr<SRenderPass>(SRenderPass::Create(SRenderPass::SCreateInfo {
        .Format = SwapChainSurfaceFormat.format,
    }));

    m_SwapChain = std::unique_ptr<SSwapChain>(SSwapChain::Create(SSwapChain::SCreateInfo {
        .Surface       = m_Surface,
        .SurfaceFormat = SwapChainSurfaceFormat,
        .Size          = VkExtent2D {1280, 720},
        .PresentMode   = VK_PRESENT_MODE_FIFO_KHR,
    }));

    m_CommandPool = std::unique_ptr<SCommandPool>(
        SCommandPool::Create(m_LogicalDevice->GetLogicalDevice(),
                            {VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_LogicalDevice->GetGraphicQueueFamilyIndex()}));
}

void SRendererContext::Deinit()
{
    vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

    SRenderPass::Destroy(*m_SwapChainRenderPass);
    SSwapChain::Destroy(*m_SwapChain);
    SCommandPool::Destroy(*m_CommandPool);
    SLogicalDeviceAndQueues::Destroy(*m_LogicalDevice);

    if(m_bEnableValidationLayers)
    {
        DestroyMessenger();
    }

    vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
    vkDestroyInstance(m_VulkanInstance, nullptr);
}

VkInstance SRendererContext::GetVulkanInstance() const
{
    return m_VulkanInstance;
}

VkSurfaceKHR SRendererContext::GetSurface() const
{
    return m_Surface;
}

VkCommandPool SRendererContext::GetCommandPool() const
{
    return m_CommandPool->GetPool();
}

const SGraphicCard& SRendererContext::GetGraphicCard() const
{
    return *m_Gpu;
}

const SLogicalDeviceAndQueues& SRendererContext::GetDeviceAndQueues() const
{
    return *m_LogicalDevice;
}

SSwapChain& SRendererContext::GetSwapChain() const
{
    return *m_SwapChain;
}

SRenderPass& SRendererContext::GetRenderPass() const
{
    return *m_SwapChainRenderPass;
}

void SRendererContext::CreateInstance()
{
    constexpr VkApplicationInfo AppInfo {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "South",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "None",
        .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion         = VK_MAKE_API_VERSION(0, 1, 3, 0),
    };

    const std::vector<const char*> Extensions = GetRequiredInstanceExtensions();

    const VkInstanceCreateInfo CreateInfo {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = nullptr,
        .pApplicationInfo        = &AppInfo,
        .enabledLayerCount       = m_bEnableValidationLayers ? static_cast<uint32_t>(m_RequiredValidationLayers.size()) : 0,
        .ppEnabledLayerNames     = m_bEnableValidationLayers ? m_RequiredValidationLayers.data() : nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(Extensions.size()),
        .ppEnabledExtensionNames = Extensions.data(),
    };

    vkCreateInstance(&CreateInfo, nullptr, &m_VulkanInstance);
}

void SRendererContext::CreateSurface(GLFWwindow& InGlfWwindow)
{
    glfwCreateWindowSurface(m_VulkanInstance, &InGlfWwindow, nullptr, &m_Surface);
}

void SRendererContext::CreateDevices()
{
    m_Gpu = std::unique_ptr<SGraphicCard>(SGraphicCard::Create(m_VulkanInstance,
                                                             SGraphicCard::SCreateInfo {
                                                                 .RequiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                                                                 .RequiredFeatures =
                                                                     {
                                                                         .multiViewport = true,
                                                                     },
                                                             }));


    m_LogicalDevice = std::unique_ptr<SLogicalDeviceAndQueues>(SLogicalDeviceAndQueues::Create(*m_Gpu, m_Surface));
}

std::vector<const char*> SRendererContext::GetRequiredInstanceExtensions() const
{
    uint32_t     ExtensionCount = 0;
    const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);

    std::vector Extensions(GlfwExtensions, GlfwExtensions + ExtensionCount);

    if(m_bEnableValidationLayers)
    {
        Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return Extensions;
}

void SRendererContext::CreateMessenger()
{
    constexpr VkDebugUtilsMessengerCreateInfoEXT CreateInfo {
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext           = nullptr,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = ValidationMessageCallback,
        .pUserData       = nullptr,
    };

    if(const auto Func =
           reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_VulkanInstance, "vkCreateDebugUtilsMessengerEXT")))
    {
        Func(m_VulkanInstance, &CreateInfo, nullptr, &m_Messenger);
    }
    else
    {
        STH_VK_WARN("vkCreateDebugUtilsMessengerEXT not found");
    }
}

void SRendererContext::DestroyMessenger()
{
    if(const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
           vkGetInstanceProcAddr(m_VulkanInstance, "vkDestroyDebugUtilsMessengerEXT")))
    {
        Func(m_VulkanInstance, m_Messenger, nullptr);
        m_Messenger = nullptr;
    }
    else
    {
        STH_VK_WARN("vkDestroyDebugUtilsMessengerEXT not found");
    }
}

VkBool32 SRendererContext::ValidationMessageCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT InMessageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT              InMessageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT*  InCallbackData,
                                                    void*                                        InUserData)
{
    if(InCallbackData)
    {
        switch(InMessageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                STH_VK_ERR(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                STH_VK_WARN(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                STH_VK_INFO(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                STH_VK_TRACE(InCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                break;
        }
    }

    // Return always false.
    return VK_FALSE;
}

bool SRendererContext::CheckValidationLayers() const
{
    uint32_t Count;
    vkEnumerateInstanceLayerProperties(&Count, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(Count);
    vkEnumerateInstanceLayerProperties(&Count, AvailableLayers.data());

    for(const char* ReqLayer : m_RequiredValidationLayers)
    {
        bool bFound = false;

        for(const VkLayerProperties& AvLayer : AvailableLayers)
        {
            if(strcmp(ReqLayer, AvLayer.layerName) == 0)
            {
                bFound = true;
                break;
            }
        }

        if(!bFound)
        {
            return false;
        }
    }

    return true;
}

} // namespace South