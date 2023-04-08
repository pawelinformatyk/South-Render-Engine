#include "sthpch.h"

#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/RendererContext.h"

#include "CommandPool.h"
#include "Core/Window/SwapChain.h"

#include <GLFW/glfw3.h>

namespace South
{

RendererContext::RendererContext(const CreateInfo& Info)
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

    m_SwapChain = new SwapChain(SwapChain::CreateInfo {.LogicalDevice = *m_LogicalDevice,
                                                       .PhysDevice    = m_Gpu->GetPhysicalDevice(),
                                                       .Surface       = m_Surface,
                                                       .Size          = VkExtent2D {1280, 720},
                                                       .PresentMode   = VK_PRESENT_MODE_FIFO_KHR});


    m_CommandPool = std::unique_ptr<CommandPool>(
        CommandPool::Create(m_LogicalDevice->GetLogicalDevice(),
                            {VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_LogicalDevice->GetGraphicQueueFamilyIndex()}));
}

RendererContext::~RendererContext()
{
    CommandPool::Destroy(*m_CommandPool);
    LogicalDeviceAndQueues::Destroy(*m_LogicalDevice);

    if(m_bEnableValidationLayers)
    {
        DestroyMessenger();
    }

    vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
    vkDestroyInstance(m_VulkanInstance, nullptr);
}

VkInstance RendererContext::GetVulkanInstance() const
{
    return m_VulkanInstance;
}

VkSurfaceKHR RendererContext::GetSurface() const
{
    return m_Surface;
}

VkQueue RendererContext::GetGraphicQueue() const
{
    return m_LogicalDevice->GetGraphicQueue();
}

uint32_t RendererContext::GetGraphicQueueFamilyIndex() const
{
    return m_LogicalDevice->GetGraphicQueueFamilyIndex();
}

VkDevice RendererContext::GetLogicalDevice() const
{
    return m_LogicalDevice->GetLogicalDevice();
}

VkPhysicalDevice RendererContext::GetPhysicalDevice()
{
    return m_Gpu->GetPhysicalDevice();
}

const GraphicCard& RendererContext::GetGraphicCard() const
{
    return *m_Gpu;
}

const LogicalDeviceAndQueues& RendererContext::GetDeviceAndQueues() const
{
    return *m_LogicalDevice;
}

VkSwapchainKHR RendererContext::GetSwapChain() const
{
    return m_SwapChain->GetVulkanSwapChain();
}

VkRenderPass RendererContext::GetSwapchainRenderPass() const
{
    return m_SwapChain->GetRenderPass();
}

VkFramebuffer RendererContext::GetSwapChainFramebuffer(const uint32_t InIndex) const
{
    return m_SwapChain->GetFramebuffer(InIndex);
}

void RendererContext::RecreateSwapChain(int InWidth, int InHeight)
{
    // #TODO: Change without waiting
    // vkDeviceWaitIdle(Renderer::GetContext().GetDeviceAndQueues().GetLogicalDevice());
    //
    // CleanupSwapChain();
    //
    // CreateSwapchain(InWidth, InHeight);
    // CreateSwapchainImageViews();
    // CreateSwapchainFramebuffers();
}

void RendererContext::CreateInstance()
{
    const VkApplicationInfo AppInfo {
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

void RendererContext::CreateSurface(GLFWwindow& InGlfWwindow)
{
    glfwCreateWindowSurface(m_VulkanInstance, &InGlfWwindow, nullptr, &m_Surface);
}

void RendererContext::CreateDevices()
{
    m_Gpu = std::unique_ptr<GraphicCard>(GraphicCard::Create(m_VulkanInstance,
                                                             GraphicCard::CreateInfo {
                                                                 .RequiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                                                                 .RequiredFeatures =
                                                                     {
                                                                         .multiViewport = true,
                                                                     },
                                                             }));


    m_LogicalDevice = std::unique_ptr<LogicalDeviceAndQueues>(LogicalDeviceAndQueues::Create(*m_Gpu, m_Surface));
}

std::vector<const char*> RendererContext::GetRequiredInstanceExtensions() const
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

void RendererContext::CreateMessenger()
{
    const VkDebugUtilsMessengerCreateInfoEXT CreateInfo {
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

void RendererContext::DestroyMessenger()
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

VkBool32 RendererContext::ValidationMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      InMessageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT             InMessageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* InCallbackData,
                                                    void*                                       InUserData)
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

bool RendererContext::CheckValidationLayers() const
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
