#include "sthpch.h"

#include "Core/Window/SwapChain.h"

#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Utils/VulkanUtils.h"

#include <GLFW/glfw3.h>

namespace South
{

namespace Private
{

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& InAvailableFormats)
{
    const std::array RequestSurfaceImageFormat = {VK_FORMAT_B8G8R8A8_UNORM,
                                                  VK_FORMAT_R8G8B8A8_UNORM,
                                                  VK_FORMAT_B8G8R8_UNORM,
                                                  VK_FORMAT_R8G8B8_UNORM};

    constexpr VkColorSpaceKHR RequestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    // First check if only one format, VK_FORMAT_UNDEFINED, is available, which
    // would imply that any format is available
    if(InAvailableFormats.size() == 1)
    {
        if(InAvailableFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            VkSurfaceFormatKHR ret;
            ret.format     = RequestSurfaceImageFormat[0];
            ret.colorSpace = RequestSurfaceColorSpace;
            return ret;
        }
        else
        {
            // No point in searching another format
            return InAvailableFormats[0];
        }
    }
    else
    {
        // Request several formats, the first found will be used
        for(const VkFormat RequestFormat : RequestSurfaceImageFormat)
        {
            for(const VkSurfaceFormatKHR AvailableFormat : InAvailableFormats)
            {
                if(AvailableFormat.format == RequestFormat && AvailableFormat.colorSpace == RequestSurfaceColorSpace)
                {
                    return AvailableFormat;
                }
            }
        }

        // If none of the requested image formats could be found, use the first
        // available
        return InAvailableFormats[0];
    }
}

} // namespace Private

SwapChain::SwapChain(const CreateInfo& InInfo)
{
    m_PresentMode = InInfo.PresentMode;
    m_Size        = InInfo.Size;

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(InInfo.PhysDevice, InInfo.Surface, &SurfaceCapabilities);

    uint32_t SurfaceFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(InInfo.PhysDevice, InInfo.Surface, &SurfaceFormatsCount, nullptr);
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(InInfo.PhysDevice, InInfo.Surface, &SurfaceFormatsCount, SurfaceFormats.data());

    uint32_t PresentModesCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(InInfo.PhysDevice, InInfo.Surface, &PresentModesCount, nullptr);
    std::vector<VkPresentModeKHR> PresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(InInfo.PhysDevice, InInfo.Surface, &PresentModesCount, PresentModes.data());

    const VkSurfaceFormatKHR SurfaceFormat = Private::ChooseSwapSurfaceFormat(SurfaceFormats);
    m_ImageFormat                          = SurfaceFormat.format;
    m_ImageColorSpace                      = SurfaceFormat.colorSpace;

    uint32_t ImagesCount = SurfaceCapabilities.minImageCount + 1;
    if(SurfaceCapabilities.maxImageCount > 0 && ImagesCount > SurfaceCapabilities.maxImageCount)
    {
        ImagesCount = SurfaceCapabilities.maxImageCount;
    }

    std::array queueFamilyIndices = {InInfo.LogicalDevice.GetGraphicQueueFamilyIndex(), InInfo.LogicalDevice.GetPresentQueueFamilyIndex()};

    const VkSwapchainCreateInfoKHR VulkanSwapChainInfo {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = nullptr,
        .surface               = InInfo.Surface,
        .minImageCount         = ImagesCount,
        .imageFormat           = m_ImageFormat,
        .imageColorSpace       = m_ImageColorSpace,
        .imageExtent           = m_Size,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size()),
        .pQueueFamilyIndices   = queueFamilyIndices.data(),
        .preTransform          = SurfaceCapabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = m_PresentMode,
        .clipped               = true,
    };

    const VkDevice Device = InInfo.LogicalDevice.GetLogicalDevice();

    vkCreateSwapchainKHR(Device, &VulkanSwapChainInfo, nullptr, &m_VulkanSwapChain);

    vkGetSwapchainImagesKHR(Device, m_VulkanSwapChain, &ImagesCount, nullptr);
    m_Images.resize(ImagesCount);
    vkGetSwapchainImagesKHR(Device, m_VulkanSwapChain, &ImagesCount, m_Images.data());

    m_ImageViews.reserve(m_ImageViews.size());
    for(const VkImage& Image : m_Images)
    {
        m_ImageViews.emplace_back(VulkanUtils::CreateImageView(Device, Image, m_ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT));
    }
}

SwapChain::~SwapChain()
{
}

} // namespace South