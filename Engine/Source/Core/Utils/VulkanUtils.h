#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

    namespace VulkanUtils
    {
        // #TODO : Should it be string?
        std::string DeviceTypeToString(const VkPhysicalDeviceType InType);
    } // namespace VulkanUtils

} // namespace South
