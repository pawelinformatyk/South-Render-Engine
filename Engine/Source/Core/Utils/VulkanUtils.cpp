#include "sthpch.h"

#include "VulkanUtils.h"

namespace South
{
std::string VulkanUtils::DeviceTypeToString(const VkPhysicalDeviceType InType)
{
    switch(InType)
    {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "Other";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "Integrated";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "Discrete";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "Virtual";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "CPU";
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
            return "Other";
    }
    return {};
}

VkImageView VulkanUtils::CreateImageView(const VkDevice           InDevice,
                                         const VkImage            InImage,
                                         const VkFormat           InFormat,
                                         const VkImageAspectFlags InAspectFlags)
{
    VkImageView OutImageView;

    const VkImageViewCreateInfo ViewInfo {
        .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext    = nullptr,
        .image    = InImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format   = InFormat,
        .subresourceRange =
            {
                .aspectMask     = InAspectFlags,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
    };

    vkCreateImageView(InDevice, &ViewInfo, nullptr, &OutImageView);

    return OutImageView;
}

VkSurfaceFormatKHR VulkanUtils::ChooseSwapSurfaceFormat(const VkPhysicalDevice InPhysDevice, const VkSurfaceKHR InSurface)
{
    uint32_t AvailableFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(InPhysDevice, InSurface, &AvailableFormatsCount, nullptr);

    std::vector<VkSurfaceFormatKHR> AvailableFormats;
    AvailableFormats.resize(AvailableFormatsCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(InPhysDevice, InSurface, &AvailableFormatsCount, AvailableFormats.data());

    const std::array RequestSurfaceImageFormat = {VK_FORMAT_B8G8R8A8_UNORM,
                                                  VK_FORMAT_R8G8B8A8_UNORM,
                                                  VK_FORMAT_B8G8R8_UNORM,
                                                  VK_FORMAT_R8G8B8_UNORM};

    constexpr VkColorSpaceKHR RequestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    // First check if only one format, VK_FORMAT_UNDEFINED, is available, which
    // would imply that any format is available
    if(AvailableFormats.size() == 1)
    {
        if(AvailableFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            VkSurfaceFormatKHR ret;
            ret.format     = RequestSurfaceImageFormat[0];
            ret.colorSpace = RequestSurfaceColorSpace;
            return ret;
        }
        else
        {
            // No point in searching another format
            return AvailableFormats[0];
        }
    }
    else
    {
        // Request several formats, the first found will be used
        for(const VkFormat RequestFormat : RequestSurfaceImageFormat)
        {
            for(const VkSurfaceFormatKHR AvailableFormat : AvailableFormats)
            {
                if(AvailableFormat.format == RequestFormat && AvailableFormat.colorSpace == RequestSurfaceColorSpace)
                {
                    return AvailableFormat;
                }
            }
        }

        // If none of the requested image formats could be found, use the first
        // available
        return AvailableFormats[0];
    }
}

} // namespace South