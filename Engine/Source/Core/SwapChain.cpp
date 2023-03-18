#include "sthpch.h"

#include "Core/SwapChain.h"

#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace South
{

void SwapChain::Init(GLFWwindow& Window, VkSurfaceKHR InSurface)
{
    const RendererContext& Context          = Renderer::GetContext();
    VkDevice               LogicDevice      = Context.GetLogicalDevice();
    VkPhysicalDevice       PhysDev          = Context.GetGraphicCard().GetPhysicalDevice();
    uint32_t               QueueFamilyIndex = Context.GetGraphicQueueFamilyIndex();

    CreateSwapChain(PhysDev, LogicDevice, QueueFamilyIndex);
    CreateImages(LogicDevice);
}

void SwapChain::DeInit()
{
    VkDevice LogicDevice = Renderer::GetContext().GetLogicalDevice();

    for(auto ImageView : m_SwapChainImageViews)
    {
        vkDestroyImageView(LogicDevice, ImageView, nullptr);
    }

    vkDestroySwapchainKHR(LogicDevice, m_SwapChain, nullptr);
}

void SwapChain::CreateImages(VkDevice LogicDevice)
{
    uint32_t ImageCount;
    vkGetSwapchainImagesKHR(LogicDevice, m_SwapChain, &ImageCount, nullptr);

    m_SwapChainImages.resize(ImageCount);
    vkGetSwapchainImagesKHR(LogicDevice, m_SwapChain, &ImageCount, m_SwapChainImages.data());

    m_SwapChainImageViews.resize(m_SwapChainImages.size());

    const VkImageSubresourceRange SubresourceRange {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1,
    };

    VkImageViewCreateInfo CreateInfo {
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext            = nullptr,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = m_SwapChainImageFormat,
        .components       = VkComponentMapping(VK_COMPONENT_SWIZZLE_IDENTITY),
        .subresourceRange = SubresourceRange,
    };

    for(size_t i = 0; i < m_SwapChainImages.size(); i++)
    {
        CreateInfo.image = m_SwapChainImages[i];

        vkCreateImageView(LogicDevice, &CreateInfo, nullptr, &m_SwapChainImageViews[i]);
    }
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
{
    uint32_t FormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> AvailableFormats(FormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice, inSurface, &FormatCount, AvailableFormats.data());

    for(const auto& Format : AvailableFormats)
    {
        if(Format.format == VK_FORMAT_B8G8R8A8_SRGB && Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return Format;
        }
    }

    return AvailableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(VkPhysicalDevice inDevice, VkSurfaceKHR inSurface)
{
    // Presentation mode.
    uint32_t PresentModesCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount, nullptr);

    std::vector<VkPresentModeKHR> AvailablePresentModes(PresentModesCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(inDevice, inSurface, &PresentModesCount, AvailablePresentModes.data());

    for(const auto& PresentMode : AvailablePresentModes)
    {
        if(PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return PresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, GLFWwindow& Window)
{
    if(Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return Capabilities.currentExtent;
    }
    else
    {
        int Width, Height;
        glfwGetFramebufferSize(&Window, &Width, &Height);

        VkExtent2D ActualExtent = {static_cast<uint32_t>(Width), static_cast<uint32_t>(Height)};

        ActualExtent.width  = std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
        ActualExtent.height = std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

        return ActualExtent;
    }
}

void SwapChain::CreateSwapChain(VkPhysicalDevice PhysDevice, VkDevice LogicDevice, uint32_t QueueFamilyIndex)
{
    throw std::logic_error("The method or operation is not implemented.");
}

} // namespace South
