#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

namespace VulkanUtils
{

// #TODO : Should it be string?
std::string DeviceTypeToString(VkPhysicalDeviceType InType);

VkImageView CreateImageView(VkDevice InDevice, VkImage InImage, VkFormat InFormat, VkImageAspectFlags InAspectFlags);

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkPhysicalDevice InPhysDevice, VkSurfaceKHR InSurface);

} // namespace VulkanUtils

} // namespace South
