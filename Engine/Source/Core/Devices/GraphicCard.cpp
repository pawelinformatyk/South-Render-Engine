#include "sthpch.h"

#include "Core/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Utils/VulkanUtils.h"
#include <optional>

namespace South
{

    GraphicCard* GraphicCard::Create(const CreateInfo& InCreateInfo)
    {
        const VkPhysicalDevice FoundDevice = FindPhysicalDevice(InCreateInfo);
        if (!FoundDevice) return nullptr;

        auto* Gpu              = new GraphicCard;
        Gpu->m_PhysicalDevice  = FoundDevice;
        Gpu->m_ExtensionsNames = InCreateInfo.RequiredExtensions;
        vkGetPhysicalDeviceFeatures(FoundDevice, &Gpu->m_Features);
        vkGetPhysicalDeviceProperties(FoundDevice, &Gpu->m_Properties);
        Gpu->m_TypeName = VulkanUtils::DeviceTypeToString(Gpu->m_Properties.deviceType);

        return Gpu;
    }

    VkPhysicalDevice GraphicCard::FindPhysicalDevice(const CreateInfo& InCreateInfo)
    {
        uint32_t GpuCount = 0;
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GpuCount, nullptr);

        std::vector<VkPhysicalDevice> AvailableGpus(GpuCount);
        vkEnumeratePhysicalDevices(InCreateInfo.VulkanInstance, &GpuCount, AvailableGpus.data());

        for (const VkPhysicalDevice Gpu : AvailableGpus)
        {
            if (!CheckFeatures(Gpu, InCreateInfo.RequiredFeatures)) continue;

            if (!CheckExtensions(Gpu, InCreateInfo.RequiredExtensions)) continue;

            return Gpu;
        }

        return nullptr;
    }

    bool GraphicCard::CheckFeatures(const VkPhysicalDevice InGpu, const VkPhysicalDeviceFeatures& InRequiredFeatures)
    {
        VkPhysicalDeviceFeatures FoundFeatures;
        vkGetPhysicalDeviceFeatures(InGpu, &FoundFeatures);

        auto IsFeatureNotPresent = [](const bool InLhs, const bool InRhs) { return InLhs && InLhs != InRhs; };

        if (IsFeatureNotPresent(InRequiredFeatures.robustBufferAccess, FoundFeatures.robustBufferAccess)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.fullDrawIndexUint32, FoundFeatures.fullDrawIndexUint32))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.fullDrawIndexUint32, FoundFeatures.fullDrawIndexUint32))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.imageCubeArray, FoundFeatures.imageCubeArray)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.independentBlend, FoundFeatures.independentBlend)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.geometryShader, FoundFeatures.geometryShader)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.tessellationShader, FoundFeatures.tessellationShader)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sampleRateShading, FoundFeatures.sampleRateShading)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.dualSrcBlend, FoundFeatures.dualSrcBlend)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.logicOp, FoundFeatures.logicOp)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.multiDrawIndirect, FoundFeatures.multiDrawIndirect)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.drawIndirectFirstInstance, FoundFeatures.drawIndirectFirstInstance))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.depthClamp, FoundFeatures.depthClamp)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.depthBiasClamp, FoundFeatures.depthBiasClamp)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.fillModeNonSolid, FoundFeatures.fillModeNonSolid)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.depthBounds, FoundFeatures.depthBounds)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.wideLines, FoundFeatures.wideLines)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.largePoints, FoundFeatures.largePoints)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.alphaToOne, FoundFeatures.alphaToOne)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.multiViewport, FoundFeatures.multiViewport)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.samplerAnisotropy, FoundFeatures.samplerAnisotropy)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.textureCompressionETC2, FoundFeatures.textureCompressionETC2))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.textureCompressionASTC_LDR,
                                FoundFeatures.textureCompressionASTC_LDR))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.textureCompressionBC, FoundFeatures.textureCompressionBC))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.occlusionQueryPrecise, FoundFeatures.occlusionQueryPrecise))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.pipelineStatisticsQuery, FoundFeatures.pipelineStatisticsQuery))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.vertexPipelineStoresAndAtomics,
                                FoundFeatures.vertexPipelineStoresAndAtomics))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.fragmentStoresAndAtomics, FoundFeatures.fragmentStoresAndAtomics))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderTessellationAndGeometryPointSize,
                                FoundFeatures.shaderTessellationAndGeometryPointSize))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderImageGatherExtended, FoundFeatures.shaderImageGatherExtended))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageExtendedFormats,
                                FoundFeatures.shaderStorageImageExtendedFormats))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageMultisample,
                                FoundFeatures.shaderStorageImageMultisample))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageReadWithoutFormat,
                                FoundFeatures.shaderStorageImageReadWithoutFormat))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageWriteWithoutFormat,
                                FoundFeatures.shaderStorageImageWriteWithoutFormat))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderUniformBufferArrayDynamicIndexing,
                                FoundFeatures.shaderUniformBufferArrayDynamicIndexing))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderSampledImageArrayDynamicIndexing,
                                FoundFeatures.shaderSampledImageArrayDynamicIndexing))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageBufferArrayDynamicIndexing,
                                FoundFeatures.shaderStorageBufferArrayDynamicIndexing))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageArrayDynamicIndexing,
                                FoundFeatures.shaderStorageImageArrayDynamicIndexing))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderClipDistance, FoundFeatures.shaderClipDistance)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderCullDistance, FoundFeatures.shaderCullDistance)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderFloat64, FoundFeatures.shaderFloat64)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderInt64, FoundFeatures.shaderInt64)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderInt16, FoundFeatures.shaderInt16)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderResourceResidency, FoundFeatures.shaderResourceResidency))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.shaderResourceMinLod, FoundFeatures.shaderResourceMinLod))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseBinding, FoundFeatures.sparseBinding)) return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidencyBuffer, FoundFeatures.sparseResidencyBuffer))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidencyImage2D, FoundFeatures.sparseResidencyImage2D))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidencyImage3D, FoundFeatures.sparseResidencyImage3D))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidency2Samples, FoundFeatures.sparseResidency2Samples))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidency4Samples, FoundFeatures.sparseResidency4Samples))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidency8Samples, FoundFeatures.sparseResidency8Samples))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidency16Samples, FoundFeatures.sparseResidency16Samples))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.sparseResidencyAliased, FoundFeatures.sparseResidencyAliased))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.variableMultisampleRate, FoundFeatures.variableMultisampleRate))
            return false;
        if (IsFeatureNotPresent(InRequiredFeatures.inheritedQueries, FoundFeatures.inheritedQueries)) return false;

        return true;
    }

    bool GraphicCard::CheckExtensions(const VkPhysicalDevice InGpu,
                                      const std::vector<const char*>& InRequiredExtensions)
    {
        uint32_t ExtensionCount;
        vkEnumerateDeviceExtensionProperties(InGpu, nullptr, &ExtensionCount, nullptr);

        std::vector<VkExtensionProperties> Extensions = std::vector<VkExtensionProperties>(ExtensionCount);
        vkEnumerateDeviceExtensionProperties(InGpu, nullptr, &ExtensionCount, Extensions.data());

        std::set<std::string> RequiredExtensionsSet(InRequiredExtensions.begin(), InRequiredExtensions.end());

        for (const VkExtensionProperties& Extension : Extensions)
        {
            RequiredExtensionsSet.erase(Extension.extensionName);
        }

        if (!RequiredExtensionsSet.empty()) { return false; }

        return true;
    }

    bool GraphicCard::CreateLogicalDevice(VkDevice& OutLogicalDevice, Queue& OutGraphicQueue) const
    {
        const std::optional<uint32_t> GraphicQueueFamilyIndex =
            FindQueueFamilyIndex(m_PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
        if (!GraphicQueueFamilyIndex.has_value()) { return false; }

        constexpr float QueuePrio = 1.f;
        const VkDeviceQueueCreateInfo GraphicQueueCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0x00,
            .queueFamilyIndex = GraphicQueueFamilyIndex.value(),
            .queueCount       = 1,
            .pQueuePriorities = &QueuePrio,
        };

        const VkDeviceCreateInfo LogicDeviceCreateInfo{
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0x00,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &GraphicQueueCreateInfo,
            .enabledExtensionCount   = static_cast<uint32_t>(m_ExtensionsNames.size()),
            .ppEnabledExtensionNames = m_ExtensionsNames.data(),
            .pEnabledFeatures        = &m_Features,
        };

        vkCreateDevice(m_PhysicalDevice, &LogicDeviceCreateInfo, nullptr, &OutLogicalDevice);

        VkQueue GraphicQueue = nullptr;
        vkGetDeviceQueue(OutLogicalDevice, GraphicQueueFamilyIndex.value(), 0, &GraphicQueue);

        OutGraphicQueue.m_Queue       = GraphicQueue;
        OutGraphicQueue.m_QueueFamily = GraphicQueueFamilyIndex.value();

        return true;
    }

    VkPhysicalDevice GraphicCard::GetPhysicalDevice() const { return m_PhysicalDevice; }

    const VkPhysicalDeviceFeatures& GraphicCard::GetFeatures() const { return m_Features; }

    const VkPhysicalDeviceProperties& GraphicCard::GetProperties() const { return m_Properties; }

    const std::string& GraphicCard::GetTypeName() const { return m_TypeName; }

    std::optional<uint32_t> GraphicCard::FindQueueFamilyIndex(const VkPhysicalDevice InGpu,
                                                              const VkQueueFlagBits InFlags)
    {
        uint32_t QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(InGpu, &QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(InGpu, &QueueFamilyCount, QueueFamilies.data());

        for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
        {
            if (QueueFamilies[QueueFamilyIndex].queueFlags & InFlags) { return std::optional(QueueFamilyIndex); }
        }

        return {};
    }

} // namespace South