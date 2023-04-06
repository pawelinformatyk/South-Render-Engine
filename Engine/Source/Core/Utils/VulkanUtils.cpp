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

VkImageView VulkanUtils::CreateImageView(const VkDevice InDevice, VkImage InImage, VkFormat InFormat, VkImageAspectFlags InAspectFlags)
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

} // namespace South