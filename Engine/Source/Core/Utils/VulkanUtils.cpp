#include "sthpch.h"

#include "VulkanUtils.h"

namespace South
{

    std::string VulkanUtils::DeviceTypeToString(const VkPhysicalDeviceType InType)
    {
        switch (InType)
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

} // namespace South